#pragma once

namespace MR {
    template<class T>
    class AssignableArray {
    public:
        AssignableArray() {
            mArr = 0;
            mSize = 0;
        }
        ~AssignableArray() {
            if (mArr) {
                delete[] mArr;
            }
        }

        T** mArr;    // _0
        s32 mSize;  // _4
    };

    template<class T>
    class Vector {
    public:
        Vector() { }
        ~Vector() { }

        T inst;
    };
};