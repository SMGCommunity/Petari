#include "Game/Boss/SkeletalFishGuardHolder.hpp"
#include "Game/Boss/SkeletalFishBoss.hpp"
#include "Game/Boss/SkeletalFishBossFunc.hpp"
#include "Game/Boss/SkeletalFishBossInfo.hpp"
#include "Game/Boss/SkeletalFishGuard.hpp"
#include "Game/Boss/SkeletalFishBossRail.hpp"

SkeletalFishGuardHolder::SkeletalFishGuardHolder(SkeletalFishBoss *pBoss, s32 numGuards, const char *pName) : NameObj(pName) {
    mFishBoss = pBoss;
    mBossRail = nullptr;
    mNumGuards = numGuards;
    mGuards = nullptr;
    _1C = 0.0f;
    _20 = 1;
    createAndInitGuard();
}

void SkeletalFishGuardHolder::movement() {
    if (_20) {
        checkRailChangedAndReset();
        s32 appearableGuard = countAppearableGuard();
        if (appearableGuard > 0) {
            checkDistanceAndAppear(appearableGuard);
        }
    }
}

void SkeletalFishGuardHolder::forceAppearAll() {
    SkeletalFishGuard* guard;  //
    SkeletalFishBossInfo::LevelStatus* lvl;
    s32 guardWaitLvlId;  // r28
    s32 appearableGuard = countAppearableGuard();  // r27
    s32 i;  // r26

    for (i = 0; i < mNumGuards; i++) {
        if (MR::isDead(mGuards[i])) {
            guardWaitLvlId = mFishBoss->getLevelStatus()->mStatusArray[i].mGuardWaitLevelID;
            lvl = mFishBoss->getLevelStatus();
            guard = mGuards[i];
            guard->_94.set(lvl->mStatusArray[i].mGuardPosLevel);
            guard->_A0 = guardWaitLvlId;
            mGuards[i]->appearForce();

            if (--appearableGuard <= 0) {
                break;
            }
        }
    }
}

void SkeletalFishGuardHolder::forceKillAll() {
    for (s32 i = 0; i < mNumGuards; i++) {
        mGuards[i]->makeActorDead();
    }
}

void SkeletalFishGuardHolder::killAll() {
    for (s32 i = 0; i < mNumGuards; i++) {
        mGuards[i]->killNaturally();
    }
}

void SkeletalFishGuardHolder::notifyAttack(SkeletalFishGuard *pGuard) {
    for (s32 i = 0; i < mNumGuards; i++) {
        SkeletalFishGuard* guard = mGuards[i];

        if (guard != pGuard) {
            guard->waitAttack(120);
        }
    }
}

void SkeletalFishGuardHolder::invalidate() {
    _20 = 0;
}

void SkeletalFishGuardHolder::validate() {
    if (!_20) {
        _20 = 1;
        _1C = MR::getRailCoord(mFishBoss->getCurrentBossRail());
    }
}

void SkeletalFishGuardHolder::checkRailChangedAndReset() {
    if (mBossRail != mFishBoss->getCurrentBossRail()) {
        _1C = MR::getRailCoord(mFishBoss->getCurrentBossRail());
        mBossRail = (SkeletalFishBossRail*)mFishBoss->getCurrentBossRail();
    }
}

s32 SkeletalFishGuardHolder::countAppearableGuard() const {
    SkeletalFishBossInfo::LevelStatus* lvl = mFishBoss->mBossInfo->getLevelStatus(mFishBoss->_110);
    s32 guardCount = lvl->mGuardAppearNumLevel - countGuardAlive();

    if (guardCount < 0) {
        guardCount = 0;
    }

    return guardCount;
}

s32 SkeletalFishGuardHolder::countGuardAlive() const {
    s32 aliveGuard = 0;

    for (s32 i = 0; i < mNumGuards; i++) {
        if (!MR::isDead(mGuards[i])) {
            aliveGuard++;
        }
    }

    return aliveGuard;
}

void SkeletalFishGuardHolder::checkDistanceAndAppear(s32 numGuards) {
    f32 len, v7, v9;
    
    f32 guardOffsLvl = mFishBoss->mBossInfo->getLevelStatus(mFishBoss->_110)->mGuardOffsLevel;
    f32 coord = MR::getRailCoord(mBossRail);

    if (coord < _1C) {
        coord = coord + MR::getRailTotalLength(mBossRail);
    }

    for (s32 i = 0; i < MR::getRailPointNum(mBossRail); i++) {
        if (mBossRail->isFillUpPoint(i)) {
            v7 = (MR::getRailPointCoord(mBossRail, i) - guardOffsLvl);
            len = MR::getRailTotalLength(mBossRail);
            v9 = MR::modAndSubtract(v7, len, 0.0f);

            if (v9 < _1C) {
                v9 = (v9 + (MR::getRailTotalLength(mBossRail)));
            }

            if (v9 < coord) {
                appearGuard(numGuards, i);
                break;
            }
        }
    }

    _1C = MR::getRailCoord(mBossRail);
}

void SkeletalFishGuardHolder::appearGuard(s32 numGuards, s32 startGuard) {
    SkeletalFishGuard* guard;
    SkeletalFishBossInfo::LevelStatus* level;
    s32 pos = mBossRail->getFillUpNamePos(startGuard);
    s32 waitID;

    TVec3f guardPos;
    if (pos >= 0 && SkeletalFishBossFunc::copyGuardPos(&guardPos, pos)) {
        for (s32 i = 0; i < mNumGuards; i++) {
            if (MR::isDead(mGuards[i])) {
                mGuards[i]->mPosition.set(guardPos);
                waitID = mFishBoss->getLevelStatus()->mStatusArray[i].mGuardWaitLevelID;
                level = mFishBoss->getLevelStatus();
                guard = mGuards[i];
                guard->_94.set(level->mStatusArray[i].mGuardPosLevel);
                guard->_A0 = waitID;
                mGuards[i]->appearNaturally();

                if (--numGuards <= 0) {
                    break;
                }
            }
        }
    }
}

SkeletalFishGuardHolder::~SkeletalFishGuardHolder() {

}

void SkeletalFishGuardHolder::createAndInitGuard() {
    mGuards = new SkeletalFishGuard*[mNumGuards];

    for (s32 i = 0; i < mNumGuards; i++) {
        mGuards[i] = new SkeletalFishGuard(mFishBoss, "スカルシャークガード");
        mGuards[i]->initWithoutIter();
    }
}