#include "Game/Boss/SkeletalFishBossRail.hpp"
#include "Game/Boss/SkeletalFishBossFunc.hpp"
#include "Game/Boss/SkeletalFishBossRailHolder.hpp"

SkeletalFishBossRail::SkeletalFishBossRail(const char *pName) : LiveActor(pName) {
    _8C = -1;
    mFillUpFlags = nullptr;
    mNamePosIDs = nullptr;
    _98 = 0.0f;
}

void SkeletalFishBossRail::init(const JMapInfoIter &rIter) {
    MR::createSkeletalFishBossRailHolder();
    initRailRider(rIter);
    MR::getJMapInfoArg0WithInit(rIter, &_8C);
    MR::getSkeletalFishBossRailHolder()->add(this);
    createFillUpFlag();
    createFillUpNamePosID();
    makeActorDead();
}

void SkeletalFishBossRail::initAfterPlacement() {
    linkFillUpNamePosID();
}

bool SkeletalFishBossRail::isFillUpPoint(s32 idx) const {
    return mFillUpFlags[idx];
}

s32 SkeletalFishBossRail::getFillUpNamePos(s32 idx) const {
    return mNamePosIDs[idx];
}

void SkeletalFishBossRail::createFillUpFlag() {
    mFillUpFlags = new bool[MR::getRailPointNum(this)];

    for (u32 i = 0; i < MR::getRailPointNum(this); i++) {
        s32 pointArg;
        MR::getRailPointArg0WithInit(this, i, &pointArg);

        if (pointArg == -1) {
            mFillUpFlags[i] = false;
        }
        else {
            mFillUpFlags[i] = true;
        }
    }
}

void SkeletalFishBossRail::createFillUpNamePosID() {
    mNamePosIDs = new s32[MR::getRailPointNum(this)];

    for (s32 i = 0; i < MR::getRailPointNum(this); i++) {
        mNamePosIDs[i] = -1;
    }
}

void SkeletalFishBossRail::linkFillUpNamePosID() {
    for (s32 i = 0; i < MR::getRailPointNum(this); i++) {
        TVec3f railPos;
        MR::calcRailPointPos(&railPos, this, i);
        mNamePosIDs[i] = SkeletalFishBossFunc::getNearestGuardPosID(railPos);
    }
}

SkeletalFishBossRail::~SkeletalFishBossRail() {

}
