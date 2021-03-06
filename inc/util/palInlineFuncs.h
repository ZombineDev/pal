/*
 *******************************************************************************
 *
 * Copyright (c) 2014-2017 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/**
 ***********************************************************************************************************************
 * @file  palInlineFuncs.h
 * @brief PAL utility collection inline functions.
 ***********************************************************************************************************************
 */

#pragma once

#include "palAssert.h"
#include <cstdlib>
#include <cstring>

namespace Util
{

/// Describes a value type, primarily used for loading settings values.
enum class ValueType : uint32
{
    Boolean,  ///< Boolean type.
    Int,      ///< Signed integer type.
    Uint,     ///< Unsigned integer type.
    Uint64,   ///< 64-bit unsigned integer type.
    Float,    ///< Floating point type.
    Str,      ///< String type.
};

/// Increments a pointer by nBytes by first casting it to a uint8*.
///
/// @returns Incremented pointer.
PAL_INLINE void* VoidPtrInc(
    const void* p,         ///< [in] Pointer to be incremented.
    size_t      numBytes)  ///< Number of bytes to increment the pointer by.
{
    void* ptr = const_cast<void*>(p);
    return (static_cast<uint8*>(ptr) + numBytes);
}

/// Decrements a pointer by nBytes by first casting it to a uint8*.
///
/// @returns Decremented pointer.
PAL_INLINE void* VoidPtrDec(
    const void* p,         ///< [in] Pointer to be decremented.
    size_t      numBytes)  ///< Number of bytes to decrement the pointer by.
{
    void* ptr = const_cast<void*>(p);
    return (static_cast<uint8*>(ptr) - numBytes);
}

/// Finds the number of bytes between two pointers by first casting them to uint8*.
///
/// This function expects the first pointer to not be smaller than the second.
///
/// @returns Number of bytes between the two pointers.
PAL_INLINE size_t VoidPtrDiff(
    const void* p1,  ///< [in] First pointer (higher address).
    const void* p2)  ///< [in] Second pointer (lower address).
{
    PAL_ASSERT(p1 >= p2);
    return (static_cast<const uint8*>(p1) - static_cast<const uint8*>(p2));
}

/// Determines if any of the bits set in "test" are also set in "src".
///
/// @returns True if any bits in "test" are set in "src", false otherwise.
PAL_INLINE bool TestAnyFlagSet(
    uint32 src,   ///< Source pattern.
    uint32 test)  ///< Test pattern.
{
    return ((src & test) != 0);
}

/// Determines if all of the bits set in "test" are also set in "src".
///
/// @returns True if all bits set in "test" are also set in "src", false otherwise.
PAL_INLINE bool TestAllFlagsSet(
    uint32 src,   ///< Source pattern.
    uint32 test)  ///< Test pattern.
{
    return ((src & test) == test);
}

/// Tests if a single bit in a "wide bitfield" is set. A "wide bifield" is a bitfield which spans an array of
/// integers because there are more flags than bits in one integer.
///
/// @param [in] bitfield  Reference to the bitfield being tested
/// @param [in] bit       Index of the flag to test
///
/// @returns True if the flag is set.
template <typename T, size_t N>
PAL_INLINE bool WideBitfieldIsSet(
    T      (&bitfield)[N],
    uint32 bit)
{
    const uint32 index = (bit / (sizeof(T) << 3));
    const uint32 mask  = (1 << (bit & ((sizeof(T) << 3) - 1)));

    return (0 != (bitfield[index] & mask));
}

/// Sets a single bit in a "wide bitfield" to one. A "wide bifield" is a bitfield which spans an array of
/// integers because there are more flags than bits in one integer.
///
/// @param [in] bitfield  Reference to the bitfield being modified
/// @param [in] bit       Index of the flag to set
template <typename T, size_t N>
PAL_INLINE void WideBitfieldSetBit(
    T      (&bitfield)[N],
    uint32 bit)
{
    const uint32 index = (bit / (sizeof(T) << 3));
    const uint32 mask  = (1 << (bit & ((sizeof(T) << 3) - 1)));

    bitfield[index] |= mask;
}

/// Clears a single bit in a "wide bitfield" to zero. A "wide bifield" is a bitfield which spans an array of
/// integers because there are more flags than bits in one integer.
///
/// @param [in] bitfield  Reference to the bitfield being modified
/// @param [in] bit       Index of the flag to set
template <typename T, size_t N>
PAL_INLINE void WideBitfieldClearBit(
    T      (&bitfield)[N],
    uint32 bit)
{
    const uint32 index = (bit / (sizeof(T) << 3));
    const uint32 mask  = (1 << (bit & ((sizeof(T) << 3) - 1)));

    bitfield[index] &= ~mask;
}

/// XORs all of the bits in two "wide bitfields". A "wide bifield" is a bitfield which spans an array of integers
/// because there are more flags than bits in one integer.
///
/// @param [in] bitfield1 Reference to the first bitfield.
/// @param [in] bitfield2 Reference to the second bitfield.
/// @param [in] pOut      Result of (bitfield1 ^ bitfield2)
template <typename T, size_t N>
PAL_INLINE void WideBitfieldXorBits(
    T      (&bitfield1)[N],
    T      (&bitfield2)[N],
    T*     pOut)
{
    for (uint32 i = 0; i < N; i++)
    {
        pOut[i] = (bitfield1[i] ^ bitfield2[i]);
    }
}

/// ANDs all of the bits in two "wide bitfields". A "wide bifield" is a bitfield which spans an array of integers
/// because there are more flags than bits in one integer.
///
/// @param [in] bitfield1 Reference to the first bitfield.
/// @param [in] bitfield2 Reference to the second bitfield.
/// @param [in] pOut      Result of (bitfield1 & bitfield2)
template <typename T, size_t N>
PAL_INLINE void WideBitfieldAndBits(
    T      (&bitfield1)[N],
    T      (&bitfield2)[N],
    T*     pOut)
{
    for (uint32 i = 0; i < N; i++)
    {
        pOut[i] = (bitfield1[i] & bitfield2[i]);
    }
}

/// Determines if a value is a power of two.
///
/// @returns True if it is a power of two, false otherwise.
PAL_INLINE bool IsPowerOfTwo(
    uint64 value)  ///< Value to check.
{
    return (value == 0) ? false : ((value & (value - 1)) == 0);
}

/// Determines if 'value' is at least aligned to the specified power-of-2 alignment.
///
/// @returns True if aligned, false otherwise.
PAL_INLINE bool IsPow2Aligned(
    uint64 value,      ///< Value to check.
    uint64 alignment)  ///< Desired alignment.
{
    PAL_ASSERT(IsPowerOfTwo(alignment));
    return ((value & (alignment - 1)) == 0);
}

/// Rounds the specified uint 'value' up to the nearest value meeting the specified 'alignment'.  Only power of 2
/// alignments are supported by this function.
///
/// @returns Aligned value.
template<typename T>
PAL_INLINE T Pow2Align(
    T      value,      ///< Value to align.
    uint64 alignment)  ///< Desired alignment (must be a power of 2).
{
    PAL_ASSERT(IsPowerOfTwo(alignment));
    return ((value + static_cast<T>(alignment) - 1) & ~(static_cast<T>(alignment) - 1));
}

/// Implements an alternative version of integer division in which the quotient is always rounded up instead of down.
///
/// @returns The rounded quotient.
template<typename T>
constexpr PAL_INLINE T RoundUpQuotient(
    T dividend, ///< Value to divide.
    T divisor)  ///< Value to divide by.
{
    return ((dividend + (divisor - 1)) / divisor);
}

/// Rounds up the specified integer to the nearest multiple of the specified alignment value.
///
/// @returns Rounded value.
template<typename T>
PAL_INLINE T RoundUpToMultiple(
    T operand,   ///< Value to be aligned.
    T alignment) ///< Alignment desired.
{
    return (((operand + (alignment - 1)) / alignment) * alignment);
}

/// Rounds down the specified integer to the nearest multiple of the specified alignment value.
///
/// @returns Rounded value.
template<typename T>
PAL_INLINE T RoundDownToMultiple(
    T operand,    ///< Value to be aligned.
    T alignment)  ///< Alignment desired.
{
    return ((operand / alignment) * alignment);
}

/// Rounds the specified 'value' down to the nearest value meeting the specified 'alignment'.  Only power of 2
/// alignments are supported by this function.
///
/// @returns Rounded value.
template<typename T>
PAL_INLINE T Pow2AlignDown(
    T      value,      ///< Value to align.
    uint64 alignment)  ///< Desired alignment (must be a power of 2).
{
    PAL_ASSERT(IsPowerOfTwo(alignment));
    return (value & ~(alignment - 1));
}

/// Rounds the specified uint 'value' up to the nearest power of 2
///
/// @returns Power of 2 padded value.
template<typename T>
PAL_INLINE T Pow2Pad(
    T value)  ///< Value to pad.
{
    T ret = 1;
    if (IsPowerOfTwo(value))
    {
        ret = value;
    }
    else
    {
        while (ret < value)
        {
            ret <<= 1;
        }
    }

    return ret;
}

/// Determines the maximum of two numbers.
///
/// @returns The larger of the two inputs.
template<typename T>
PAL_INLINE T Max(
    T value1,  ///< First value to check.
    T value2)  ///< Second value to check.
{
    return ((value1 > value2) ? value1 : value2);
}

template<typename T>
constexpr PAL_INLINE T ConstexprMax(
    T value1,  ///< First value to check.
    T value2)  ///< Second value to check.
{
    return ((value1 > value2) ? value1 : value2);
}

/// Determines the minimum of two numbers.
///
/// @returns The smaller of the two inputs.
template<typename T>
PAL_INLINE T Min(
    T value1,  ///< First value to check.
    T value2)  ///< Second value to check.
{
    return ((value1 < value2) ? value1 : value2);
}

/// Clamps the input number so that it falls in-between the lower and upper bounds (inclusive).
///
/// @returns Clamped input number.
template<typename T>
PAL_INLINE T Clamp(
    T input,      ///< Input number to clamp.
    T lowBound,   ///< Lower-bound to clamp to.
    T highBound)  ///< Upper-bound to clamp to.
{
    if (input < lowBound)
    {
        return lowBound;
    }
    else if (input > highBound)
    {
        return highBound;
    }
    else
    {
        return input;
    }
}

/// Computes the base-2 logarithm of an unsigned 64-bit integer.
///
/// If the given integer is not a power of 2, this function will not provide an exact answer.
///
/// @returns log_2(u)
template< typename T>
PAL_INLINE uint32 Log2(
    T u)  ///< Value to compute the logarithm of.
{
    uint32 logValue = 0;

    while (u > 1)
    {
        ++logValue;
        u >>= 1;
    }

    return logValue;
}

/// Computes the base-2 logarithm of an unsigned 64-bit integer based on ceiling
///
/// If the given integer is not a power of 2, this function will not provide an exact answer.
///
/// @returns ceilLog_2(u)
template< typename T>
PAL_INLINE uint32 CeilLog2(
    T u)  ///< Value to compute the ceil logarithm of.
{
    uint32 logValue = 0;

    while ((0x1ul << logValue) < u)
    {
        logValue++;
    }

    return logValue;
}

/// Scans the specified bit-mask for the least-significant '1' bit.
///
/// @returns True if the input was nonzero; false otherwise.
PAL_INLINE bool BitMaskScanForward(
    uint32* pIndex,  ///< [out] Index of least-significant '1' bit.
    uint32  mask)    ///< Bit-mask to scan.
{
    bool result = false;

    const uint32 indexPlusOne = __builtin_ffs(mask);
    if (indexPlusOne == 0)
    {
        *pIndex = 0;
        result  = false;
    }
    else
    {
        *pIndex = (indexPlusOne - 1);
        result  = true;
    }

    return result;
}

/// Scans the specified wide bit-mask for the least-significant '1' bit.
///
/// @returns True if input was nonzero; false otherwise.
template <typename T, size_t N>
PAL_INLINE bool WideBitMaskScanForward(
    uint32* pIndex,        ///< [out] Index of least-significant '1' bit.
    T       (&mask)[N])    ///< Bit-mask to scan.
{
    const  uint32 originalIndex = (*pIndex);
    uint32        maskIndex     = ((*pIndex) / (sizeof(T) << 3));

    // Check to see if the wide bitmask has some bits set.
    uint32 index = 0;
    while ((mask[index] == 0) && (++index < N));
    bool result = (index < N);

    // We're now using this to represent a local copy of the index of the least-significant '1' bit.
    index = (*pIndex);

    while (result == true)
    {
        const uint32 indexPlusOne = __builtin_ffs(mask[maskIndex]);
        if (indexPlusOne == 0)
        {
            *pIndex = 0;
            result = false;
        }
        else
        {
            *pIndex = (indexPlusOne - 1);
            result = true;
        }
        if (result == false)
        {
            maskIndex++;
            result = (maskIndex < N);
        }
        else
        {
            (*pIndex) = (*pIndex) + (maskIndex * (sizeof(T) << 3));
            break;
        }
    }

    return result;
}

/// Returns the high 32 bits of a 64-bit integer.
///
/// @returns Returns the high 32 bits of a 64-bit integer.
PAL_INLINE uint32 HighPart(
    uint64 value)  ///< 64-bit input value.
{
    return (value & 0xFFFFFFFF00000000) >> 32;
}

/// Returns the low 32 bits of a 64-bit integer.
///
/// @returns Returns the low 32 bits of a 64-bit integer.
PAL_INLINE uint32 LowPart(
    uint64 value)  ///< 64-bit input value.
{
    return (value & 0x00000000FFFFFFFF);
}

/// Converts a byte value to the equivalent number of DWORDs (uint32) rounded up.  I.e., 3 bytes will return 1 dword.
///
/// @returns Number of dwords necessary to cover numBytes.
PAL_INLINE uint32 NumBytesToNumDwords(
    uint32 numBytes)  ///< Byte count to convert.
{
    return Pow2Align(numBytes, static_cast<uint32>(sizeof(uint32))) / sizeof(uint32);
}

/// Performs a safe strcpy by requiring the destination buffer size.
PAL_INLINE void Strncpy(
    char*       pDst,     ///< [out] Destination string.
    const char* pSrc,     ///< [in] Source string to be copied into destination.
    size_t      dstSize)  ///< Size of the destination buffer in bytes.
{
    PAL_ASSERT(pDst != nullptr);
    PAL_ASSERT(pSrc != nullptr);
    PAL_ASSERT(strlen(pSrc) < dstSize);

    strncpy(pDst, pSrc, (dstSize - 1));
    pDst[dstSize - 1] = '\0';
}

/// Simple wrapper for strncat or strncat_s which provides a safe version of strncat.
PAL_INLINE void Strncat(
    char*       pDst,     ///< [in,out] Destination string.
    size_t      sizeDst,  ///< Length of the destination string, including the null terminator.
    const char* pSrc)     ///< [in] Source string.
{
    PAL_ASSERT((pDst != nullptr) && (pSrc != nullptr));

    // Compute the length of the destination string to prevent buffer overruns.
    const size_t dstLength = strlen(pDst);
    strncat(pDst, pSrc, (sizeDst - dstLength - 1));
}

/// Rounds the specified pointer up to the nearest value meeting the specified 'alignment'.  Only power of 2 alignments
/// are supported by this function.
///
/// @returns Aligned pointer.
PAL_INLINE void* VoidPtrAlign(
    void*  ptr,        ///< Pointer to align.
    size_t alignment)  ///< Desired alignment.
{
    // This function only works for POW2 alignment
    PAL_ASSERT(IsPowerOfTwo(alignment));

    return reinterpret_cast<void*>(
               (reinterpret_cast<size_t>(ptr) + (alignment - 1)) & ~(alignment - 1));
}

/// Converts a raw string value to the correct data type.
PAL_INLINE void StringToValueType(
    const char* pStrValue,  ///< [in] Setting value in string form.
    ValueType   type,       ///< Data type of the value being converted.
    size_t      valueSize,  ///< Size of pValue buffer.
    void*       pValue)     ///< [out] Converted setting value buffer.
{
    switch (type)
    {
    case ValueType::Boolean:
        *(static_cast<bool*>(pValue)) = ((atoi(pStrValue)) ? true : false);
        break;
    case ValueType::Int:
        *(static_cast<int32*>(pValue)) = static_cast<int32>(strtol(pStrValue, nullptr, 0));
        break;
    case ValueType::Uint:
        *(static_cast<uint32*>(pValue)) = static_cast<uint32>(strtoul(pStrValue, nullptr, 0));
        break;
    case ValueType::Uint64:
        *(static_cast<uint64*>(pValue)) = static_cast<uint64>(strtoull(pStrValue, nullptr, 0));
        break;
    case ValueType::Float:
        *(static_cast<float*>(pValue)) = static_cast<float>(atof(pStrValue));
        break;
    case ValueType::Str:
        Strncpy(static_cast<char*>(pValue), pStrValue, valueSize);
        break;
    }
}

/// Hashes the provided string using FNV1a hashing (http://www.isthe.com/chongo/tech/comp/fnv/) algorithm.
///
/// @returns 32-bit hash generated from the provided string.
PAL_INLINE uint32 HashString(
    const char* pStr,     ///< [in] String to be hashed.
    size_t      strSize)  ///< Size of the input string.
{
    PAL_ASSERT((pStr != nullptr) && (strSize > 0));

    const uint32 fnvPrime  = 16777619;
    const uint32 fnvOffset = 2166136261;

    uint32 hash = fnvOffset;

    for (uint32 i = 0; i < strSize; i++)
    {
        hash ^= static_cast<uint32>(pStr[i]);
        hash *= fnvPrime;
    }

    return hash;
}

/// Counts the number of one bits (population count) in a 32-bit unsinged integer using some bitwise magic explained in
/// the Software Optimization Guide for AMD64 Processors.
///
/// @returns Number of one bits in the input
PAL_INLINE uint32 CountSetBits(
    uint32  value)    ///< [in] The value need to be counted
{
    uint32 x = value;

    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

    return x;
}

} // Util
