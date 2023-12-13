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

void AlreadyDoneInfo::init(const char *pInfo, s32 a2, s32 a3) {
    u32 val = MR::getHashCode(pInfo) & 0x7FFF;
    _2 = a2;
    _0 = val;
    _4 = a3; 
}
 
bool AlreadyDoneInfo::isEqual(const AlreadyDoneInfo &otherInfo) const {
    bool ret = 0;

    if ((otherInfo._0 & 0x7FFF) == (_0 & 0x7FFF)) {
        if (otherInfo._2 == _2) {
            if (otherInfo._4 == _4) {
                ret = 1;
            }
        }
    }

    return ret;
}

void AlreadyDoneInfo::set(bool flag) {
    _0 = (flag ? 0 : 0x8000) | _0 & 0x7FFF;
}

AlreadyDoneFlagInGalaxy::AlreadyDoneFlagInGalaxy(int numInfos)
    : mDoneInfos(0), mNumInfos(0), _8(0) {

    mDoneInfos = new AlreadyDoneInfo[numInfos];
    mNumInfos = numInfos;
}

void AlreadyDoneFlagInGalaxy::clear() {
    _8 = 0;
}

#ifdef NON_MATCHING
u32 AlreadyDoneFlagInGalaxy::setupFlag(const char *pName, const JMapInfoIter &rIter, u32 *a3) {
    u32 result;

    s32 linkID = -1;
    MR::getJMapInfoLinkID(rIter, &linkID);
    s32 zoneID = MR::getPlacedZoneId(rIter);

    AlreadyDoneInfo info;
    info.init(pName, zoneID, linkID);

    AlreadyDoneInfo* infs = mDoneInfos;
    AlreadyDoneInfo* lastInfs = &mDoneInfos[_8];

    while (infs != lastInfs && !infs->isEqual(info)) {
        infs++;
    }

    u32 v10 = _8;
    AlreadyDoneInfo* new_infs = &mDoneInfos[v10];

    if (infs != new_infs) {
        result = infs - mDoneInfos;
        *a3 = (infs->_0 >> 15) & 0x1;
    }
    else {
        result = _8;
        _8 = v10 + 1;
        new_infs->_0 = info._0;
        new_infs->_2 = info._2;
        new_infs->_4 = info._4;
        *a3 = 0;
    }

    return result;
}
#endif