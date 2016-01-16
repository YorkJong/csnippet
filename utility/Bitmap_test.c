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

/// Returns the number of elements of an array
#define ElemsOfArray(x) (sizeof(x) / sizeof(x[0]))


int main()
{
    Bitmap b1, b2;
    Byte a1[BITMAP_NSLOTS(8*3)]= {0x00, 0x00, 0x00};
    Byte a2[BITMAP_NSLOTS(8*3)];

    Bitmap_init(&b1, a1, ElemsOfArray(a1));
    TU_ASSERT("t0-1", a1[0]==0x00);
    TU_ASSERT("t0-2", Bitmap_totalBits(&b1) == 8*3);
    TU_ASSERT("t0-3", Bitmap_totalBytes(&b1)==3);
    TU_ASSERT("t0-4", Bitmap_byteArray(&b1)==(uint8_t*)a1);

    Bitmap_setByteBits(&b1, 0);  // {0xff, 0x00, 0x00}
    Bitmap_setByteBits(&b1, 1);  // {0xff, 0xff, 0x00}
    TU_ASSERT("t1-1", a1[0]==0xff);
    TU_ASSERT("t1-2", a1[1]==0xff);

    Bitmap_clrByteBits(&b1, 0);  // {0x00, 0xff, 0x00}
    TU_ASSERT("t2-1", a1[0]==0x00);

    Bitmap_setBit(&b1, 0);       // {0x01, 0xff, 0x00}
    TU_ASSERT("t3-1", a1[0]==0x01);

    Bitmap_setBit(&b1, 16);      // {0x01, 0xff, 0x01}
    TU_ASSERT("t3-2", a1[2]==0x01);

    Bitmap_clrBit(&b1, 15);      // {0x01, 0x7f, 0x01}
    TU_ASSERT("t4-1", a1[1]==0x7f);
    TU_ASSERT("t5-1", Bitmap_getBit(&b1, 15)==0);
    TU_ASSERT("t5-2", Bitmap_getBit(&b1, 16)==1);
    Bitmap_clrBit(&b1, 16);      // {0x01, 0x7f, 0x00}
    Bitmap_setBit(&b1, 17);      // {0x01, 0x7f, 0x02}

    TU_ASSERT("t6-1", Bitmap_risenBitCount(&b1, 24)==9);
    TU_ASSERT("t6-2", Bitmap_risenBitCount(&b1, 10)==3);
    TU_ASSERT("t6-3", Bitmap_risenBitCount(&b1, 17)==8);
    TU_ASSERT("t7-1", Bitmap_sunkBitCount(&b1, 19)==10);

    TU_ASSERT("t8-1", Bitmap_findRisenBit(&b1, 8, 24)==8);
    Bitmap_clrByteBits(&b1, 1);  // {0x01, 0x00, 0x02}
    Bitmap_clrBit(&b1, 17);      // {0x01, 0x00, 0x00}
    TU_ASSERT("t8-2", Bitmap_findRisenBit(&b1, 1, 24)==24);
    Bitmap_setBit(&b1, 19);      // {0x01, 0x00, 0x08}
    TU_ASSERT("t8-3", Bitmap_findRisenBit(&b1, 1, 17)==17);
    TU_ASSERT("t8-4", Bitmap_findRisenBit(&b1, 1, 20)==19);
    Bitmap_setBit(&b1, 9);       // {0x01, 0x02, 0x08}
    TU_ASSERT("t8-5", Bitmap_findRisenBit(&b1, 1, 17)==9);
    TU_ASSERT("t8-6", Bitmap_findRisenBit(&b1, 9, 24)==9);
    TU_ASSERT("t8-7", Bitmap_findRisenBit(&b1, 18, 24)==19);

    Bitmap_clrBit(&b1, 9);       // {0x01, 0x00, 0x08}
    Bitmap_clrByteBits(&b1, 2);  // {0x01, 0x00, 0x00}
    TU_ASSERT("t9-1", Bitmap_findRisenBitRingedly(&b1, 1, 15)==0);
    TU_ASSERT("t9-2", Bitmap_findRisenBitRingedly(&b1, 1, 24)==0);
    Bitmap_clrByteBits(&b1, 0);  // {0x00, 0x00, 0x00}
    TU_ASSERT("t9-3", Bitmap_findRisenBitRingedly(&b1, 9, 24)==24);
    Bitmap_setBit(&b1, 8);       // {0x00, 0x01, 0x00}
    TU_ASSERT("t9-4", Bitmap_findRisenBitRingedly(&b1, 9, 24)==8);
    Bitmap_setBit(&b1, 0);       // {0x01, 0x01, 0x00}
    TU_ASSERT("t9-5", Bitmap_findRisenBitRingedly(&b1, 0, 24)==0);
    Bitmap_clrBit(&b1, 0);       // {0x00, 0x01, 0x00}
    Bitmap_setByteBits(&b1, 1);  // {0x00, 0xff, 0x00}
    TU_ASSERT("t10-1", Bitmap_findRisenByte(&b1, 0, 3)==1);
    TU_ASSERT("t11-1", Bitmap_findRisenByteRingedly(&b1, 2, 3)==1);
    Bitmap_clrByteBits(&b1, 1);  // {0x00, 0x00, 0x00}
    TU_ASSERT("t11-2", Bitmap_findRisenByteRingedly(&b1, 2, 3)==3);
    Bitmap_setByteBits(&b1, 0);  // {0xff, 0x00, 0x00}
    TU_ASSERT("t11-3", Bitmap_findRisenByteRingedly(&b1, 0, 3)==0);

    Bitmap_init(&b2, a2, ElemsOfArray(a2));
    Bitmap_copyAllBits(&b1, &b2);
    TU_ASSERT("t12-1", a2[0]==b1.a[0]);
    TU_ASSERT("t12-2", a2[1]==b1.a[1]);
    TU_ASSERT("t12-3", a2[2]==b1.a[2]);

    a2[2] = 0xFF;
    Bitmap_clearAllBits(&b2);
    TU_ASSERT("t13-1", a2[0]==0);
    TU_ASSERT("t13-2", a2[1]==0);
    TU_ASSERT("t13-3", a2[2]==0);


    TU_RESULT();

    return 0;
}
