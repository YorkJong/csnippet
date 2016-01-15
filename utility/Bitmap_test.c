/**
 * @file Bitmap_test.c
 *      Unit Test for Bitmap operations.
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2005/03/13 (initial)
 * @date 2016/01/15 (last revise)
 * @see Bitmap.h
 * @see Bitmap.c
 */
#include "ToyUnit.h"
#include "Bitmap.h"


int main()
{
    Byte a[BITMAP_NSLOTS(8*3)]= {0x00, 0x00, 0x00};

    SetByteBits(a, 0);  // {0xff, 0x00, 0x00}
    SetByteBits(a, 1);  // {0xff, 0xff, 0x00}
    TU_ASSERT("t1-1", a[0]==0xff);
    TU_ASSERT("t1-2", a[1]==0xff);

    ClrByteBits(a, 0);  // {0x00, 0xff, 0x00}
    TU_ASSERT("t2-1", a[0]==0x00);

    SetBit(a, 0);       // {0x01, 0xff, 0x00}
    TU_ASSERT("t3-1", a[0]==0x01);

    SetBit(a, 16);      // {0x01, 0xff, 0x01}
    TU_ASSERT("t3-2", a[2]==0x01);

    ClrBit(a, 15);      // {0x01, 0x7f, 0x01}
    TU_ASSERT("t4-1", a[1]==0x7f);
    TU_ASSERT("t5-1", GetBit(a, 15)==0);
    TU_ASSERT("t5-2", GetBit(a, 16)==1);
    ClrBit(a, 16);      // {0x01, 0x7f, 0x00}
    SetBit(a, 17);      // {0x01, 0x7f, 0x02}

    TU_ASSERT("t6-1", RisenBitCount(a, 24)==9);
    TU_ASSERT("t6-2", RisenBitCount(a, 10)==3);
    TU_ASSERT("t6-3", RisenBitCount(a, 17)==8);
    TU_ASSERT("t7-1", SunkBitCount(a, 19)==10);

    TU_ASSERT("t8-1", FindRisenBit(a, 8, 24)==8);
    ClrByteBits(a, 1);  // {0x01, 0x00, 0x02}
    ClrBit(a, 17);      // {0x01, 0x00, 0x00}
    TU_ASSERT("t8-2", FindRisenBit(a, 1, 24)==24);
    SetBit(a, 19);      // {0x01, 0x00, 0x08}
    TU_ASSERT("t8-3", FindRisenBit(a, 1, 17)==17);
    TU_ASSERT("t8-4", FindRisenBit(a, 1, 20)==19);
    SetBit(a, 9);       // {0x01, 0x02, 0x08}
    TU_ASSERT("t8-5", FindRisenBit(a, 1, 17)==9);
    TU_ASSERT("t8-6", FindRisenBit(a, 9, 24)==9);
    TU_ASSERT("t8-7", FindRisenBit(a, 18, 24)==19);

    ClrBit(a, 9);       // {0x01, 0x00, 0x08}
    ClrByteBits(a, 2);  // {0x01, 0x00, 0x00}
    TU_ASSERT("t9-1", FindRisenBitRingedly(a, 1, 15)==0);
    TU_ASSERT("t9-2", FindRisenBitRingedly(a, 1, 24)==0);
    ClrByteBits(a, 0);  // {0x00, 0x00, 0x00}
    TU_ASSERT("t9-3", FindRisenBitRingedly(a, 9, 24)==24);
    SetBit(a, 8);       // {0x00, 0x01, 0x00}
    TU_ASSERT("t9-4", FindRisenBitRingedly(a, 9, 24)==8);
    SetBit(a, 0);       // {0x01, 0x01, 0x00}
    TU_ASSERT("t9-5", FindRisenBitRingedly(a, 0, 24)==0);
    ClrBit(a, 0);       // {0x00, 0x01, 0x00}
    SetByteBits(a, 1);  // {0x00, 0xff, 0x00}
    TU_ASSERT("t10-1", FindRisenByte(a, 0, 3)==1);
    TU_ASSERT("t11-1", FindRisenByteRingedly(a, 2, 3)==1);
    ClrByteBits(a, 1);  // {0x00, 0x00, 0x00}
    TU_ASSERT("t11-2", FindRisenByteRingedly(a, 2, 3)==3);
    SetByteBits(a, 0);  // {0xff, 0x00, 0x00}
    TU_ASSERT("t11-3", FindRisenByteRingedly(a, 0, 3)==0);

    TU_RESULT();

    return 0;
}
