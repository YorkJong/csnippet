/**
 * @file scheduler.c
 *   Core scheduler functions
 *      - These functions may be used with all 8051 devices
 *      - Includes (optional) power-saving mode
 *      - You must ensure that the power-down mode is adapted
 *        to match your chosen device (or disabled altogether)
 *
 * @note SCH_MAX_TASKS \em must be set by the user
 * @note SCH_Init(), SCH_Start(), SCH_Update(), and SCH_GoToSleep()
 *      should be modified for porting to other platform.
 * @see scheduler.h
 * @see PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont, Ch14
 * @todo add a watchdog (see PTTES, p274)
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28 (initial version)
 * @date 2006/5/18 (last revision)
 */
#include "main.h"
#include "scheduler.h"


/// Used to display the error code
/// @see main.h for details of error codes
/// @see port.h for details of the error port
uint8_t gErrorCode= EC_NO_ANY_ERROR;


/// The array of tasks
static Task _SCH_tasks[SCH_MAX_TASKS];

/// Keeps track of time since last error was recorded (see below)
static uint16_t _errorTickCount;

/// The code of the last error (reset after ~1 minute)
static uint8_t _lastErrorCode;


static void SCH_T2_Init( uint8_t ms );
static void SCH_GoToSleep();


/**
 * Scheduler initialisation function.  Prepares scheduler
 *  data structures and sets up timer interrupts at required rate.
 *
 * @param ms ticks interval in milli-seconds
 * @note you must call this function before using the scheduler.
 */
void SCH_Init( uint8_t ms )
{
    uint8_t i;

    for (i = 0; i < SCH_MAX_TASKS; i++)
        SCH_DeleteTask(i);

    SCH_T2_Init( ms );
}


/**
 * Starts the scheduler, by enabling interrupts.
 *
 * @note Usually called after all regular tasks are added,
 *      to keep the tasks synchronised.
 * @note ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!!
 * @todo start the watchdog
 */
void SCH_Start()
{
    EA = 1;
}


/**
 * This is the scheduler ISR.  It is called at a rate
 * determined by the timer settings in the 'init' function.
 *
 * This version is triggered by Timer 2 interrupts:
 * timer is automatically reloaded.
 * @todo refresh the watchdog
 */
void SCH_Update(void) interrupt INTERRUPT_Timer_2_Overflow
{
    uint8_t i;

    TF2 = 0; // Have to manually clear this.

    // NOTE: calculations are in *TICKS* (not milliseconds)
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        // Check if there is a task at this location
        if (_SCH_tasks[i].routine != 0) {
            if (_SCH_tasks[i].countdown == 0) {
                // The task is due to run
                _SCH_tasks[i].due= true;

                // Schedule periodic tasks to run again
                _SCH_tasks[i].countdown = _SCH_tasks[i].period;
            }
            else {
                // Not yet ready to run: just decrement count down a tick
                _SCH_tasks[i].countdown--;
            }
        }
    }
}


/**
 *  This is the 'dispatcher' function.  When a task (function)
 *  is due to run, SCH_DispatchTasks() will run it.
 *  This function must be called (repeatedly) from the main loop.
 */
void SCH_DispatchTasks()
{
    uint8_t i;

    // Dispatches (runs) the next task (if one is ready)
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        if (_SCH_tasks[i].due) {
            (*_SCH_tasks[i].routine)();  // Run the task

            _SCH_tasks[i].due= false;

            // Periodic tasks will automatically run again:
            // if this is a 'one shot' task, remove it from the array.
            if (_SCH_tasks[i].period == 0) {
                SCH_DeleteTask(i);
            }
        }
    }

    // Report system status
    SCH_ReportStatus();

    // The scheduler enters idle mode at this point
    SCH_GoToSleep();
}


/**
 * Causes a task (function) to be executed at regular intervals
 *  or after a user-defined delay.
 *
 * @code
 *      Task_ID = SCH_AddTask(Do_X,1000,0);
 * @endcode
 *      Causes the function Do_X() to be executed once after 1000 sch ticks.
 *
 * @code
 *      Task_ID = SCH_AddTask(Do_X,0,1000);
 * @endcode
 *      Causes the function Do_X() to be executed regularly, every 1000 sch ticks.
 *
 * @code
 *      Task_ID = SCH_AddTask(Do_X,300,1000);
 * @endcode
 *      Causes the function Do_X() to be executed regularly, every 1000 ticks.
 *      Task will be first executed at T = 300 ticks, then 1300, 2300, etc.
 *
 * @param[in] fn  The name of the function which is to be scheduled.
 *      NOTE: All scheduled functions must be 'void, void' -
 *      that is, they must take no parameters, and have
 *      a void return type.
 * @param[in] delay  The interval (TICKS) before the task is first executed
 * @param[in] period If 'period' is 0, the function is only called once,
 *      at the time determined by 'delay'.  If period is non-zero,
 *      then the function is called repeatedly at an interval
 *      determined by the value of period (see below for examples
 *      which should help clarify this).
 * @return the position in the task array at which the task has been added.
 *      If the return value is SCH_MAX_TASKS then the task could
 *      not be added to the array (there was insufficient space).  If the
 *      return value is < SCH_MAX_TASKS, then the task was added successfully.
 * @note this return value may be required, if a task is
 *      to be subsequently deleted - see SCH_DeleteTask().
 */
