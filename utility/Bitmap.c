/**
 * @file Bitmap.c
 *      This module provides \em bitmap operations for byte-array.
 *      The bitmap is implemented with byte-array, each byte keeps 8 bits.
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2005/03/11 (initial)
 * @date 2016/01/16 (last revise)
 * @version 2.0
 * @see Bitmap.h
 * @see Bitmap_test.c
 */
#include "assertions.h"

#include "platform.h"
#include "Bitmap.h"


/// Returns the slot index of a given bit index
#define BITSLOT(b)  ((b) / ELEM_BITS)

/// Returns the bit mask of a given bit index
#define BITMASK(b)  (1 << ((b) % ELEM_BITS))


//-----------------------------------------------------------------------------
// Bit-wise operators
//-----------------------------------------------------------------------------

/** Initializes the bitmap.
 * @param[out] b the bitmap
 * @param[in] a the pointer to an Elem array
 * @param[in] n the number of elements of the array
 */
void Bitmap_init(Bitmap* b, Elem a[], size_t n)
{
    b->a = a;
    b->n = n;
    b->p = 1;
}


/** Returns the total bits of the bitmap. */
size_t Bitmap_totalBits(const Bitmap* b)
{
    return ELEM_BITS * b->n;
}


/** Sets bit[i] to 1
 * @param[out] b the bitmap
 * @param[in] i the index of the \em bit to be set
 * @see Bitmap_clrBit()
 */
void Bitmap_setBit(Bitmap* b, Index i)
{
    ASSERT_OP (i, <, Bitmap_totalBits(b));

    b->a[BITSLOT(i)] |= BITMASK(i);
}


/** Clears bit[i] to 0
 * @param[out] b the bitmap
 * @param[in] i the index of the cleared \em bit
 * @see Bitmap_setBit()
 */
void Bitmap_clrBit(Bitmap* b, Index i)
{
    ASSERT_OP (i, <, Bitmap_totalBits(b));

    b->a[BITSLOT(i)] &= ~BITMASK(i);
}


/** Gets bit[i]
 * @param[in] b the bitmap
 * @param[in] i the index of the gotten bit
 */
Bit Bitmap_getBit(const Bitmap* b, Index i)
{
    ASSERT_OP (i, <, Bitmap_totalBits(b));

    return (b->a[BITSLOT(i)] & BITMASK(i))  !=  0;
}

//-----------------------------------------------------------------------------

/** Sets all bits to zero. */
void Bitmap_clearAllBits(Bitmap* b)
{
    Index i;
    for (i=0; i<b->n; ++i)
        b->a[i] = 0;
}


/** Copies bits from \a src to \a tgt. */
void Bitmap_copyAllBits(const Bitmap* src, Bitmap* tgt)
{
    Index i;

    ASSERT_OP (src->n, ==, tgt->n);

    for (i=0; i<src->n; ++i)
        tgt->a[i] = src->a[i];
}

//-----------------------------------------------------------------------------

/** Counts the total risen bit (value=1)
 *      in the range [\em 0, \a end) of a give bitmap.
 * @param b the bitmap
 * @param end the \em limit index of the counted bits (= \em last+1).
 * @return the count result
 * @see Bitmap_sunkBitCount()
 */
size_t Bitmap_risenBitCount(const Bitmap* b, Index end)
{
    Index i; // index of byte in the byte-array
    Index j; // index of bit in a byte
    const Index lastByteIdx = (end-1)/ELEM_BITS;
    size_t restBits = end;
    size_t result = 0;

    for (i=0; i<=lastByteIdx; i++) {
        if (b->a[i] != 0) {
            Byte byte = b->a[i];
            Byte bits = (restBits<ELEM_BITS) ? restBits : ELEM_BITS;
            for (j=0; j<bits; j++) {
                result += byte & 0x1;
                byte >>= 1;
            }
        }
        restBits -= ELEM_BITS;
    }
    return result;
}


