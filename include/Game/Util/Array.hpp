#pragma once

#include "revolution.h"
#include "Inline.hpp"

namespace MR {
    template<class T>
    class Vector;

    template<class T>
    class AssignableArray {
    public:
        typedef T Item;

        inline AssignableArray() {
            mArr = 0;
            mMaxSize = 0;
        }

        ~AssignableArray() NO_INLINE {
            if (mArr) {
                delete[] mArr;
            }
        }

        T& operator[](int idx) {
            return mArr[idx];
        }

        const T& operator[](int idx) const {
            return mArr[idx];
        }

        inline void init(s32 cnt) {
            mArr = new T[cnt];
            mMaxSize = cnt;
        }

        inline void assign(T what, s32 where) {
            mArr[where] = what;
        }

        int size() const {
            return mMaxSize;
        }

        T* begin() {
            return &mArr[0];
        }

        const T* begin() const {
            return &mArr[0];
        }

        T* end() {
            return &mArr[mMaxSize];
        }

        const T* end() const {
            return &mArr[mMaxSize];
        }
    
        T* mArr;        // 0x0
        s32 mMaxSize;   // 0x4
    };

    template<class T, int C>
    class FixedArray {
    public:
        typedef T Item;

        inline ~FixedArray() {

        }

        T& operator[](int idx) {
            return mArr[idx];
        }

        const T& operator[](int idx) const {
            return mArr[idx];
        }

        int size() const {
            return C;
        }

        T* begin() {
            return &mArr[0];
        }

        const T* begin() const {
            return &mArr[0];
        }

        T* end() {
            return &mArr[C];
        }

        const T* end() const {
            return &mArr[C];
        }

        T mArr[C];          // 0x0
    };

    template<class T>
    class Vector {
    public:
        inline Vector() {
            mCount = 0;
        }

        ~Vector() NO_INLINE {
            
        }

        T::Item& operator[](int idx) {
            return mArray[idx];
        }

        const T::Item& operator[](int idx) const {
            return mArray[idx];
        }

        inline void init(s32 cnt) {
            mArray.mArr = new T::Item[cnt];
            mArray.mMaxSize = cnt;
        }

        int size() const {
            return mCount;
        }

        inline void assign(T::Item &pItem, int where) {
            mArray.mArr[where] = pItem;
        }

        void push_back(const T::Item &rItem) {
            u32 count = mCount;
            mCount++;
            mArray.mArr[count] = rItem;
        }

        T::Item* erase(T::Item* pItem) NO_INLINE {
            if (end() - pItem - 1 > 0) {
                for (T::Item* p = pItem; p + 1 != end(); p++) {
                    *p = *(p + 1);
                }
            }

            mCount--;

            return pItem;
        }

        T::Item* begin() {
            return &mArray[0];
        }

        const T::Item* begin() const {
            return &mArray[0];
        }

        T::Item* end() {
            return &mArray[mCount];
        }

        const T::Item* end() const {
            return &mArray[mCount];
        }

        T mArray;       // 0x0
        s32 mCount;     // 0x8
    };

    template<class T, int S>
    class FixedRingBuffer {
    public:
        class iterator {
        public:
            iterator(T *head, T *tail) {
                mHead = head;
                mTail = tail;
                mEnd = head + S;
            }

            T* mHead;   // 0x0
            T* mTail;   // 0x4
            T* mEnd;    // 0x8
        };
    };
};
