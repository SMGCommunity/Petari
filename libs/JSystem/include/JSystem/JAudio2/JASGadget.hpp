#pragma once

#include <mem.h>
#include <revolution/types.h>


template < typename T >
class JASPtrTable {
public:
    JASPtrTable(T** ptrTable, u32 len) {
        mLen = len;
        mPtrTable = ptrTable;
        memset(mPtrTable, 0, len * sizeof(T*));
    }

    T* get(u32 idx) const {
        if (idx >= mLen)
            return nullptr;
        return mPtrTable[idx];
    }

    void set(u32 idx, T* value) {
        mPtrTable[idx] = value;
    }

    T** mPtrTable;  // 0x0
    u32 mLen;       // 0x4
};

template < typename T, u32 LEN >
class JASPtrArray : public JASPtrTable< T > {
public:
    JASPtrArray() : JASPtrTable(mPtrArray, LEN) {
    }

    T* mPtrArray[LEN];  // 0x8
};