/** Counts the total sunk bit (value=0)
 *      in the range [\em 0, \a end) of a give bitmap.
 * @param b the bitmap
 * @param end the \em limit index of the counted bits (= \em last+1).
 * @return the count result
 * @see Bitmap_risenBitCount()
 */
size_t Bitmap_sunkBitCount(const Bitmap* b, Index end)
{
    return end - Bitmap_risenBitCount(b, end);
}


/** Finds the 1st risen bit (value=1)
 *      in the range [\a begin, \a end) of a give bitmap.
 * @param b the bitmap
 * @param begin the index of the \a begin search bit
 * @param end the \em limit index of the searched bits (= \em last+1).
 * @return the index of the found risen bit;
 * @return \a end if not found
 */
Index Bitmap_findRisenBit(const Bitmap* b, Index begin, Index end)
{
    Index i; // index of byte in the byte-array
    Index j; // index of bit int a byte
    const Index beginByteIdx = begin/ELEM_BITS;
    const Index lastByteIdx = (end-1)/ELEM_BITS;
    Byte r;  // remainder

    assert (begin < end);

    // To search the prefix head bits, which
    //   may be a whole or non-wole byte
    r = begin%ELEM_BITS;
    if (b->a[beginByteIdx] != 0) {
        Byte byte = b->a[beginByteIdx] >> r;
        for (j=r; j<ELEM_BITS; j++) {
            if (byte & 0x1) {
                assert ((begin+j-r) < end);
                return begin + j - r;
            }
            byte >>= 1;
        }
    }

    // To search the bits of body whole-bytes and tail bits.
    for (i=beginByteIdx+1; i<=lastByteIdx; i++) {
        if (b->a[i] != 0) {
            Byte byte = b->a[i];
            size_t restBits = (end - begin)
                             - (i-(beginByteIdx+1)) * ELEM_BITS
                             - (ELEM_BITS - r);
            Byte bits = (restBits<ELEM_BITS) ? restBits : ELEM_BITS;
            for (j=0; j<bits; j++) {
                if (byte & 0x1) {
                    assert ((i*ELEM_BITS + j)<end);
                    return i*ELEM_BITS + j;
                }
                byte >>= 1;
            }
        }
    }
    return end;
}


/** Finds a risen bit (value=1) in a ring-shaped bitmap.
 *      - It first searches the bits in [\a begin, \a end);
 *      - if not found, re-searches the bits in [\a 0, \a begin).
 *
 * @param b the bitmap
 * @param begin the index of the \a begin search bit
 * @param end the \em limit index of the searched bits (= \em last+1).
 * @return the index of the found risen bit;
 * @return \a end if not found
 */
Index Bitmap_findRisenBitRingedly(const Bitmap* b, Index begin, Index end)
{
    Index i = Bitmap_findRisenBit(b, begin, end);
    if (i == end) {
        i = Bitmap_findRisenBit(b, 0, begin);
        if (i == begin)
            return end;
    }
    return i;
}


//-----------------------------------------------------------------------------
// Byte-wise operators
//-----------------------------------------------------------------------------

/** Sets all 8 bits of byte[i] to 1
 * @param b the bitmap
 * @param i the index of the \em byte to be set
 * @see Bitmap_clrByteBits()
 */
void Bitmap_setByteBits(Bitmap* b, Index i)
{
    cassert (ELEM_BITS == 8);

    b->a[i] = 0xFF;
}


/** Clears all 8 bits of byte[i] to 0
 * @param b the bitmap
 * @param i the index of the cleared \em byte
 * @see Bitmap_setByteBits()
 */
void Bitmap_clrByteBits(Bitmap* b, Index i)
{
    cassert (ELEM_BITS == 8);

    b->a[i] = 0x00;
}


/** Finds the lst risen byte (value=FFh)
 *      in the range [\a begin, \a end) of a give byte array.
 * @param b a bitmap containing the byte-array
 * @param begin the index of the \a begin search byte
 * @param end the \em limit index of the searched bytes (= \em last+1).
 * @return the index of the found risen byte;
 * @return \a end if not found
 */
