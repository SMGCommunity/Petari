#include "Game/Boss/SkeletalFishBossInfo.hpp"
#include "Game/Boss/SkeletalFishBoss.hpp"

namespace {
    static const char* sParamFileName = "Param";
    static const char* sParamNameLevelNum = "LevelNum";
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
            s->getGuardStatus(j)->mGuardPosLevel.setZYX(0.0f);
            LevelStatus* cur = &mLevelStatusArray[i];
            cur->getGuardStatus(j)->mGuardWaitLevelID = 0;
        }
    }
}

SkeletalFishBossInfo::GuardStatus::GuardStatus() {

}

// SkeletalFishBossInfo::loadLevelStatus
// SkeletalFishBossInfo::loadGuardStatus

SkeletalFishBossInfo::~SkeletalFishBossInfo() {

}
