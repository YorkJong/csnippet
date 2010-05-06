/**
 * @file Bitmap.c
 *      This module provides \em bitmap operations for byte-array.
 *      The bitmap is implemented with byte-array, each byte keeps 8 bits.
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @date 2005/3/11 (initial)
 * @date 2005/5/14 (last revise)
 * @version 2.0
 * @see Bitmap.h
 * @see Bitmap_test.c
 */
#include "platform.h"
#include "Bitmap.h"
#include <assert.h>
#include <stddef.h>

enum {
    BYTE_BITS= 8 ///< the total bits of a byte
};


//-----------------------------------------------------------------------------
// Bit-wise operators
//-----------------------------------------------------------------------------

/** Sets bit[i] to 1
 * @param a a byte-array that stores the bitmap
 * @param i the index of the \em bit to be set
 * @see ClrBit()
 */
void SetBit( Byte a[], Index i )
{
    a[i/BYTE_BITS] |= (1 << (i%BYTE_BITS));
}


/** Clears bit[i] to 0
 * @param a a byte-array that stores the bitmap
 * @param i the index of the cleared \em bit
 * @see SetBit()
 */
void ClrBit( Byte a[], Index i )
{
    a[i/BYTE_BITS] &= ~(1 << (i%BYTE_BITS));
}


/** Gets bit[i]
 * @param a a byte-array that stores the bitmap
 * @param i the index of the gotten bit
 */
Bit GetBit( const Byte a[], Index i )
{
    Byte r= i%BYTE_BITS;
    return (a[i/BYTE_BITS] & (1<<r))  >>  r;
}


/** Counts the total risen bit (value=1)
 *      in the range [\em 0, \a end) of a give bitmap.
 * @param a a byte-array that stores the given bitmap
 * @param end the \em limit index of the counted bits (= \em last+1).
 * @return the count result
 * @see SunkBitCount()
 */
size_t RisenBitCount( const Byte a[], Index end )
{
    Index i; // index of byte in the byte-array
    Index j; // index of bit in a byte
    const Index lastByteIdx= (end-1)/BYTE_BITS;
    size_t restBits= end;
    size_t result= 0;

    for (i= 0; i<=lastByteIdx; i++) {
        if (a[i] != 0) {
            Byte b= a[i];
            Byte bits= (restBits<BYTE_BITS) ? restBits : BYTE_BITS;
            for (j= 0; j<bits; j++) {
                result += b&0x1;
                b >>= 1;
            }
        }
        restBits -= BYTE_BITS;
    }
    return result;
}


/** Counts the total sunk bit (value=0)
 *      in the range [\em 0, \a end) of a give bitmap.
 * @param a a byte-array that stores the given bitmap
 * @param end the \em limit index of the counted bits (= \em last+1).
 * @return the count result
 * @see RisenBitCount()
 */
size_t SunkBitCount( const Byte a[], Index end )
{
    return end - RisenBitCount(a, end);
}


/** Finds the 1st risen bit (value=1)
 *      in the range [\a begin, \a end) of a give bitmap.
 * @param a a byte-array that stores the given bitmap
 * @param begin the index of the \a begin search bit
 * @param end the \em limit index of the searched bits (= \em last+1).
 * @return the index of the found risen bit;
 * @return \a end if not found
 */
Index FindRisenBit( const Byte a[], Index begin, Index end )
{
    Index i; // index of byte in the byte-array
    Index j; // index of bit int a byte
    const Index beginByteIdx= begin/BYTE_BITS;
    const Index lastByteIdx= (end-1)/BYTE_BITS;
    Byte r;  // remainder

    assert (begin < end);

    // To search the prefix head bits, which
    //   may be a whole or non-wole byte
    r= begin%BYTE_BITS;
    if (a[beginByteIdx] != 0) {
        Byte b= a[beginByteIdx] >> r;
        for (j= r; j<BYTE_BITS; j++) {
            if (b & 0x1) {
                assert ((begin+j-r) < end);
                return begin + j - r;
            }
            b >>= 1;
        }
    }

    // To search the bits of body whole-bytes and tail bits.
    for (i= beginByteIdx+1; i<=lastByteIdx; i++) {
        if (a[i] != 0) {
            Byte b= a[i];
            size_t restBits= (end - begin)
                             - (i-(beginByteIdx+1)) * BYTE_BITS
                             - (BYTE_BITS - r);
            Byte bits= (restBits<BYTE_BITS) ? restBits : BYTE_BITS;
            for (j= 0; j<bits; j++) {
                if (b & 0x1) {
                    assert ((i*BYTE_BITS + j)<end);
                    return i*BYTE_BITS + j;
                }
                b >>= 1;
            }
        }
    }
    return end;
}


/** Finds a risen bit (value=1) in a ring-shaped bitmap.
 *      - It first searches the bits in [\a begin, \a end);
 *      - if not found, re-searches the bits in [\a 0, \a begin).
 *
 * @param a a byte-array that stores the given ringed bitmap
 * @param begin the index of the \a begin search bit
 * @param end the \em limit index of the searched bits (= \em last+1).
 * @return the index of the found risen bit;
 * @return \a end if not found
 */
Index FindRisenBitRingedly( const Byte a[], Index begin, Index end )
{
    Index i= FindRisenBit( a, begin, end );
    if (i == end) {
        i= FindRisenBit( a, 0, begin );
        if (i == begin)
            return end;
    }
    return i;
}


//-----------------------------------------------------------------------------
// Byte-wise operators
//-----------------------------------------------------------------------------

/** Sets all 8 bits of byte[i] to 1
 * @param a a byte-array that stores the bitmap
 * @param i the index of the \em byte to be set
 * @see ClrByteBits()
 */
void SetByteBits( Byte a[], Index i )
{
    a[i]= 0xFF;
}


/** Clears all 8 bits of byte[i] to 0
 * @param a a byte-array that stores the bitmap
 * @param i the index of the cleared \em byte
 * @see SetByteBits()
 */
void ClrByteBits( Byte a[], Index i )
{
    a[i]= 0x00;
}


/** Finds the lst risen byte (value=FFh)
 *      in the range [\a begin, \a end) of a give byte array.
 * @param a the byte-array
 * @param begin the index of the \a begin search byte
 * @param end the \em limit index of the searched bytes (= \em last+1).
 * @return the index of the found risen byte;
 * @return \a end if not found
 */
Index FindRisenByte( const Byte a[], Index begin, Index end )
{
    Index i;
    for (i= begin; i<end; i++)
        if (a[i] == 0xFF) break;
    return i;
}


/** Finds a risen byte (value=FFh) in a ring-shaped byte-array.
 *      - It first searches bytes in [\a begin, \a end);
 *      - if not found, re-searches bytes in [\a 0, \a begin).
 *
 * @param a the byte-array
 * @param begin the index of the \a begin search byte
 * @param end the \em limit index of the searched bytes (= \em last+1).
 * @return the index of the found risen byte;
 * @return \a end if not found
 */
Index FindRisenByteRingedly( const Byte a[], Index begin, Index end )
{
    Index i= FindRisenByte( a, begin, end );
    if (i == end) {
        i= FindRisenByte( a, 0, begin );
        if (i == begin)
            return end;
    }
    return i;
}
