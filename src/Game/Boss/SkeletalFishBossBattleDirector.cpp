#include "Game/Boss/SkeletalFishBossBattleDirector.hpp"
#include "Game/Boss/SkeletalFishBoss.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/MapObj/SubmarineVolcanoBigColumn.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <cstdio>

SkeletalFishBossBattleDirector::SkeletalFishBossBattleDirector(SkeletalFishBoss* pBoss) : NameObj("スカルシャークボス戦指揮") {
    mFishBoss = pBoss;
    _90 = 0;
    _D4 = 0;
    MR::connectToSceneEnemyMovement(this);

    for (s32 i = 0; i < ARRAY_SIZE(mColumns); i++) {
        mColumns[i] = nullptr;
    }

    for (s32 i = 0; i < ARRAY_SIZE(mGuardModels); i++) {
        ModelObj* guardMdl = new ModelObj("デモ用スカルシャークガード", "SkeletalFishGuard", nullptr, MR::DrawBufferType_Enemy, -2, -2, false);
        mGuardModels[i] = guardMdl;
        mGuardModels[i]->initWithoutIter();
        MR::initLightCtrl(mGuardModels[i]);
        MR::invalidateClipping(mGuardModels[i]);
        mGuardModels[i]->makeActorDead();
    }
}

void SkeletalFishBossBattleDirector::initiate() {
    LiveActorGroup* grp = MR::getGroupFromArray(mFishBoss);

    for (s32 i = 0; i < grp->mObjectCount; i++) {
        if (!strcmp(grp->getActor(i)->mName, "海底火山巨大石柱") || (!strcmp(grp->getActor(i)->mName, "海底火山石柱（小）"))) {
            mColumns[_90] = static_cast< SubmarineVolcanoBigColumn* >(grp->getActor(i));
            _90++;
        }

        if (strcmp(grp->getActor(i)->mName, "羽虫Ｓ")) {
            if (strcmp(grp->getActor(i)->mName, "羽虫Ｌ")) {
                continue;
            }
        }

        _94[_D4] = grp->getActor(i);
        MR::invalidateClipping(_94[_D4]);
        _D4++;
    }

    killBirdLouse();
}

void SkeletalFishBossBattleDirector::startPowerUpDemo1() {
    playGuardAnim("PowerUp1", 2);
}

void SkeletalFishBossBattleDirector::startPowerUpDemo2() {
    playGuardAnim("PowerUp2", 4);
}

void SkeletalFishBossBattleDirector::playGuardAnim(const char* pAnimName, s32 numGuards) {
    for (s32 i = 0; i < numGuards; i++) {
        char animName[0x80];
        snprintf(animName, sizeof(animName), "%s%c", pAnimName, i + 'A');
        mGuardModels[i]->makeActorAppeared();
        MR::requestMovementOn(mGuardModels[i]);
        MR::startBck(mGuardModels[i], animName, nullptr);
        MR::startSound(mGuardModels[i], "SE_BM_SKL_GUARD_APPEAR");
    }
}

void SkeletalFishBossBattleDirector::movement() {
}

void SkeletalFishBossBattleDirector::tryColumnCollision(HitSensor* pSensor) {
    for (s32 i = 0; i < _90; i++) {
        CollisionParts* parts = MR::getCollisionParts(mColumns[i]);

        if (parts != nullptr) {
            HitInfo hitInfo;
            if (parts->checkStrikeBall(&hitInfo, 1, pSensor->mPosition, pSensor->mRadius, false, nullptr)) {
                if (hitInfo.mParentTriangle.mSensor != nullptr) {
                    MR::sendMsgEnemyAttack(hitInfo.mParentTriangle.mSensor, pSensor);
                    return;
                }
            }
        }
    }
}

void SkeletalFishBossBattleDirector::pauseOffCast() {
    LiveActorGroup* group = MR::getGroupFromArray(mFishBoss);

    for (s32 i = 0; i < group->mObjectCount; i++) {
        MR::requestMovementOn(group->getActor(i));
        if (MR::isExistEffectKeeper(group->getActor(i))) {
            MR::pauseOffEffectAll(group->getActor(i));
        }
    }

    for (s32 i = 0; i < _90; i++) {
        mColumns[i]->pauseOff();
    }
}

void SkeletalFishBossBattleDirector::endPowerUpDemo1() {
    killGuard();
}

void SkeletalFishBossBattleDirector::endPowerUpDemo2() {
    killGuard();
}

void SkeletalFishBossBattleDirector::killGuard() {
    for (s32 i = 0; i < ARRAY_SIZE(mGuardModels); i++) {
        mGuardModels[i]->makeActorDead();
    }
}

void SkeletalFishBossBattleDirector::appearBirdLouse() {
    for (s32 i = 0; i < _D4; i++) {
        _94[i]->endClipped();
    }
}

void SkeletalFishBossBattleDirector::killBirdLouse() {
    for (s32 i = 0; i < _D4; i++) {
        _94[i]->startClipped();
    }
}

SkeletalFishBossBattleDirector::~SkeletalFishBossBattleDirector() {
}
