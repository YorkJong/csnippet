/**
 * @file main.c
 *      Test program for menu-driven PC link library, generic version.
 *
 * @note OVERLAY (main ~ (MENU_CommandProcessor),
 *       SCH_DispatchTasks ! (MENU_CommandProcessor))
 * @see PTTES, CH14 for requring OVERLAY options
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28 (initial version)
 * @date 2006/5/18 (last revision)
 */
#include "main.h"
#include "conio.h"
#include "menu.h"
#include "scheduler.h"
#include "timer.h"


static void TestTimer();


/// TIMING IS IN TICKS NOT MILLISECONDS
void main()
{
    TestTimer(); // Using Timer 0
    Tick_init(); // Using Timer 0 generates a 1 ms inverval ticks.

    SCH_Init(5); // Set up the scheduler (5 ms tick interval)

    CON_Init( 9600 ); // Set baud rate to 9600: generic version.

    SCH_AddTask( CON_Update, 5, 2 );

    // Must schedule this task 10x ~ 100x a second
    SCH_AddTask( MENU_CommandProcessor, 10, 2 );

    SCH_Start();

    for (;;) {
        // Displays error codes on P4 (see scheduler.c)
        SCH_DispatchTasks();
    }
}


//---------------------------------------------------------------------------


static void Polling()
{
    while (!TimeOut());
}


static void TestTimer()
{
    uint8_t i;
    for (i=0; i<100; ++i) {
        TimeReset(1);
        //TimeResetHalted(1);
        Polling();
        Delay(1);
    }
}
