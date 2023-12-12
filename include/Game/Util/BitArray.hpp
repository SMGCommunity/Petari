#pragma once

#include <revolution.h>

namespace MR {
    
    class BitArray {
    public:
        BitArray(int);

        bool isOn(int) const;
        void set(int, bool);


        u8* mArray;         // _0
        int mArraySize;     // _4
    };
};