/**
 * @file debug.c
 *      functions implement for debuging
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @date 2005/09/13 (initial)
 * @date 2005/09/13 (last revise)
 */
#if !defined( NDEBUG )
    #include <stdio.h>
    /** Prints infomation for assertion failed, and
     *  is called by macro ASSERT(.).
     * @attention This function is designed for non-file-system environment,
     *      e.g. the Keil C51 platform. Hence, \em buffer \em flushing
     *      (for stdout/stderr) is not necessary at all.
     * @see Book "Writing Solid Code"
     *      (http://www.testing.com/writings/reviews/maguire-solid.html)
     */
    void _AssertionFailedPrintout( const char* fileName, unsigned line )
    {
        printf("Assertion failed: %s, line %u\n", fileName, line);
        while (1);
    }
#endif // NDEBUG
