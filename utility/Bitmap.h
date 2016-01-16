/**
 * @file Bitmap.h
 *      This module provides \em bitmap operations for byte-array.
 * @author Jiang Yu-Kuan <yukuan.jiang@gmail.com>
 * @date 2005/03/12 (initial)
 * @date 2016/01/15 (last revise)
 * @version 1.5
 * @see Bitmap.c
 * @see Bitmap_test.c
 */
#ifndef _BITMAP_H_
#define _BITMAP_H_


#include <stddef.h>

#include "platform.h"


/// Returns total slots of a given number of bits.
#define BITMAP_NSLOTS(nb)    (((nb) + ELEM_BITS - 1) / ELEM_BITS)

typedef bool Bit;   ///< Bit type for Bitmap
typedef uint8_t Elem;

enum {
    ELEM_BITS = 8  ///< the total bits of an element
};


typedef struct {
    Elem *a;    ///< the pointer to an byte array
    size_t n;       ///< the number of elements of the array
} Bitmap;

void Bitmap_init(Bitmap*, Elem a[], size_t n);

size_t Bitmap_totalBits(const Bitmap*);
void Bitmap_setBit(Bitmap*, Index i);
void Bitmap_clrBit(Bitmap*, Index i);
Bit Bitmap_getBit(const Bitmap*, Index i);

void Bitmap_clearAllBits(Bitmap*);
void Bitmap_copyAllBits(const Bitmap* src, Bitmap* tgt);

//----------------------------------------------------------------------------

size_t Bitmap_risenBitCount(const Bitmap*, Index end);
size_t Bitmap_sunkBitCount(const Bitmap*, Index end);

Index Bitmap_findRisenBit(const Bitmap*, Index begin, Index end);
Index Bitmap_findRisenBitRingedly(const Bitmap*, Index begin, Index end);

//----------------------------------------------------------------------------

void Bitmap_setByteBits(Bitmap*, Index i);
void Bitmap_clrByteBits(Bitmap*, Index i);

Index Bitmap_findRisenByte(const Bitmap*, Index begin, Index end);
Index Bitmap_findRisenByteRingedly(const Bitmap*, Index begin, Index end);

size_t Bitmap_totalBytes(const Bitmap*);
uint8_t* Bitmap_byteArray(const Bitmap*);

//----------------------------------------------------------------------------

#endif //  _BITMAP_H_


/** @example Bitmap_test.c
 *      This is an example of how to use the Bitmap operations.
 */
