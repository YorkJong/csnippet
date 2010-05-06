/**
 * @file conio.c
 *      Console versions of common I/O functions.
 * @note Uses the USART, and Pins 3.1 (Tx) and 3.0 (Rx).
 * @note CONSOLE_Init(), CONSOLE_Update(), CONSOLE_SendChar()
        should be modified for porting to other platform.
 * @see PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont, Ch18
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28
 */
#include "main.h"
#include "conio.h"
#include <stdio.h>
#include "Queue.h"


extern uint8_t gErrorCode;


static void CONSOLE_WriteCharToBuffer( char );
static char CONSOLE_GetCharFromBuffer();

static void CONSOLE_SendChar( char );


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
static Queue _recvQueue;
static Queue _tranQueue;
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
    return Q_size( &_recvQueue );
}


/**
 *  This (generic) version uses T1 for baud rate generation.
 */
void CONSOLE_Init( uint16_t baud )
{
    static uint8_t recvBuf[RECV_BUFFER_LENGTH];
    static uint8_t tranBuf[TRAN_BUFFER_LENGTH];

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

    // Initializes queues

    Q_init( &_recvQueue, recvBuf, RECV_BUFFER_LENGTH );
    Q_init( &_tranQueue, tranBuf, TRAN_BUFFER_LENGTH );

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
    if (!Q_empty(&_tranQueue)) {
        if (Q_peek(&_tranQueue) == '\n')
            CONSOLE_SendChar('\r');
        CONSOLE_SendChar(Q_get(&_tranQueue));
    }

    // Is data ready to be read into the received buffer?
    if (RI == 1) {
        // Read the data from USART buffer
        if (!Q_full(&_recvQueue))
        	Q_add(&_recvQueue, SBUF);
        RI = 0;  // Clear RT flag
    }
}


/**
 * Stores a character in the 'write' buffer, ready for
 * later transmission.
 */
static void CONSOLE_WriteCharToBuffer( char ch )
{
    if (!Q_full(&_tranQueue))
		Q_add(&_tranQueue, ch);
    else
        gErrorCode = EC_USART_WRITE_CHAR;
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

    if (!Q_empty(&_recvQueue))
        ch= Q_get(&_recvQueue);

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
