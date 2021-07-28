#include "Game/System/AlreadyDoneFlagInGalaxy.h"
#include "Util/HashUtil.h"

AlreadyDoneInfo::AlreadyDoneInfo()
{
    _0 = 0;
    _2 = 0xFFFF;
    _4 = 0xFFFF;

    clear(); 
}

void AlreadyDoneInfo::clear()
{
    _0 = 0;
    _2 = 0xFFFF;
    _4 = 0xFFFF;
}

void AlreadyDoneInfo::init(const char *pInfo, s32 a2, s32 a3)
{
    _0 = MR::getHashCode(pInfo) & 0x7FFF;
    _2 = a2;
    _4 = a3; 
}
 
bool AlreadyDoneInfo::isEqual(const AlreadyDoneInfo &otherInfo) const
{
    bool ret = 0;

    if ((otherInfo._0 & 0x7FFF) == (_0 & 0x7FFF))
    {
        if (otherInfo._2 == _2)
        {
            if (otherInfo._4 == _4)
            {
                ret = 1;
            }
        }
    }

    return ret;
}

void AlreadyDoneInfo::set(bool flag)
{
    _0 = (_0 | 0x8000) & 0x3FFF;
}