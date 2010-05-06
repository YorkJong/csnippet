/**
 * @file ToyUnit_test.c
 *      An example to show how to use the \em ToyUnit testing framework
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @date 2005/3/9
 * @see ToyUnit.h
 */
#include "ToyUnit.h"

int main()
{
    TU_ASSERT("test 1", 1==1);
    TU_ASSERT("test 2", 1+1!=2); // test fail
    TU_ASSERT("test 3", 1+1==2);
    TU_ASSERT("test 4", 1*1==1);
    TU_ASSERT("test 5", 1-1==0);
    TU_ASSERT("test 6", 2==2);
    TU_ASSERT("test 7", 2+1==3);
    TU_ASSERT("test 8", 1==1);
    TU_RESULT(); // Tests [Pass-Fail]: [7-1]

    return 0;
}
