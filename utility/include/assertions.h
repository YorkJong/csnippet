/**
 * @file assertions.h
 *      assertion macros
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2013/03/15 (initial)
 * @date 2016/01/16 (last revise)
 * @version 1.0
 */
#ifndef _ASSERTIONS_H
#define _ASSERTIONS_H

#include <stdint.h>
#include <stdio.h>


//------------------------------------------------------------------------------

/// Halt the program running.
#ifdef NOHALT_ASSERT
    #define _HALT()
#else
    #define _HALT() while (1)
#endif

/// A replacement of assert macro of C standard.
#ifndef NDEBUG
    #define assert(expr)                        \
        if (!(expr)) {                          \
            printf("[ERR] Assert failed: ");    \
            printf("%s", #expr);                \
            printf(" (file %s line %d)\n", __FILE__, (int)__LINE__ ); \
            _HALT();                            \
        }
#else
    #define	assert(expr)
#endif

//------------------------------------------------------------------------------

/// Asserts that "a op b" satisfied
#define ASSERT_OP(a, op, b)                     \
    if (!(a op b)) {                            \
        printf("[ERR] Assert failed: ");        \
        printf("%s %s %s; %s:%u, %s:%u",        \
                #a, #op, #b, #a, (uint32_t)(a), #b, (uint32_t)(b)); \
        printf(" (file %s line %d)\n",  __FILE__, (int)__LINE__ );  \
        _HALT();                                \
    }

/// Asserts that a > b
#define ASSERT_GT(a, b) ASSERT_OP(a, >, b)

/// Asserts that a >= b
#define ASSERT_GE(a, b) ASSERT_OP(a, >=, b)

/// Asserts that a < b
#define ASSERT_LT(a, b) ASSERT_OP(a, <, b)

/// Asserts that a <= b
#define ASSERT_LE(a, b) ASSERT_OP(a, <=, b)

/// Asserts that a == b
#define ASSERT_EQ(a, b) ASSERT_OP(a, ==, b)

/// Asserts that a != b
#define ASSERT_NE(a, b) ASSERT_OP(a, !=, b)

//------------------------------------------------------------------------------

/// macro for compile-time assertions
#define cassert(exp) typedef char __cassert[(exp)?1:-1]
//#define cassert(exp) extern char _cassert[exp]

//------------------------------------------------------------------------------


#endif

