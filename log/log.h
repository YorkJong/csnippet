/**
 * @file log.h
 *      a Log System implemented with only C macros
 * @author Jiang Yu-Kuan <yukuan.jiang(at)gmail.com>
 * @date 2010/05/05
 * @version 1.0
 */
#ifndef _LOG_H
#define _LOG_H


#include <stdio.h>


//-----------------------------------------------------------------------------

#define LL_TRACE 4
#define LL_DEBUG 3
#define LL_INFO  2
#define LL_ERROR 1
#define LL_OFF   0

#if !defined(LOG_LEVEL)
    #define LOG_LEVEL   LL_INFO
#endif

#define LOG(tag, args)  \
    do {                \
        printf(tag);    \
        printf args;    \
        printf("\n");   \
    } while (0)

//-----------------------------------------------------------------------------

#if LOG_LEVEL >= LL_TRACE
    #define TRC(args)  LOG("[TRC] ", args)
#else
    #define TRC(args)
#endif

#if LOG_LEVEL >= LL_DEBUG
    #define DBG(args)  LOG("[DBG] ", args)
#else
    #define DBG(args)
#endif

#if LOG_LEVEL >= LL_INFO
    #define INFO(args)  LOG("[INF] ", args)
#else
    #define INFO(args)
#endif

#if LOG_LEVEL >= LL_ERROR
    #define ERR(args)  LOG("[ERR] ", args)
#else
    #define ERR(args)
#endif

//-----------------------------------------------------------------------------

#if LOG_LEVEL >= LL_TRACE
    #define TraceCode(statements)   {statements}
#else
    #define TraceCode(statements)
#endif

#if LOG_LEVEL >= LL_DEBUG
    #define DebugCode(statements)   {statements}
#else
    #define DebugCode(statements)
#endif

//-----------------------------------------------------------------------------


#endif
