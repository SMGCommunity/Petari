#include "Game/Util/BitArray.hpp"
#include "Game/Util.hpp"

namespace MR {
    BitArray::BitArray(int num) :
        mArray(0)
        {
            int arr_size = (num + 7 & (~7)) / 8;
            mArraySize = num;
            mArray = new u8[arr_size];
            MR::zeroMemory(mArray, arr_size);
        }

    bool BitArray::isOn(int bitIdx) const {
        s32 bit = mArray[bitIdx / 8];
        return (bit & (1 << (bitIdx & 0x7))) != 0;
    }

    void BitArray::set(int bitIdx, bool flag) {
        int byteIdx = bitIdx / 8;
        u8* arr = mArray;
        
        if (flag) {
            arr[byteIdx] |= (1 << (bitIdx & 0x7));
        }
        else {
            arr[byteIdx] &= ~(1 << (bitIdx & 0x7));
        }
    }
};