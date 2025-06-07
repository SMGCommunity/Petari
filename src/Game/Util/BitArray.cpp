#include "Game/Util/BitArray.hpp"
#include "Game/Util/MemoryUtil.hpp"

namespace MR {
    BitArray::BitArray(int bitNum) :
        mArray(NULL),
        mArraySize(bitNum)
    {
        int byteNum = (bitNum + 7 & ~7) / 8;

        mArray = new u8[byteNum];

        MR::zeroMemory(mArray, byteNum);
    }

    bool BitArray::isOn(int bitIdx) const {
        u8 byte = mArray[bitIdx / 8];

        return (byte & (1 << (bitIdx & 0x7))) != 0;
    }

    void BitArray::set(int bitIdx, bool isOn) {
        int byteIdx = bitIdx / 8;
        u8* pArray = mArray;

        if (isOn) {
            pArray[byteIdx] |= (1 << (bitIdx & 0x7));
        }
        else {
            pArray[byteIdx] &= ~(1 << (bitIdx & 0x7));
        }
    }
};
