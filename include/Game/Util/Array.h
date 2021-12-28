#pragma once

#include "revolution.h"
#include "Inline.h"

namespace MR {
    template<class T>
    class Vector;

    template<class T>
    class AssignableArray {
    public:
        inline AssignableArray() {
            mArr = 0;
            mMaxSize = 0;
            //mCount = 0;
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

        typedef T Item;
    
        T* mArr;        // _0
        s32 mMaxSize;   // _4
    };

    template<class T>
    class Vector {
    public:
        inline Vector() {
            
        }
        ~Vector();

        inline void assign(T::Item &pItem, int where) {
            mArray.mArr[where] = pItem;
        }

        inline void assignToMax(T::Item &pItem) {
            assign(pItem, mCount);
        }

        T::Item* erase(T::Item *pItem);

        void push_back(const T::Item &rItem) {
            u32 count = mCount;
            mCount++;
            mArray.mArr[count] = rItem;
        }

        T::Item* begin() {
            return mArray.mArr;
        }

        T::Item* end() {
            return &mArray.mArr[mArray.mMaxSize];
        }

        T mArray;       // _0
        u32 mCount;     // _8
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

            T* mHead;   // _0
            T* mTail;   // _4
            T* mEnd;    // _8
        };
    };
};