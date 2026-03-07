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
