/**
 * @file conio.c
 *      Console versions of common I/O functions.
 * @note Uses the USART, and Pins 3.1 (Tx) and 3.0 (Rx).
 * @note CON_Init(), CON_Update(), CON_SendChar()
        should be modified for porting to other platform.
 * @see PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont, Ch18
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.1
 * @date 2006/04/28 (initial version)
 * @date 2006/05/18 (last revision)
 */
#include "main.h"
#include "conio.h"
#include <stdio.h>
#include "Queue.h"


extern uint8_t gErrorCode;


static void CON_SendChar( char );


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


/** Low level output interface of Keil C51's Standard I/O. */
char putchar ( char ch )
{
    if (!Q_full(&_tranQueue))
		Q_put(&_tranQueue, ch);
    else
        gErrorCode = EC_USART_WRITE_CHAR;
    return ch;
}


/** Low level input interface of Keil C51's Standard I/O. */
char _getkey()
{
    char ch;

    do {
        //CON_Update();
        if (!Q_empty(&_recvQueue))
            ch= Q_get(&_recvQueue);
    } while (ch == CON_NO_CHAR);
    return ch;
}


/** Checks if a keyboard key has been pressed but not yet read.
 * @return the number of buffered characters.
 * @retval 0 if no any key is pressed.
 */
int kbhit()
{
    //CON_Update();
    return Q_size( &_recvQueue );
}


void CON_Init( uint16_t baud )
{
    static uint8_t recvBuf[RECV_BUFFER_LENGTH];
    static uint8_t tranBuf[TRAN_BUFFER_LENGTH];

    uint16_t SREL;

    PCON &= 0x7F;   // Set SMOD bit to 0 (don't double baud rates)

    //  Receiver enabled.
    //  8-bit data, 1 start bit, 1 stop bit, 
    //  Variable baud rate (asynchronous)
    //  Receive flag will only be set if a valid stop bit is received
    //  Set TI (transmit buffer is empty)
    SCON = 0x72;

    // Use the internal baudrate generator (80c5x5 family)
    ADCON0 |= 0x80;

    // Set the baud rate (begin)
	SREL = 1024 - (OSC_FREQ + 32ul*baud/2) 
				/ (32ul*baud);
		
   	SRELL = (uint8_t)(SREL & 0x00FF);
   	SRELH = (uint8_t)((SREL & 0x0300) >> 8);

   	TI = 1;
  	// Set the baud rate (end)

    // Initializes queues

    Q_init( &_recvQueue, recvBuf, RECV_BUFFER_LENGTH );
    Q_init( &_tranQueue, tranBuf, TRAN_BUFFER_LENGTH );

    printf("Serial OK\n");

    // Serial interrupt *NOT* enabled
    ES = 0;
}


/** Updates the console. This involve with two parts:
 *      - Checks for character in the UART (hardware) receive buffer.
 *      - Sends next character from the software transmit buffer.
 */
void CON_Update()
{
    if (!Q_empty(&_tranQueue)) {
        if (Q_first(&_tranQueue) == '\n')
            CON_SendChar('\r');
        CON_SendChar(Q_get(&_tranQueue));
    }

    // Is data ready to be read into the received buffer?
    if (RI == 1) {
        // Read the data from USART buffer
        if (!Q_full(&_recvQueue))
        	Q_put(&_recvQueue, SBUF);
        RI = 0;  // Clear RT flag
    }
}


/** Sends a character via UART.
 *  - This is based on Keil sample code, with added (loop) timeouts.
 *  - Implements Xon / Off control.
 *  - Uses on-chip UART hardware.
 * @param ch the sent character
 */
static void CON_SendChar( char ch )
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
