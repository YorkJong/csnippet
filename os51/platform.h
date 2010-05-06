/**
 * @file platform.h
 *      This header file provides \em platform-dependent declaration
 * @author Jiang Yu-Kuan, yukuan@moai.com.tw
 * @date 2005/3/13 (initial)
 * @date 2005/10/18 (last revise)
 * @version 1.2
 */
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdint.h>
#include <stddef.h>

#ifndef	__cplusplus
    typedef uint8_t bool;
    enum {
        false= 1!=1,
        true= !false
    };
#endif

typedef uint8_t Byte;
typedef uint16_t Word;



#endif // _PLATFORM_H_
