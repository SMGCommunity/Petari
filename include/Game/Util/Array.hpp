#pragma once

#include "Inline.hpp"
#include <revolution.h>

namespace MR {
    template<class T>
    class AssignableArray {
    public:
        typedef T Item;

        AssignableArray() {
            mArr = 0;
            mMaxSize = 0;
        }

        ~AssignableArray() {
            if (mArr != NULL) {
                delete[] mArr;
            }
        }

        T& operator[](int idx) {
            return mArr[idx];
        }

        const T& operator[](int idx) const {
            return mArr[idx];
        }

        void init(s32 cnt) {
            mArr = new T[cnt];
            mMaxSize = cnt;
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

        /* 0x0 */ T* mArr;
        /* 0x4 */ s32 mMaxSize;
    };

    template<class T, int N>
    class FixedArray {
    public:
        typedef T Item;

        T& operator[](int idx) {
            return mArr[idx];
        }

        const T& operator[](int idx) const {
            return mArr[idx];
        }

        int size() const {
            return N;
        }

        T* begin() {
            return &mArr[0];
        }

        const T* begin() const {
            return &mArr[0];
        }

        T* end() {
            return &mArr[N];
        }

        const T* end() const {
            return &mArr[N];
        }

    private:
        /* 0x0 */ T mArr[N];
    };

    template<class T>
    class Vector {
    public:
        Vector() {
            mCount = 0;
        }

        ~Vector() {
            
        }

        T::Item& operator[](int idx) {
            return mArray[idx];
        }

        const T::Item& operator[](int idx) const {
            return mArray[idx];
        }

        void init(s32 cnt) {
            mArray.mArr = new T::Item[cnt];
            mArray.mMaxSize = cnt;
        }

        int size() const {
            return mCount;
        }

        void push_back(const T::Item &rItem) {
            u32 index = mCount;

            mCount++;
            mArray[index] = rItem;
        }

        T::Item* erase(T::Item* pIter) NO_INLINE {
            if (end() - pIter - 1 > 0) {
                for (T::Item* p = pIter; p + 1 != end(); p++) {
                    *p = *(p + 1);
                }
            }

            mCount--;

            return pIter;
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

        /* 0x0 */ T mArray;
        /* 0x? */ s32 mCount;
    };

    template<class T, int S>
    class FixedRingBuffer {
    public:
        class iterator {
        public:
            iterator(T* head, T* tail) {
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
