#include "Game/MapObj/PowerStarHolder.hpp"
#include "Game/NPC/EventDirector.hpp"

PowerStarHolder::PowerStarHolder(const char *pName) : NameObj(pName) {
    mNumInfos = 0;
    MR::zeroMemory(mInfos, sizeof(mInfos));
}

void PowerStarHolder::init(const JMapInfoIter &) {

}

void PowerStarHolder::registerPowerStar(PowerStar *pStar, int starNum) {
    PowerStarRequestInfo* info = new PowerStarRequestInfo();
    info->mStar = pStar;
    info->mStarNum = starNum;
    info->mIsAppear = false;
    mInfos[mNumInfos] = info;
    mNumInfos++;
}

void PowerStarHolder::requestAppearPowerStar(int starNum, const TVec3f *pPos, bool forceAppear) {
    PowerStarRequestInfo* info = findPowerStarRequestInfo(starNum);
    info->mIsAppear = true;
    PowerStar* star = info->mStar;

    if (pPos != nullptr) {
        star->setDemoAppearPos(*pPos);
    }

    if (forceAppear) {
        star->_11C = true;
        star->appear();
    }
    else {
        star->requestAppear();
    }
}

void PowerStarHolder::appearPowerStarWithoutDemo(int starNum) {
    PowerStarRequestInfo* info = findPowerStarRequestInfo(starNum);
    info->mIsAppear = true;
    PowerStar* star = info->mStar;
    star->offAppearDemo();
    star->appear();
}

PowerStar* PowerStarHolder::getAppearedPowerStar(int starNum) const {
    for (s32 i = 0; i < mNumInfos; i++) {
        PowerStarRequestInfo* info = mInfos[i];

        if (info->mStarNum == starNum && info->mIsAppear) {
            return info->mStar;
        }
    }

    return nullptr;
}

PowerStarRequestInfo* PowerStarHolder::findPowerStarRequestInfo(int starNum) const {
    for (s32 i = 0; i < mNumInfos; i++) {
        PowerStarRequestInfo* info = mInfos[i];

        if (info->mStarNum == starNum) {
            return info;
        }
    }

    return nullptr;
}

namespace MR {
    void registerPowerStar(PowerStar *pStar, int starNum) {
        EventFunction::getPowerStarHolder()->registerPowerStar(pStar, starNum);
    }
};

bool PowerStarFunction::isEndPowerStarAppearDemo(int starNum) {
    return EventFunction::getPowerStarHolder()->getAppearedPowerStar(starNum)->isEndAppearDemo();
}

PowerStar* PowerStarFunction::findPowerStar(int starNum) {
    return EventFunction::getPowerStarHolder()->findPowerStarRequestInfo(starNum)->mStar;
}

PowerStarHolder::~PowerStarHolder() {

}
