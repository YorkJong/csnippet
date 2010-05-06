/**
 * @file Bitmap.h
 *      This module provides \em bitmap operations for byte-array.
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @date 2005/3/12 (initial)
 * @date 2005/4/19 (last revise)
 * @version 1.5
 * @see Bitmap.c
 * @see Bitmap_test.c
 */
#ifndef _BITMAP_H_
#define _BITMAP_H_


#include "platform.h"
#include <stddef.h>

typedef bool Bit;   ///< Bit type for Bitmap

void SetBit( Byte a[], Index i );
void ClrBit( Byte a[], Index i );
Bit GetBit( const Byte a[], Index i );

size_t RisenBitCount( const Byte a[], Index end );
size_t SunkBitCount( const Byte a[], Index end );

Index FindRisenBit( const Byte a[], Index begin, Index end );
Index FindRisenBitRingedly( const Byte a[], Index begin, Index end );

void SetByteBits( Byte a[], Index i );
void ClrByteBits( Byte a[], Index i );

Index FindRisenByte( const Byte a[], Index begin, Index end );
Index FindRisenByteRingedly( const Byte a[], Index begin, Index end );


#endif //  _BITMAP_H_


/** @example Bitmap_test.c
 *      This is an example of how to use the Bitmap operations.
 */
