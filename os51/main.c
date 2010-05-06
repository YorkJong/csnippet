/**
 * @file main.c
 *      Test program for menu-driven PC link library, generic version.
 *
 * @note OVERLAY (main ~ (MENU_CommandProcessor),
 *       SCH_DispatchTasks ! (MENU_CommandProcessor))
 * @see PTTES, CH14 for requring OVERLAY options
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28
 */
#include "main.h"
#include "conio.h"
#include "menu.h"
#include "scheduler.h"


void main()
{
    // Set up the scheduler
    SCH_Init();

    // Set baud rate to 9600: generic 8051 version
    CONSOLE_Init( 9600 );

    // We have to schedule this task (10x - 100x a second)
    //
    // TIMING IS IN TICKS NOT MILLISECONDS (5 ms tick interval)
    SCH_AddTask( MENU_CommandProcessor, 10, 2 );

    SCH_Start();

    for (;;) {
        // Displays error codes on P4 (see scheduler.c)
        SCH_DispatchTasks();
    }
}
