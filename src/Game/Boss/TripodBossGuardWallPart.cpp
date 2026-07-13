#include "Game/Boss/TripodBossGuardWallPart.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void TripodBossGuardWallPart_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace NrvTripodBossGuardWallPart {
    NEW_NERVE(TripodBossGuardWallPartNrvNonActive, TripodBossGuardWallPart, NonActive);
    NEW_NERVE(TripodBossGuardWallPartNrvDemo, TripodBossGuardWallPart, Demo);
    NEW_NERVE(TripodBossGuardWallPartNrvActive, TripodBossGuardWallPart, Active);
    NEW_NERVE(TripodBossGuardWallPartNrvBreak, TripodBossGuardWallPart, Break);
    NEW_NERVE(TripodBossGuardWallPartNrvRepair, TripodBossGuardWallPart, Repair);
};  // namespace NrvTripodBossGuardWallPart

TripodBossGuardWallPart::TripodBossGuardWallPart(const char* pName) : LiveActor(pName) {
    mHostMtx = nullptr;
    mPlacementAngle = 0.0f;
    mStartTiming = 0;
}

void TripodBossGuardWallPart::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("TripodBossGuardWall", nullptr, false);
    MR::connectToScene(this, MR::MovementType_MapObjDecoration, MR::CalcAnimType_MapObjDecoration, MR::DrawBufferType_TripodBoss, -1);
    initHitSensor(2);
    TVec3f sensorOffs;
    sensorOffs.z = sensorOffs.y = sensorOffs.x = 0.0f;
    MR::initCollisionParts(this, "TripodBossGuardWall", MR::addHitSensor(this, "collision", ATYPE_TRIPODBOSS_GUARD_WALL, 0, 1000.0f, sensorOffs),
                           nullptr);
    initSound(4, false);
    initEffectKeeper(0, "TripodBoss", false);
    MR::invalidateClipping(this);
    MR::startBck(this, "2ndDemo", nullptr);
    MR::setBckFrameAndStop(this, 0.0f);
    initNerve(&NrvTripodBossGuardWallPart::TripodBossGuardWallPartNrvNonActive::sInstance);
    MR::addTripodBossPartsMovement(this);
    makeActorDead();
}

void TripodBossGuardWallPart::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    MR::invalidateCollisionParts(this);
}

void TripodBossGuardWallPart::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "ExplosionM");
}

void TripodBossGuardWallPart::control() {
    mHostMtx->getTransInline(mPosition);
}

void TripodBossGuardWallPart::calcAndSetBaseMtx() {
    // FIXME: float swap in setEulerY
    TPos3f mtx;
    mtx.identity();
    mtx.setEulerY(MR::toRadian(mPlacementAngle));
    mtx.concat(*mHostMtx, mtx);
    MR::setBaseTRMtx(this, mtx);
}

bool TripodBossGuardWallPart::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorType(pReceiver, ATYPE_TRIPODBOSS_GUARD_WALL) && MR::isMsgExplosionAttack(msg)) {
        return requestBreak();
    }

    return false;
}

void TripodBossGuardWallPart::requestStartDemo() {
    setNerve(&NrvTripodBossGuardWallPart::TripodBossGuardWallPartNrvDemo::sInstance);
}

bool TripodBossGuardWallPart::requestBreak() {
    if (MR::isDead(this) || !isNerve(&NrvTripodBossGuardWallPart::TripodBossGuardWallPartNrvActive::sInstance)) {
        return false;
    }

    setNerve(&NrvTripodBossGuardWallPart::TripodBossGuardWallPartNrvBreak::sInstance);
    return true;
}

bool TripodBossGuardWallPart::isEndDemo() const {
    return isNerve(&NrvTripodBossGuardWallPart::TripodBossGuardWallPartNrvActive::sInstance);
}

void TripodBossGuardWallPart::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_TRIPOD_C_COVER_BREAK");
        MR::invalidateCollisionParts(this);
    }

    if (MR::isStep(this, 5)) {
        MR::startBck(this, "2ndDemo", nullptr);
        MR::setBckFrameAndStop(this, 0.0f);
    }

    bool isNearPlayer = !MR::isNearPlayer(this, 1000.0f);

    if (isNearPlayer && MR::isGreaterStep(this, 600)) {
        setNerve(&NrvTripodBossGuardWallPart::TripodBossGuardWallPartNrvRepair::sInstance);
    }
}

void TripodBossGuardWallPart::exeRepair() {
    if (MR::isFirstStep(this)) {
        MR::validateCollisionParts(this);
        MR::startBck(this, "2ndDemo", nullptr);
    }

    if (MR::isStep(this, 38)) {
        MR::startSound(this, "SE_BM_TRIPOD_WALL_UP_STOP");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTripodBossGuardWallPart::TripodBossGuardWallPartNrvActive::sInstance);
    }
}

void TripodBossGuardWallPart::exeActive() {
}

void TripodBossGuardWallPart::exeDemo() {
    if (MR::isGreaterStep(this, mStartTiming)) {
        setNerve(&NrvTripodBossGuardWallPart::TripodBossGuardWallPartNrvRepair::sInstance);
    }
}

void TripodBossGuardWallPart::exeNonActive() {
}

void TripodBossGuardWallPart::setHostMatrix(const TPos3f* pPos) {
    mHostMtx = pPos;
}

void TripodBossGuardWallPart::setPlacementAngle(f32 angle) {
    mPlacementAngle = angle;
    MR::repeatDegree(&mPlacementAngle);
}

void TripodBossGuardWallPart::setStartTiming(s32 timing) {
    mStartTiming = timing;
}
