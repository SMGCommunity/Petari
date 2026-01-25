#include "Game/System/AlreadyDoneFlagInGalaxy.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

AlreadyDoneInfo::AlreadyDoneInfo() {
    _0 = 0;
    _2 = 0xFFFF;
    _4 = 0xFFFF;

    clear();
}

void AlreadyDoneInfo::clear() {
    _0 = 0;
    _2 = 0xFFFF;
    _4 = 0xFFFF;
}

void AlreadyDoneInfo::init(const char* pInfo, s32 a2, s32 a3) {
    u32 val = MR::getHashCode(pInfo) & 0x7FFF;
    _2 = a2;
    _0 = val;
    _4 = a3;
}

bool AlreadyDoneInfo::isEqual(const AlreadyDoneInfo& otherInfo) const {
    bool ret = 0;

    if ((otherInfo._0 & 0x7FFFu) == (_0 & 0x7FFFu)) {
        if (otherInfo._2 == _2) {
            if (otherInfo._4 == _4) {
                ret = 1;
            }
        }
    }

    return ret;
}

void AlreadyDoneInfo::set(bool flag) {
    _0 = (flag ? 0x8000 : 0) | _0 & 0x7FFF;
}

AlreadyDoneFlagInGalaxy::AlreadyDoneFlagInGalaxy(int numInfos) : mDoneInfos(), _8(0) {
    mDoneInfos.init(numInfos);
}

void AlreadyDoneFlagInGalaxy::clear() {
    _8 = 0;
}

u32 AlreadyDoneFlagInGalaxy::setupFlag(const char* pName, const JMapInfoIter& rIter, u32* a3) {
    u32 result;

    s32 linkID = -1;
    MR::getJMapInfoLinkID(rIter, &linkID);
    s32 zoneID = MR::getPlacedZoneId(rIter);

    // There may be a function that got inlined here, because in SMG2 there is a function call here with the
    // contents below this comment
    
     AlreadyDoneInfo info;
     info.init(pName, zoneID, linkID);

     AlreadyDoneInfo* infs = mDoneInfos.begin();
     AlreadyDoneInfo* lastInfs = &mDoneInfos[_8];

     while (infs != lastInfs && !infs->isEqual(info)) {
     infs++;
     }

     AlreadyDoneInfo* new_infs = &mDoneInfos[_8];

     if (infs != new_infs) {
         result = infs - mDoneInfos.begin();
         *a3 = (infs->_0 >> 15) & 0x1;
     } else {
         result = _8++;
         new_infs->_0 = info._0;
         new_infs->_2 = info._2;
         new_infs->_4 = info._4;
         *a3 = 0;
     }

    return result;
}

void AlreadyDoneFlagInGalaxy::updateValue(int a1, u32 a2) {
    mDoneInfos[a1].set(a2 != 0);
}
