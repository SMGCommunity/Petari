#include "Game/MapObj/DriftWood.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

struct SinCosPair {
    f32 sin;
    f32 cos;
};

extern SinCosPair lbl_8060FC80[];

DriftWood::DriftWood(const char* pName)
    : MapObjActor(pName), mRailDirection(0.0f, 0.0f, 1.0f), mRailClipping(0.0f, 0.0f, 0.0f), mVibrateOffset(0.0f, 0.0f, 0.0f), mWaveSoundTimer(0l) {
}

namespace NrvDriftWood {
    NEW_NERVE(DriftWoodNrvWait, DriftWood, Wait);
    NEW_NERVE(DriftWoodNrvWaitLand, DriftWood, Wait);
    NEW_NERVE(DriftWoodNrvVibrateLand, DriftWood, Vibrate);
    NEW_NERVE(DriftWoodNrvVibrateTakeOff, DriftWood, Vibrate);
};  // namespace NrvDriftWood

void DriftWood::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupHitSensor();
    info.setupHitSensorParam(8, 300.0f, TVec3f(0.0f, -60.0f, 0.0f));
    info.setupEffect(nullptr);
    info.setupSound(4);
    info.setupNerve(&NrvDriftWood::DriftWoodNrvWait::sInstance);
    initialize(rIter, info);
    MR::moveCoordAndTransToNearestRailPos(this);
    mRailDirection.set(MR::getRailDirection(this));
    getSensor(nullptr)->setType(ATYPE_EYE);
    mWaveSoundTimer = MR::getRandom(30l, 90l);
    f32 radius = 0.0f;
    MR::calcModelBoundingRadius(&radius, this);
    MR::initAndSetRailClipping(&mRailClipping, this, 100.0f, radius);
    MR::tryCreateMirrorActor(this, mObjectName);
}

void DriftWood::exeWait() {
    if (MR::isFirstStep(this) && !MR::isEffectValid(this, "Ripple")) {
        MR::emitEffect(this, "Ripple");
    }

    MR::moveCoordAndFollowTrans(this, 3.0f);
    MR::rotateVecDegree(&mRailDirection, mGravity, 0.05f);

    tryVibrate();

    if (mWaveSoundTimer < 1) {
        MR::startSound(this, "SE_OJ_DRIFT_WOOD_WAVE");
        mWaveSoundTimer = MR::getRandom(30l, 90l);
    } else {
        mWaveSoundTimer--;
    }
}

void DriftWood::exeVibrate() {
    // FIXME: load order in .scale
    // https://decomp.me/scratch/z1m1A

    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_DRIFT_WOOD_PLAYER_ON");
        mVibrateOffset.zero();
    }

    MR::moveCoord(this, 3.0f);

    f32 step = 30.0f * MR::sinDegree(16.0f * getNerveStep());
    f32 nerveValue = MR::calcNerveValue(this, 45, 0.1f, 1.0f);

    mVibrateOffset.scale((1.0f - nerveValue) * step, mGravity);
    mPosition.add(MR::getRailPos(this), mVibrateOffset);

    if (!tryVibrate()) {
        if (MR::isGreaterEqualStep(this, 45)) {
            mVibrateOffset.zero();

            if (MR::isOnPlayer(this)) {
                setNerve(&NrvDriftWood::DriftWoodNrvWaitLand::sInstance);
            } else {
                setNerve(&NrvDriftWood::DriftWoodNrvWait::sInstance);
            }
        }
    }
}

void DriftWood::connectToScene(const MapObjActorInitInfo&) {
    MR::connectToSceneCollisionMapObj(this);
}

void DriftWood::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, mRailDirection);
    MR::setBaseTRMtx(this, mtx);
}

void DriftWood::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        TVec3f delta;
        delta.sub(pSender->mPosition, pReceiver->mPosition);

        TVec3f upVec;
        MR::calcUpVec(&upVec, this);

        TVec3f result = delta.killElement(upVec);

        f32 radius = pReceiver->mRadius;
        f32 magnitude = result.length();
        f32 threshold = 140.0f + radius;

        if (magnitude > threshold) {
        } else {
            LiveActor* mHost = pReceiver->mHost;
            if (!MR::isInWater(mHost, TVec3f(0.0f, 0.0f, 0.0f))) {
                MR::sendMsgEnemyAttackFlip(pReceiver, pSender);
                MR::startSound(this, "SE_OJ_DRIFT_WOOD_ATTACK");
            }
        }
    } else if (MR::isSensorMapObj(pReceiver) && MR::sendMsgEnemyAttack(pReceiver, pSender) && MR::isOnPlayer(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }
}

bool DriftWood::tryVibrate() {
    if (MR::isOnPlayer(this)) {
        if (isNerve(&NrvDriftWood::DriftWoodNrvVibrateTakeOff::sInstance) || isNerve(&NrvDriftWood::DriftWoodNrvWait::sInstance)) {
            setNerve(&NrvDriftWood::DriftWoodNrvVibrateLand::sInstance);
            return true;
        }
    } else {
        if (isNerve(&NrvDriftWood::DriftWoodNrvWaitLand::sInstance) || isNerve(&NrvDriftWood::DriftWoodNrvWaitLand::sInstance)) {
            setNerve(&NrvDriftWood::DriftWoodNrvVibrateTakeOff::sInstance);
            return true;
        }
    }
    return false;
}
