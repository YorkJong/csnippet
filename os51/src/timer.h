/**
 * @file timer.h
 *      for timer relative operations, such as delay, timeout.
 * @author Jiang Yu-Kuan, york_jiang@mars-semi.com.tw
 * @version 1.0
 * @date 2007/11/16 (initial version)
 */
#ifndef _TIMER_H
#define _TIMER_H


void Delay( uint16_t ms );

void TimeReset( uint16_t ms );
void TimeResetHalted( uint16_t ms );
bool TimeOut();

void Tick_init();
uint32_t Time();


#endif