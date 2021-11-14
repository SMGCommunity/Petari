#pragma once

namespace MR {
    template<class T>
    class Vector;

    template<class T>
    class AssignableArray {
    public:
        AssignableArray() {
            mArr = 0;
            mMaxSize = 0;
            mCount = 0;
        }
        ~AssignableArray() {
            if (mArr) {
                delete[] mArr;
            }
        }

        typedef T Item;
    
        T* mArr;       // _0
        u32 mMaxSize;   // _4
        u32 mCount;     // _8
    };

    template<class T>
    class Vector {
    public:
        inline Vector() { }
        ~Vector() { }

        T::Item* erase(T::Item *pItem);

        void push_back(const T::Item &rItem) {
            u32 count = mArray.mCount;
            mArray.mCount++;
            mArray.mArr[count] = rItem;
        }

        T::Item* begin() {
            return mArray.mArr;
        }

        T::Item* end() {
            return &mArray.mArr[mArray.mMaxSize];
        }

        T mArray;
    };
};