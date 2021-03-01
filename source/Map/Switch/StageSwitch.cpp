#include "Map/Switch/StageSwitch.h"

bool BitFlag128::get(s32 idx) const
{
    return (mFlags[idx / 32] & (1 << (idx % 32))) == 1 << (idx % 32);
}

void BitFlag128::set(s32 bit_index, bool setTrue)
{
    u32 valIdx = bit_index / 32;
    u32& flag = mFlags[valIdx];

    if (setTrue)
    {
        flag |= (1 << (bit_index % 32));
    }
    else
    {
        flag &= ~(1 << (bit_index % 32));
    }
}