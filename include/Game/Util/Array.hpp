#pragma once

#include "revolution.h"
#include "Inline.hpp"

namespace MR {
    template<class T>
    class Vector;

    template<class T>
    class AssignableArray {
    public:
        inline AssignableArray() {
            mArr = 0;
            mMaxSize = 0;
        }

        inline void init(s32 cnt) {
            mArr = new T[cnt];
            mMaxSize = cnt;
        }

        inline void assign(T what, s32 where) {
            mArr[where] = what;
        }

        inline T* begin() {
            return mArr;
        }

        inline T* end() {
            return &mArr[mMaxSize];
        }

        ~AssignableArray() {
            if (mArr) {
                delete[] mArr;
            }
        }

        inline T* getAtIdx(u32 idx) {
            return mArr[idx];
        }

        typedef T Item;
    
        T* mArr;        // 0x0
        s32 mMaxSize;   // 0x4
    };

    template<class T, int C>
    class FixedArray {
    public:
        typedef T Item;

        inline ~FixedArray() {

        }

        inline int getSize() {
            return C;
        }

        T mArr[C];          // 0x0
    };

    template<class T>
    class Vector {
    public:
        inline Vector() {
            mCount = 0;
        }


        inline ~Vector() {
            
        }

        inline s32 getCount() {
            return mCount;
        }

        inline void init(s32 cnt) {
            mArray.mArr = new T::Item[cnt];
            mArray.mMaxSize = cnt;
        }

        inline void assign(T::Item &pItem, int where) {
            mArray.mArr[where] = pItem;
        }

        inline void assignToMax(T::Item &pItem) {
            assign(pItem, mCount);
        }

        T::Item* erase(T::Item *pItem);

        void push_back(const T::Item &rItem) NO_INLINE {
            u32 count = mCount;
            mCount++;
            mArray.mArr[count] = rItem;
        }

        T::Item begin() {
            return mArray.mArr[0];
        }

        T::Item end() {
            return mArray.mArr[mCount];
        }

        inline T::Item operator[](int idx) const {
            return mArray.mArr[idx];
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