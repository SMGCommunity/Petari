#pragma once

#include "revolution/types.h"
#include <stdint.h>

template < typename T >
inline T* JSUConvertOffsetToPtr(const void* ptr, uintptr_t offset) {
    return offset == 0 ? NULL : (T*)((intptr_t)ptr + (intptr_t)offset);
}

template < typename T >
inline T* JSUConvertOffsetToPtr(const void* ptr, const void* offset) {
    T* ret;
    if (offset == NULL) {
        ret = NULL;
    } else {
        ret = (T*)((intptr_t)ptr + (intptr_t)offset);
    }
    return ret;
}

inline u8 JSULoNibble(u8 value) {
    return value & 0x0f;
}

inline u8 JSUHiNibble(u8 value) {
    return value >> 4;
}

inline u8 JSULoByte(u16 value) {
    return value & 0xff;
}

inline u8 JSUHiByte(u16 value) {
    return value >> 8;
}

inline u16 JSUHiHalf(u32 value) {
    return (value >> 16);
}

inline u16 JSULoHalf(u32 value) {
    return value;
}
