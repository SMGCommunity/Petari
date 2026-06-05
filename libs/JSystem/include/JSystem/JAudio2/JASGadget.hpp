#pragma once

#include <mem.h>
#include <revolution/types.h>

template < typename T >
class JASPtrListAdaptor {
public:
    // TEMP NO_INLINE
    JASPtrListAdaptor(T* pData) NO_INLINE : mNext(nullptr), mData(pData){};

    ~JASPtrListAdaptor(){};

    JASPtrListAdaptor< T >* getNext() const {
        return mNext;
    }

    T* get() const {
        return mData;
    }

    /* 0x00 */ JASPtrListAdaptor< T >* mNext;
    /* 0x04 */ T* mData;
};

template < typename T >
class JASPtrList {
public:
    JASPtrList() : mList(nullptr){};

    JASPtrList(JASPtrListAdaptor< T >* pAdapter) {
        mList = pAdapter;
    }

    bool isEmpty() const {
        return mList == nullptr;
    }

    ~JASPtrList(){};

    JASPtrListAdaptor< T >* begin() const {
        return mList;
    }

    /* 0x00 */ JASPtrListAdaptor< T >* mList;
};

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
