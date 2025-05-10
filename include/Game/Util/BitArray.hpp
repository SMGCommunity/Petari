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

    private:
        /* 0x0 */ u8* mArray;
        /* 0x4 */ s32 mArraySize;
    };
};
