/**
 * @file main.h
 *
 * @note OVERLAY (main ~ (MENU_CommandProcessor),
 *       SCH_DispatchTasks ! (MENU_CommandProcessor))
 * @see PTTES, CH14 for requring OVERLAY options
 * @see PTTES, Ch9 for project header
 * @see project IO_T2_T1 in PTTES, CH18
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @version 2.0
 * @date 2006/4/28
 */

#ifndef _MAIN_H
#define _MAIN_H


#include "platform.h"


//------------------------------------------------------------------
// WILL NEED TO EDIT THIS SECTION FOR EVERY PROJECT
//------------------------------------------------------------------

// Must include the appropriate microcontroller header file here

#include <reg515c.h>

/** Include oscillator / chip details here
 * (essential if generic delays / timeouts are used)
 * Oscillator / resonator frequency (in Hz) e.g. (11059200UL)
 */
#define OSC_FREQ (10000000UL)

/** Number of oscillations per instruction (4, 6 or 12).
 *  - 12: Original 8051 / 8052 and numerous modern versions.
 *  -  6: Various Infineon and Philips devices, etc.
 *  -  4: Dallas, etc.
 *
 * Take care with Dallas devices.
 *  - Timers default to \em 12 osc ticks unless CKCON is modified.
 *  - If using generic code on a Dallas device, use 12 here.
 */
enum {
    OSC_PER_INST= 6
};


//------------------------------------------------------------------
// Interrupts
// - see Chapter 13.
//------------------------------------------------------------------

///@name Generic 8051/52 timer interrupts (used in most schedulers)
//@{
#define INTERRUPT_Timer_0_Overflow 1
#define INTERRUPT_Timer_1_Overflow 3
#define INTERRUPT_Timer_2_Overflow 5
//@}

///@name Additional interrupts (used in shared-clock schedulers)
//@{
//#define INTERRUPT_EXTERNAL_0 0
//#define INTERRUPT_EXTERNAL_1 2
//#define INTERRUPT_UART_Rx_Tx 4
//#define INTERRUPT_CAN_c515c 17
//@}

/**
 * Error codes
 * @see PTTES, Ch14.
 */
typedef enum {
    EC_NO_ANY_ERROR,
    EC_SCH_TOO_MANY_TASKS,
    EC_SCH_CANNOT_DELETE_TASK,

    //EC_SCH_WAITING_FOR_SLAVE_TO_ACK= 3,
    //EC_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER= 3,

    //EC_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START= 4,
    //EC_SCH_LOST_SLAVE= 5,

    //EC_SCH_CAN_BUS_ERROR= 6,

    //EC_I2C_WRITE_BYTE= 10,
    //EC_I2C_READ_BYTE= 11,
    //EC_I2C_WRITE_BYTE_AT24C64= 12,
    //EC_I2C_READ_BYTE_AT24C64= 13,
    //EC_I2C_DS1621= 14,

    EC_USART_TI= 21,
    EC_USART_WRITE_CHAR= 22,

    //EC_SPI_EXCHANGE_BYTES_TIMEOUT= 31,
    //EC_SPI_X25_TIMEOUT= 32,
    //EC_SPI_MAX1110_TIMEOUT= 33,

    //EC_ADC_MAX150_TIMEOUT= 44
} ErrorCode;

#endif

