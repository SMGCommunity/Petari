#pragma once

#include <revolution.h>

namespace MR {
    
    class BitArray {
    public:
        BitArray(int);

        bool isOn(int) const;
        void set(int, bool);

        int size() const {
            return mArraySize;
        }

        u8* mArray;         // 0x0
        int mArraySize;     // 0x4
    };
};