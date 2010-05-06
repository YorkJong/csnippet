/**
 * @file scheduler.h
 *
 * @see PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont, Ch14
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28 (initial version)
 * @date 2006/5/18 (last revision)
 */

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "main.h"


/**
 * Data structure of Task.
 */
typedef struct {
    /// Pointer to the task function
    void (*routine)();

    /// Interval (ticks) between subsequent runs.
    uint16_t period;

    /// To count down ticks until the function will (next) be run.
    uint16_t countdown;

    /// To determine if a task is due to execute
    bool due;
} Task;


enum {
    /// The maximum number of tasks required at any one time
    ///  during the execution of the program.
    /// @warning MUST BE ADJUSTED FOR EACH NEW PROJECT.
    SCH_MAX_TASKS= 2
};


/** Status for the scheduler module */
typedef enum {
    SS_NORMAL,
    SS_ERROR
} SCH_Status;


///@name Core scheduler functions
//@{
void SCH_Init( uint8_t ms );
void SCH_Start();
void SCH_DispatchTasks();
uint8_t SCH_AddTask( const void (*fn)(), uint16_t delay, uint16_t period );
SCH_Status SCH_DeleteTask( uint8_t idx );
void SCH_GoToSleep();
void SCH_ReportStatus();
//@}

#endif
