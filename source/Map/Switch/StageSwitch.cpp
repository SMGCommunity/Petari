#include "Map/Switch/StageSwitch.h"

// todo -- scheduling issues and a "add" instruction should not be there
bool BitFlag128::get(s32 idx) const
{
    return (((u8)mFlags + (4 * (idx / 32))) & (1 << (idx % 32))) == 1 << (idx % 32);
}