uint8_t SCH_AddTask( const void (*fn)(), uint16_t delay, uint16_t period )
{
    uint8_t i = 0;

    // First find a gap in the array (if there is one)
    for (i= 0; i<SCH_MAX_TASKS; ++i) {
        if (_SCH_tasks[i].routine == 0)
            break;
    }

    // Have we reached the end of the list?
    if (i == SCH_MAX_TASKS) {
        // Task list is full.

        // Set the global error variable.
        gErrorCode = EC_SCH_TOO_MANY_TASKS;

        // Also return an error code
        return SCH_MAX_TASKS;
    }

    // If we're here, there is a space in the task array

    _SCH_tasks[i].routine= fn;
    _SCH_tasks[i].countdown= delay;
    _SCH_tasks[i].period= period;
    _SCH_tasks[i].due= false;

    return i; // return position of task (to allow later deletion)
}


/**
 * Removes a task from the scheduler.
 * @note delete the associated function from memory:
 *      it simply means that it is no longer called by the scheduler.
 * @param idx - The task index.  Provided by SCH_AddTask().
 * @retval SS_ERROR
 * @retval SS_NORMAL
 */
SCH_Status SCH_DeleteTask( uint8_t idx )
{
    SCH_Status status;

    if (_SCH_tasks[idx].routine == 0) {
        // No task at this location...

        // Set the global error variable
        gErrorCode = EC_SCH_CANNOT_DELETE_TASK;

        // ...also return an error code
        status = SS_ERROR;
    }
    else {
        status = SS_NORMAL;
    }

    _SCH_tasks[idx].routine= 0;
    _SCH_tasks[idx].countdown= 0;
    _SCH_tasks[idx].period= 0;
    _SCH_tasks[idx].due= false;

    return status;
}


/**
 * This scheduler enters \em idle \em mode between clock ticks
 * to save power. The next clock tick will return the processor
 * to the normal operating state.
 *
 * @note a slight performance improvement is possible if this
 *      function is implemented as a macro, or if the code here is simply
 *      pasted into the \em dispatch function.
 * \par
 *      However, by making this a function call, it becomes easier
 *      -- during development -- to assess the performance of the
 *      scheduler, using the 'performance analyser' in the Keil
 *      hardware simulator.
 * @see PTTES, Ch14
 * @warning May wish to disable this if using a watchdog.
 * @warning ADAPT AS REQUIRED FOR YOUR HARDWARE.
 */
void SCH_GoToSleep()
{
    PCON |= 0x01;    // Enter idle mode (generic 8051 version)

    // Entering idle mode requires TWO consecutive instructions
    // on 80c515 / 80c505 - to avoid accidental triggering
    //PCON |= 0x01;    // Enter idle mode (#1)
    //PCON |= 0x20;    // Enter idle mode (#2)
}


/**
 * Simple function to display error codes.
 *
 * This version displays code on a port with attached LEDs:
 * adapt, if required, to report errors over serial link, etc.
 *
 * Errors are only displayed for a limited period
 * (60000 ticks = 1 minute at 1ms tick interval).
 * After this the the error code is reset to 0.
 *
 * This code may be easily adapted to display the last
 * error 'for ever': this may be appropriate in your
 * application.
 *
 * @see Chapter 10 for further information.
 */
void SCH_ReportStatus()
{
#ifdef SCH_REPORT_ERRORS
    // ONLY APPLIES IF WE ARE REPORTING ERRORS
    // Check for a new error code
    if (gErrorCode != _lastErrorCode) {
        // Negative logic on LEDs assumed
        Error_port = 255 - gErrorCode;

        _lastErrorCode = gErrorCode;

        if (gErrorCode != 0) {
            _errorTickCount = 60000;
        }
        else {
            _errorTickCount = 0;
        }
    }
    else {
        if (_errorTickCount != 0) {
            if (--_errorTickCount == 0) {
                gErrorCode = 0; // Reset error code
            }
        }
    }
#endif
}

//------------------------------------------------------------------------------

static void SCH_T2_Init( uint8_t ms )
{
    uint16_t t2;

    // Reset the global error variable
    // - SCH_DeleteTask() will generate an error code,
    //   (because the task array is empty)
    gErrorCode = EC_NO_ANY_ERROR;

    // Now set up Timer 2
    // 16-bit timer function with automatic reload

    T2CON = 0x04;   // load Timer 2 control register
    T2MOD = 0x00;   // load Timer 2 mode register

    // Crystal is assumed to be 11.059 MHz
    // Assume 12 osc cycles per timer increment
    // -> timer resolution is 0.000001085069444444 seconds (1.085 us)
    // The required Timer 2 overflow is 0.005 seconds (5 ms)
    // -- this takes 4608 timer ticks
    // Reload value is 65536 - 4608 = 60928 (dec) = 0xEE00

    t2 = 65536 - (OSC_FREQ/1000*ms  + OSC_PER_INST/2) / OSC_PER_INST;
    TH2 = t2 >> 8;      // load timer 2 high byte
    TL2 = t2 & 0x00FF;  // load timer 2 low byte
    RCAP2H = TH2;  // load timer 2 reload capture reg, high byte
    RCAP2L = TL2;  // load timer 2 reload capture reg, low byte

    // Start timer
    ET2   = 1;  // Timer 2 interrupt is enabled
    TR2   = 1;  // Start Timer 2
}