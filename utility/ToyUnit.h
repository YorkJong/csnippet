/**
 * @file ToyUnit.h
 *      \em ToyUnit -- A toy unit testing framework for C Language.
 * @attention \em ToyUnit is designed for non-file-system environment,
 *      e.g. the Keil C51. Hence, \em buffer \em flushing (for stdout/stderr)
 *      is not necessary upon the implementing of \c TU_ASSERT()
 *      and \c TU_RESULT().
 * @warning It is necessary to call flush() while applying
 *      \em ToyUnit with \em buffer I/O platform.
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @date 2005/3/9
 * @version 1.1
 * @see MinUnit (http://www.jera.com/techinfo/jtns/jtn002.html)
 * @see ToyUnit_test.c
 * @todo to extend the framework to fit the more general environment
 */
#ifndef _TOY_UNIT_H_
#define _TOY_UNIT_H_


#if !defined( NDEBUG )

    #include <stdio.h>

    static unsigned int _TU_PASS_RUN= 0; ///< the number of pass test runs
    static unsigned int _TU_FAIL_RUN= 0; ///< the number of fail test runs

    /** Asserts that the \a assertion is true.
     * @param msg a message that is shown if asserting fail
     * @param assertion a boolean expression
     */
    #define TU_ASSERT( msg, assertion ) \
        do {                            \
            if (assertion) {            \
                putchar('.');           \
                _TU_PASS_RUN++;         \
            }                           \
            else {                      \
                puts("\n" msg " [test fail]");\
                _TU_FAIL_RUN++;         \
            }                           \
        } while (0)

    /** Prints the statistics of pass runs and fail runs. */
    #define TU_RESULT() \
        printf("\nTests [Pass-Fail]: [%u-%u]", _TU_PASS_RUN, _TU_FAIL_RUN)

#else

    #define TU_ASSERT( msg, assertion ) ((void)0)
    #define TU_RESULT() ((void)0)

#endif // NDEBUG


#endif // _TOY_UNIT_H_

/** @example ToyUnit_test.c
 *      This is an example of how to use the ToyUnit testing framework.
 */
