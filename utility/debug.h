/**
 * @file debug.h
 *      Macros for debuging
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @date 2005/06/06 (initial)
 * @date 2005/09/13 (last revise)
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_


#include <stdio.h>

#if defined( DEBUG )
    /** Debug a code block */
    #define DebugCode( statament_list ) { statament_list }
#else
    /** Dummy macro */
    #define DebugCode( statament_list )
#endif


#endif // _DEBUG_H_
