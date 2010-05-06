/**
 * @file conio.c
 *      Console versions of common I/O functions.
 * @note Uses the USART, and Pins 3.1 (Tx) and 3.0 (Rx).
 * @note CONSOLE_Init(), CONSOLE_Update(), CONSOLE_SendChar(),
         and CONSOLE_ReceiveChar()
        should be modified for porting to other platform.
 * @see PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont, Ch18
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28
 */
#include "main.h"
#include "conio.h"
#include <stdio.h>


extern uint8_t gErrorCode;


static void CONSOLE_WriteCharToBuffer( char );
static char CONSOLE_GetCharFromBuffer();

static void CONSOLE_SendChar( char );
static void CONSOLE_ReceiveChar();


enum {
    XON=  0x11,
    XOFF= 0x13
};


enum {
    RECV_BUFFER_LENGTH= 8, ///< The receive buffer length
    TRAN_BUFFER_LENGTH= 50 ///< The transmit buffer length
};


///@name module variables
//@{
static uint8_t _inReadIndex;     ///< Data in buffer that has been read
static uint8_t _inWaitingIndex;  ///< Data in buffer not yet read

static uint8_t _outWrittenIndex;  ///< Data in buffer that has been written
static uint8_t _outWaitingIndex;  ///< Data in buffer not yet written


static uint8_t _recvBuffer[RECV_BUFFER_LENGTH];
static uint8_t _tranBuffer[TRAN_BUFFER_LENGTH];
//@}


char putchar ( char ch )
{
    CONSOLE_WriteCharToBuffer( ch );
    return ch;
}


char _getkey ()
{
    char ch;

    do {
        CONSOLE_Update();
        ch= CONSOLE_GetCharFromBuffer();
    } while (ch == CONSOLE_NO_CHAR);
    return ch;
}


/**
 * Checks if a keyboard key has been pressed but not yet read.
 * @return the number of buffered characters.
 * @retval 0 if no any key is pressed.
 */
int kbhit()
{
    CONSOLE_Update();
    return _inWaitingIndex - _inReadIndex;
}


/**
 *  This (generic) version uses T1 for baud rate generation.
 */
void CONSOLE_Init( uint16_t baud )
{
    PCON &= 0x7F;   // Set SMOD bit to 0 (don't double baud rates)

    //  Receiver enabled.
    //  8-bit data, 1 start bit, 1 stop bit,
    //  Variable baud rate (asynchronous)
    //  Receive flag will only be set if a valid stop bit is received
    //  Set TI (transmit buffer is empty)
    SCON = 0x72;

    TMOD |= 0x20;   // T1 in mode 2, 8-bit auto reload

    TH1 = (256 - (uint8_t)((((uint32_t)OSC_FREQ / 100) * 3125)
        / ((uint32_t) baud * OSC_PER_INST * 1000)));

    TL1 = TH1;
    TR1 = 1;  // Run the timer
    TI = 1;   // Send first character (dummy)

    // Set up the buffers for reading and writing
    _inReadIndex = 0;
    _inWaitingIndex = 0;
    _outWrittenIndex = 0;
    _outWaitingIndex = 0;

    printf("Serial OK\n");

    // Interrupt *NOT* enabled
    ES = 0;
}


/**
 *  Checks for character in the UART (hardware) receive buffer
 *  Sends next character from the software transmit buffer
 */
void CONSOLE_Update()
{
    // Deal with transmit bytes here
    // Is there any data ready to send?
    if (_outWrittenIndex < _outWaitingIndex) {
        if (_tranBuffer[_outWrittenIndex] == '\n') //JYK
            CONSOLE_SendChar('\r');
        CONSOLE_SendChar(_tranBuffer[_outWrittenIndex]);

        _outWrittenIndex++;
    }
    else {
        // No data to send - just reset the buffer index
        _outWaitingIndex = 0;
        _outWrittenIndex = 0;
    }

    // Is data ready to be read into the received buffer?
    if (RI == 1)
        CONSOLE_ReceiveChar();
}


/**
 * Stores a character in the 'write' buffer, ready for
 * later transmission.
 */
static void CONSOLE_WriteCharToBuffer( char ch )
{
    // Write to the buffer *only* if there is space
    if (_outWaitingIndex < TRAN_BUFFER_LENGTH) {
        _tranBuffer[_outWaitingIndex] = ch;
        _outWaitingIndex++;
    }
    else {
        // Write buffer is full
        // Increase the size of TRAN_BUFFER_LENGTH
        // or increase the rate at which UART 'update' function is called
        // or reduce the amount of data sent to PC
        gErrorCode = EC_USART_WRITE_CHAR;
    }
}


/**
 *  Retrieves a character from the (software) buffer, if available.
 *
 *  The character from the buffer is returned, or - if no
 *  data are available - CONSOLE_NO_CHAR is returned.
 */
static char CONSOLE_GetCharFromBuffer()
{
    char ch = CONSOLE_NO_CHAR;

    // If there is new data in the buffer
    if (_inReadIndex < _inWaitingIndex) {
        ch = _recvBuffer[_inReadIndex];

        if (_inReadIndex < RECV_BUFFER_LENGTH) {
            _inReadIndex++;
        }
    }

    return ch;
}


/**
 * Based on Keil sample code, with added (loop) timeouts.
 * Implements Xon / Off control.
 *
 * Uses on-chip UART hardware.
 */
static void CONSOLE_SendChar( char ch )
{
    uint32_t timeout1;
    uint32_t timeout2;

    // for softeare XO/XOFF control
    if (RI) {
        if (SBUF == XOFF) {
            timeout2 = 1;

            do {
                RI = 0;

                // Wait for USART (with simple timeout)
                timeout1= 1;
                while ((RI == 0) && (++timeout1))
                    ; // dummy

                if (timeout1 == 0) {
                    // USART did not respond - error
                    gErrorCode = EC_USART_TI;
                    return;
                }
            } while ((SBUF != XON) && (++timeout2));

            if (timeout2 == 0) {
                // uart did not respond - error
                gErrorCode = EC_USART_TI;
                return;
            }

            RI = 0;
        }
    }

    // the core part

    timeout1= 1;
    while ((TI == 0) && (++timeout1))
        ; // dummy

    if (timeout1 == 0) {
        // USART did not respond - error
        gErrorCode = EC_USART_TI;
        return;
    }

    TI = 0;
    SBUF = ch;
}


static void CONSOLE_ReceiveChar()
{
    // Want to read into index 0, if old data has been read
    // (simple ~circular buffer)
    if (_inWaitingIndex == _inReadIndex) {
        _inWaitingIndex = 0;
        _inReadIndex = 0;
    }

    // Read the data from USART buffer
    _recvBuffer[_inWaitingIndex] = SBUF;

    if (_inWaitingIndex < RECV_BUFFER_LENGTH)
        _inWaitingIndex++; // Increment without overflowing buffer

    RI = 0;  // Clear RT flag
}
