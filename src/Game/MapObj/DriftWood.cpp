#include "Game/MapObj/DriftWood.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

struct SinCosPair {
    f32 sin;
    f32 cos;
};

extern SinCosPair lbl_8060FC80[];

DriftWood::DriftWood(const char* pName) : MapObjActor(pName), 
mRailDirection(0.0f, 0.0f, 1.0f),
mRailClipping(0.0f, 0.0f, 0.0f),
mVibrateOffset(0.0f, 0.0f, 0.0f),
mWaveSoundTimer(0l)
{}

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
    getSensor(nullptr)->setType(127);
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
        MR::startSound(this, "SE_OJ_DRIFT_WOOD_WAVE", -1, -1);
        mWaveSoundTimer = MR::getRandom(30l, 90l);
    } else {
        mWaveSoundTimer--;
    }
}

void DriftWood::exeVibrate() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_DRIFT_WOOD_PLAYER_ON", -1, -1);
        mVibrateOffset.zero();
    }

    MR::moveCoord(this, 3.0f);

    f32 step = 16.0f * getNerveStep();

    if (step < 0.0f) {
        s32 tempIndex = -45.511112f * step;
        u16 index = tempIndex & 0x3FFF;
        step = -lbl_8060FC80[index].sin;
    } else {
        s32 tempIndex = 45.511112f * step;
        u16 index = tempIndex & 0x3FFF;
        step = lbl_8060FC80[index].sin;
    }

    step = 30.0f * step;
    
    f32 nerveValue = MR::calcNerveValue(this, 45, 0.1f, 1.0f);
    
    step = (1.0f - nerveValue) * step;
    
    mVibrateOffset.scale(step, mGravity);

    JMathInlineVEC::PSVECAdd(&MR::getRailPos(this), &mVibrateOffset, &mPosition);

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
        JMathInlineVEC::PSVECSubtract2(&pSender->mPosition, &pReceiver->mPosition, &delta);
        
        TVec3f upVec;
        MR::calcUpVec(&upVec, this);

        TVec3f result;
        JMAVECScaleAdd(&upVec, &delta, &result, -upVec.dot(delta));

        float radius = pReceiver->mRadius;
        float magnitude = PSVECMag(&result);
        float threshold = 140.0f + radius;

        if (magnitude > threshold) {
        } else {
            LiveActor* mHost = pReceiver->mHost;
            if (!MR::isInWater(mHost, TVec3f(0.0f, 0.0f, 0.0f))) {
                MR::sendMsgEnemyAttackFlip(pReceiver, pSender);
                MR::startSound(this, "SE_OJ_DRIFT_WOOD_ATTACK", -1, -1);
            }
        }
    } else if (MR::isSensorMapObj(pReceiver) && MR::sendMsgEnemyAttack(pReceiver, pSender) && MR::isOnPlayer(this)) {
        MR::tryRumblePadMiddle(this, 0);
    }
}

bool DriftWood::tryVibrate() {
    if (MR::isOnPlayer(this)) {
        if (isNerve(&NrvDriftWood::DriftWoodNrvVibrateTakeOff::sInstance) ||
            isNerve(&NrvDriftWood::DriftWoodNrvWait::sInstance)) {
            setNerve(&NrvDriftWood::DriftWoodNrvVibrateLand::sInstance);
            return true;
        }
    } else {
        if (isNerve(&NrvDriftWood::DriftWoodNrvWaitLand::sInstance) ||
            isNerve(&NrvDriftWood::DriftWoodNrvWaitLand::sInstance)) {
            setNerve(&NrvDriftWood::DriftWoodNrvVibrateTakeOff::sInstance);
            return true;
        }
    }
    return false;
}
