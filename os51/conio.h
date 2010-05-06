/**
 * @file conio.h
 *      Console versions of common I/O functions.
 * @note Uses the USART, and Pins 3.1 (Tx) and 3.0 (Rx).
 * @see PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont, Ch18
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28
 */
#ifndef _CONIO_H
#define _CONIO_H


/** Value returned by PC_LINK_Get_Char_From_Buffer if no character is
 * available in buffer
 */
enum {
    CONSOLE_NO_CHAR= 127
};


void CONSOLE_Init( uint16_t );

/// Must schedule or otherwise regularly call this function...
void CONSOLE_Update();

int kbhit();
int ungetch( int ch );
int getch();
int getche();
int putch( int ch );
char* cgets( char* buf );
int cprintf( const char *format, const char* arg, ... );
int cputs( const char *string );
int cscanf( char *format, char* arg, ... );


#endif // _CONIO_H
