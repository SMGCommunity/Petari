#pragma once

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