Index Bitmap_findRisenByte(const Bitmap* b, Index begin, Index end)
{
    cassert (ELEM_BITS == 8);

    Index i;

    for (i=begin; i<end; i++)
        if (b->a[i] == 0xFF) break;
    return i;
}


/** Finds a risen byte (value=FFh) in a ring-shaped byte-array.
 *      - It first searches bytes in [\a begin, \a end);
 *      - if not found, re-searches bytes in [\a 0, \a begin).
 *
 * @param b a bitmap containing the byte-array
 * @param begin the index of the \a begin search byte
 * @param end the \em limit index of the searched bytes (= \em last+1).
 * @return the index of the found risen byte;
 * @return \a end if not found
 */
Index Bitmap_findRisenByteRingedly(const Bitmap* b, Index begin, Index end)
{
    cassert (ELEM_BITS == 8);

    Index i = Bitmap_findRisenByte(b, begin, end);

    if (i == end) {
        i = Bitmap_findRisenByte(b, 0, begin);
        if (i == begin)
            return end;
    }
    return i;
}

//-----------------------------------------------------------------------------

/** Returns the total bytes of the bitmap. */
size_t Bitmap_totalBytes(const Bitmap* b)
{
    return Bitmap_totalBits(b) / 8;
}


/** Returns the byte array of the bitmap. */
uint8_t* Bitmap_byteArray(const Bitmap* b)
{
    return (uint8_t*)(b->a);
}


//------------------------------------------------------------------------------
// Partition-wise operators
//------------------------------------------------------------------------------

/** Sets the total bits of a partition of a bitmap.
 * @param[out] b the bitmap
 * @param[in] nBits the number of bits of a partition of the bitmap
 */
void Bitmap_setPartTotalBits(Bitmap* b, size_t nBits)
{
    ASSERT_OP (nBits, <=, ELEM_BITS);
    ASSERT_OP (nBits, >, 0);

    b->p = nBits;
}


/** Returns the total partitions of the bitmap. */
size_t Bitmap_totalParts(const Bitmap* b)
{
    return Bitmap_totalBits(b) / b->p;
}


/** Returns maxima value of a partition. */
Elem Bitmap_maxPartValue(const Bitmap* b)
{
    Elem x;

    ASSERT_OP (b->p, >=, 1);
    ASSERT_OP (b->p, <=, ELEM_BITS);

    x = 1 << (b->p - 1);
    return x | (x - 1);
}


/** Sets part[i] to a value
 * @param[out] b the bitmap
 * @param[in] i the index of the \em bits to be set
 * @param[in] val the value to set
 */
void Bitmap_setPart(Bitmap* b, Index i, Elem val)
{
    Index bit_idx;
    Index begin = i*b->p;
    Index end = begin + b->p;

    ASSERT_OP (i, <, Bitmap_totalParts(b));
    ASSERT_OP (val, <=, Bitmap_maxPartValue(b));

    for (bit_idx=begin; bit_idx<end; ++bit_idx) {
        if (val & 0x00000001)
            Bitmap_setBit(b, bit_idx);
        else
            Bitmap_clrBit(b, bit_idx);
        val >>= 1;
    }
}


/** Gets part[i]
 * @param[in] b the bitmap
 * @param[in] i the index of the gotten bits
 * @return bits[i]
 */
Elem Bitmap_getPart(const Bitmap* b, Index i)
{
    Index begin = i*b->p;
    Index end = begin + b->p;
    Index bit_idx = end;
    Elem val = 0;

    ASSERT_OP (i, <, Bitmap_totalParts(b));

    for (;;) {
        val |= Bitmap_getBit(b, --bit_idx);
        if (bit_idx == begin)
            break;
        val <<= 1;
    }

    return val;
}


/** Fills all partitions with a given value.
 * @param[in] b the bitmap
 * @param[in] val the value to fill
 */
void Bitmap_fillAllParts(Bitmap* b, Elem val)
{
    Index i;

    for (i=0; i<Bitmap_totalParts(b); ++i)
        Bitmap_setPart(b, i, val);
}


//-----------------------------------------------------------------------------

