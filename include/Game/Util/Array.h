#pragma once

namespace MR {
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
        s32 mMaxSize;   // _4
        s32 mCount;     // _8
    };

    template<class T>
    class Vector : public T {
    public:
        inline Vector() { }
        ~Vector() { }

        T::Item* erase(T::Item *pItem); /*{
            if (((&mArr[mCount] - pItem) / 4 - 1) > 0) {
                for (T::Item *i = pItem; (void*)(i + 4) != &mArr[mCount]; i = (i + 4)) { 
                    *i = i[1];
                }
            }

            mCount--;
            return pItem;
        }*/

    };
};