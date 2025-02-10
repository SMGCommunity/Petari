#include "Game/Boss/SkeletalFishBossInfo.hpp"
#include "Game/Boss/SkeletalFishBoss.hpp"
#include <cstdio>

namespace {
    static const char* sParamFileName = "Param";
    static const char* sParamNameLevelNum = "LevelNum";
    static const char* sParamNameEnergy = "EnergyLv";
    static const char* sParamNameSpeed = "SpeedLv";
    static const char* sParamNameRailID = "RailIDLv";
    static const char* sParamNameGuardOffset = "GuardOffsetLv";
    static const char* sParamNameGuardAppearNum = "GuardAppearNumLv";
    static const char* sParamNameGuard = "Id";
    static const char* sParamNameGuardPos = "GuardPosLv";
    static const char* sParamNameGuardWait = "GuardWaitLv";
};

SkeletalFishBossInfo::SkeletalFishBossInfo(SkeletalFishBoss *pBoss, s32 levelNum, s32 guardNum, const char *pDesc) : NameObj(pDesc) {
    mFishBoss = pBoss;
    mLevelNum = levelNum;
    mGuardNum = guardNum;
    mLevelStatusArray = nullptr;
    createLevelStatus();
}

void SkeletalFishBossInfo::init(const JMapInfoIter &rIter) {
    volatile const JMapInfo* info = MR::tryCreateCsvParser(mFishBoss, "%s.bcsv", ::sParamFileName);

    if (info != nullptr) {
        /* there is something I am missing here */
        JMapInfoIter what;
        volatile JMapInfoIter level_iter;
        level_iter.mInfo = (JMapInfo*)info;
        what.mInfo = (JMapInfo*)info;
        level_iter._4 = 0;
        what._4 = 0;
        loadLevelStatus(what);
    }
}

SkeletalFishBossInfo::LevelStatus* SkeletalFishBossInfo::getLevelStatus(s32 idx) const {
    return &mLevelStatusArray[idx];
}

void SkeletalFishBossInfo::createLevelStatus() {
    mLevelStatusArray = new LevelStatus[mLevelNum];

    for (s32 i = 0; i < mLevelNum; i++) {
        getLevelStatus(i)->mEnergyLevel = 1;
        getLevelStatus(i)->mSpeedLevel = 20.0f;
        getLevelStatus(i)->mRailIDLevel = 0;
        getLevelStatus(i)->mGuardOffsLevel = 2000.0f;
        getLevelStatus(i)->mGuardAppearNumLevel = 0;
        getLevelStatus(i)->mStatusArray =  new GuardStatus[mGuardNum];

        for (s32 j = 0; j < mGuardNum; j++) {
            LevelStatus* s = &mLevelStatusArray[i];
            s->getGuardStatus(j)->mGuardPosLevel.set(0.0f);
            LevelStatus* cur = &mLevelStatusArray[i];
            cur->getGuardStatus(j)->mGuardWaitLevelID = 0;
        }
    }
}

SkeletalFishBossInfo::GuardStatus::GuardStatus() {

}

void SkeletalFishBossInfo::loadLevelStatus(const JMapInfoIter &rIter) {
    s32 levelNum;

    if (rIter.getValue<s32>(::sParamNameLevelNum, &levelNum)) {
        if (levelNum > mLevelNum) {
            levelNum = mLevelNum;
        }

        for (s32 i = 0; i < levelNum; i++) {
            char nameBuf[0x80];
            snprintf(nameBuf, sizeof(nameBuf), "%s%d", ::sParamNameEnergy, i);
            rIter.getValue<s32>(nameBuf, &getLevelStatus(i)->mEnergyLevel);
            snprintf(nameBuf, sizeof(nameBuf), "%s%d", ::sParamNameSpeed, i);
            rIter.getValue<f32>(nameBuf, &getLevelStatus(i)->mSpeedLevel);
            snprintf(nameBuf, sizeof(nameBuf), "%s%d", ::sParamNameRailID, i);
            rIter.getValue<s32>(nameBuf, &getLevelStatus(i)->mRailIDLevel);
            snprintf(nameBuf, sizeof(nameBuf), "%s%d", ::sParamNameGuardOffset, i);
            rIter.getValue<f32>(nameBuf, &getLevelStatus(i)->mGuardOffsLevel);
            snprintf(nameBuf, sizeof(nameBuf), "%s%d", ::sParamNameGuardAppearNum, i);
            rIter.getValue<s32>(nameBuf, &getLevelStatus(i)->mGuardAppearNumLevel);
            loadGuardStatus(rIter, i, mLevelStatusArray[i].mGuardAppearNumLevel);
        }
    }
}

void SkeletalFishBossInfo::loadGuardStatus(const JMapInfoIter &rIter, s32 levelIdx, s32 guardLevel) {
    LevelStatus* status = &mLevelStatusArray[levelIdx];

    for (s32 i = 0; i < guardLevel; i++) {
        char buf[0x80];
        snprintf(buf, sizeof(buf), "%s%d%s%d", ::sParamNameGuardPos, levelIdx, ::sParamNameGuard, i);
        MR::getJMapInfoV3f(rIter, buf, &status->getGuardStatus(i)->mGuardPosLevel);
        snprintf(buf, sizeof(buf), "%s%d%s%d", ::sParamNameGuardWait, levelIdx, ::sParamNameGuard, i);
        rIter.getValue<s32>(buf, &status->getGuardStatus(i)->mGuardWaitLevelID);
    }
}

SkeletalFishBossInfo::~SkeletalFishBossInfo() {

}
