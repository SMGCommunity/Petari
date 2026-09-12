#include "Game/System/AlreadyDoneFlagInGalaxy.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include <algorithm>

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

    if ((otherInfo.mask()) == (mask())) {
        if (otherInfo._2 == _2) {
            if (otherInfo._4 == _4) {
                ret = 1;
            }
        }
    }

    return ret;
}

void AlreadyDoneInfo::set(bool flag) {
    _0 = (flag ? 0x8000 : 0) | mask();
}

u16 AlreadyDoneInfo::mask() const {
    return _0 & 0x7FFF;
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

    AlreadyDoneInfo info;
    info.init(pName, zoneID, linkID);

    AlreadyDoneInfo* it = std::find(mDoneInfos.begin(), &mDoneInfos[_8], info);

    AlreadyDoneInfo* end = &mDoneInfos[_8];

    if (it != end) {
        result = it - mDoneInfos.begin();
        *a3 = (it->_0 >> 15) & 0x1;
    } else {
        result = _8++;
        *end = info;
        *a3 = 0;
    }

    return result;
}

void AlreadyDoneFlagInGalaxy::updateValue(int a1, u32 a2) {
    mDoneInfos[a1].set(a2 != 0);
}
