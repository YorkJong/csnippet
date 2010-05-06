/**
 * @file timer.c
 *      for timer relative operations, such as delay, timeout.
 * @author Jiang Yu-Kuan, york_jiang@mars-semi.com.tw
 * @version 1.0
 * @date 2007/11/16 (initial version)
 */
#include "main.h"
#include "timer.h"


// Timer preload values for use in simple (hardware) delays
// - Timers are 16-bit, manual reload ('one shot').
//
// NOTE: These values are portable but timings are *approximate*
//       and *must* be checked by hand if accurate timing is required.
//
// Define Timer 0 / Timer 1 reload values for ~1 msec delay
#define RELOAD_1ms  (65536 - (OSC_FREQ/1000 + OSC_PER_INST/2) / OSC_PER_INST)    
#define RELOAD_1ms_H (RELOAD_1ms / 256)
#define RELOAD_1ms_L (RELOAD_1ms % 256)


//---------------------------------------------------------------------------
// For purly delay function
//---------------------------------------------------------------------------

/**
 * Generates n milliscond delay. (approx)
 * Uses Timer 0 (easily adapted to Timer 1)
 */
void Delay( uint16_t n )
{
    uint16_t ms = n;
   
    // Configure Timer 0 as a 16-bit timer 
    TMOD &= 0xF0; // Clear all T0 bits (T1 left unchanged)
    TMOD |= 0x01; // Set required T0 bits (T1 left unchanged) 

    ET0 = 0;  // No interupts

    // Delay value is *approximately* 1 ms per loop   
    while (ms--) {                 
        TH0 = RELOAD_1ms_H;  
        TL0 = RELOAD_1ms_L;   

        TF0 = 0;        // clear overflow flag
        TR0 = 1;        // start timer 0

        while (!TF0);   // Loop until Timer 0 overflows (TF0 == 1)

        TR0 = 0;        // Stop Timer 0
    }
}


//---------------------------------------------------------------------------
// For the timeout mechanism in a busy-waiting loop
//---------------------------------------------------------------------------

static uint16_t _ms = 0;


static void T0_reload()
{
    TH0 = RELOAD_1ms_H;  
    TL0 = RELOAD_1ms_L; 
    TR0 = 1;    // start Timer 0
}


/** Sets time out after n milliseconds */
void TimeReset( uint16_t n )
{
    _ms = n;

    TMOD &= 0xF0; // Clear all T0 bits (T1 left unchanged)
    TMOD |= 0x01; // Set required T0 bits (T1 left unchanged) 

    ET0 = 0;    // No interupts

    TR0 = 0;    // stop Timer 0
    T0_reload();
    TF0 = 0;
}


/** Sets time out after n milliseconds 
 *  NOTE: The timer to go begins when TimeOut() is called. */
void TimeResetHalted( uint16_t n )
{
    _ms = n+1;

    TMOD &= 0xF0; // Clear all T0 bits (T1 left unchanged)
    TMOD |= 0x01; // Set required T0 bits (T1 left unchanged) 

    ET0 = 0;    // No interupts

    TR0 = 0;    // stop Timer 0
    TF0 = 1;
}


/** Checks if the time is out. */
bool TimeOut()
{
    if (_ms == 0)
        return true;

    if (TF0) {
        T0_reload();
        TF0 = 0;    // clear overflow flag
        _ms--;
    }
    return false;
}


//---------------------------------------------------------------------------
// For 1ms interval ticks function
//---------------------------------------------------------------------------

static uint32_t _ticks; // clock ticks in ms


uint32_t Time()
{
    return _ticks;
}


static void T0Reload()
{
    TR0 = 0; // Stop Timer 1
    TH0 = RELOAD_1ms_H;  
    TL0 = RELOAD_1ms_L; 
    TR0 = 1; // Start Timer 1
}


static void Tick_Update(void) interrupt INTERRUPT_Timer_0_Overflow
{
    T0Reload(); // Reload the timer
    ++_ticks;
}


void Tick_init()
{
    _ticks = 0;

    TMOD &= 0xF0; // Clear all T0 bits (T1 left unchanged)
    TMOD |= 0x01; // Set required T0 bits (T1 left unchanged) 

    T0Reload(); // Sets up timer reload values

    ET0 = 1; // Interrupt Timer 1 enabled
}

//---------------------------------------------------------------------------
