/**
 * @file menu.c
 *      Simple framework for interactive operation (e.g. data downloads)
 *      to / from desktop / notebook PC (etc) via serial (UART) link.
 *
 *      Use 'Hyperterminal' (under Windows 95, 98, 2000) or similar
 *      terminal emulator program on other operating systems.
 *
 *      Terminal options:
 *      - Data bits    = 8
 *      - Parity       = None
 *      - Stop bits    = 1
 *      - Flow control = Xon / Xoff
 *
 * @see PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont, Ch18
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28
 */
#include "main.h"
#include "menu.h"
#include "conio.h"
#include <stdio.h>
#include <ctype.h>

static void MENU_ShowMenu();
static void MENU_PerformTask(char);

static void FunctionA();
static void FunctionB();
static void FunctionC();


/**
 *  This function is the main menu 'command processor' function.
 *
 *  Schedule this (say) once every 10 ms (approx.).
 */
void MENU_CommandProcessor()
{
    static bool first_time= true;
    char ch= CONSOLE_NO_CHAR;

    if (first_time) {
        first_time= false;
        MENU_ShowMenu();
    }

    if (kbhit()) {
        ch= getchar();
        MENU_PerformTask(ch);
        MENU_ShowMenu();
    }
}


/**
 * Display menu options on PC screen (via serial link)
 *  edit as required to meet the needs of your application.
 */
static void MENU_ShowMenu()
{
    printf("Menu:\n");
    printf("a - x\n");
    printf("b - x\n");
    printf("c - x\n\n");
    printf("? : ");
}


/**
 * Perform the required user task
 * edit as required to match the needs of your application.
 */
static void MENU_PerformTask( char ch )
{
    // Echo the menu option
    //putchar( ch ); //JYK

    putchar('\n');

    // Perform the task
    switch (toupper(ch)) {
        case 'A':
            FunctionA();
            break;

        case 'B':
            FunctionB();
            break;

        case 'C':
            FunctionC();
            break;

        default:
            ; // dummy
    }
}


/**
 * Placeholder function
 */
static void FunctionA()
{
    printf("\n* Doing A *\n\n");
}


/**
 * Placeholder function
 */
static void FunctionB()
{
    printf("\n* Doing B *\n\n");
}


/**
 * Placeholder function
 */
static void FunctionC()
{
    printf("\n* Doing C *\n\n");
}
