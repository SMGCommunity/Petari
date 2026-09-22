#include "Game/Boss/SkeletalFishBossInfo.hpp"
#include "Game/Boss/SkeletalFishBoss.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <cstdio>

void SkeletalFishBossInfo_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

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
};  // namespace

SkeletalFishBossInfo::SkeletalFishBossInfo(SkeletalFishBoss* pBoss, s32 levelNum, s32 guardNum, const char* pName)
    : NameObj(pName), mFishBoss(pBoss), mLevelNum(levelNum), mGuardNum(guardNum), mLevelStatusArray() {
    createLevelStatus();
}

void SkeletalFishBossInfo::init(const JMapInfoIter& rIter) {
    const JMapInfo* pCsvParser = MR::tryCreateCsvParser(mFishBoss, "%s.bcsv", ::sParamFileName);

    if (pCsvParser != nullptr) {
        loadLevelStatus(pCsvParser->begin());
    }
}

SkeletalFishBossInfo::LevelStatus* SkeletalFishBossInfo::getLevelStatus(s32 idx) const {
    return mLevelStatusArray + idx;
}

void SkeletalFishBossInfo::createLevelStatus() {
    mLevelStatusArray = new LevelStatus[mLevelNum];

    for (s32 i = 0; i < mLevelNum; i++) {
        getLevelStatus(i)->mEnergyLevel = 1;
        getLevelStatus(i)->mSpeedLevel = 20.0f;
        getLevelStatus(i)->mRailIDLevel = 0;
        getLevelStatus(i)->mGuardOffsLevel = 2000.0f;
        getLevelStatus(i)->mGuardAppearNumLevel = 0;
        getLevelStatus(i)->mStatusArray = new GuardStatus[mGuardNum];

        for (s32 j = 0; j < mGuardNum; j++) {
            mLevelStatusArray[i].getGuardStatus(j)->mGuardPosLevel.zero();
            mLevelStatusArray[i].getGuardStatus(j)->mGuardWaitLevelID = 0;
        }
    }
}

SkeletalFishBossInfo::GuardStatus::GuardStatus() {
}

void SkeletalFishBossInfo::loadLevelStatus(const JMapInfoIter& rIter) {
    s32 levelNum;

    if (!rIter.getValue< s32 >(::sParamNameLevelNum, &levelNum)) {
        return;
    }

    if (levelNum > mLevelNum) {
        levelNum = mLevelNum;
    }

    for (s32 i = 0; i < levelNum; i++) {
        char paramName[128];

        snprintf(paramName, sizeof(paramName), "%s%d", ::sParamNameEnergy, i);
        rIter.getValue< s32 >(paramName, &getLevelStatus(i)->mEnergyLevel);

        snprintf(paramName, sizeof(paramName), "%s%d", ::sParamNameSpeed, i);
        rIter.getValue< f32 >(paramName, &getLevelStatus(i)->mSpeedLevel);

        snprintf(paramName, sizeof(paramName), "%s%d", ::sParamNameRailID, i);
        rIter.getValue< s32 >(paramName, &getLevelStatus(i)->mRailIDLevel);

        snprintf(paramName, sizeof(paramName), "%s%d", ::sParamNameGuardOffset, i);
        rIter.getValue< f32 >(paramName, &getLevelStatus(i)->mGuardOffsLevel);

        snprintf(paramName, sizeof(paramName), "%s%d", ::sParamNameGuardAppearNum, i);
        rIter.getValue< s32 >(paramName, &getLevelStatus(i)->mGuardAppearNumLevel);

        loadGuardStatus(rIter, i, mLevelStatusArray[i].mGuardAppearNumLevel);
    }
}

void SkeletalFishBossInfo::loadGuardStatus(const JMapInfoIter& rIter, s32 levelIdx, s32 guardLevel) {
    LevelStatus* pStatus = getLevelStatus(levelIdx);

    for (s32 i = 0; i < guardLevel; i++) {
        char paramName[128];

        snprintf(paramName, sizeof(paramName), "%s%d%s%d", ::sParamNameGuardPos, levelIdx, ::sParamNameGuard, i);
        MR::getJMapInfoV3f(rIter, paramName, &pStatus->getGuardStatus(i)->mGuardPosLevel);

        snprintf(paramName, sizeof(paramName), "%s%d%s%d", ::sParamNameGuardWait, levelIdx, ::sParamNameGuard, i);
        rIter.getValue< s32 >(paramName, &pStatus->getGuardStatus(i)->mGuardWaitLevelID);
    }
}
