#include "Game/MapObj/LotusLeaf.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "math_types.hpp"
#include "revolution/types.h"

namespace NrvLotusLeaf {
    NEW_NERVE(HostTypeWait, LotusLeaf, Wait);
    NEW_NERVE(HostTypeShakeOnPlayer, LotusLeaf, Shake);
    NEW_NERVE(HostTypeShake, LotusLeaf, Shake);
    NEW_NERVE(HostTypeWaitPlayerOn, LotusLeaf, WaitPlayerOn);
}

LotusLeaf::LotusLeaf(const char* pName) : LiveActor(pName) {
    _8C.zero();
    //_8C.z = 0;
    _98 = 0.0f;
    _9C = 0.0f;
}

LotusLeaf::~LotusLeaf() {}

void LotusLeaf::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    _8C.set(mPosition);
    initModelManagerWithAnm("LotusLeaf", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    HitSensor* sensor = MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "LotusLeaf", sensor, nullptr);
    initSound(4, false);
    initNerve(&NrvLotusLeaf::HostTypeWait::sInstance);
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostSRT(this, "LotusLeafRipple", &_8C, nullptr, nullptr);
    MR::initShadowVolumeCylinder(this, 300.0f);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void LotusLeaf::exeWait() {
    convergeToInitPos();
    if (MR::isFirstStep(this)) {
        MR::deleteEffect(this, "LotusLeafRipple");
    } 
    if (MR::isOnPlayer(this)) setNerve(&NrvLotusLeaf::HostTypeShakeOnPlayer::sInstance);
}

void LotusLeaf::exeWaitPlayerOn() {
    if (_8C.y < mPosition.y) {
        convergeToInitPos();
    }
    if (!MR::isOnPlayer(this)) {
        setNerve(&NrvLotusLeaf::HostTypeWait::sInstance);
    }
}

//Todo: Match. Decomp.me: https://decomp.me/scratch/zsY3a
void LotusLeaf::exeShake() {
    if (MR::isFirstStep(this)) {
        _98 = 1.5f;
        _9C = 17.0f;
        MR::startSound(this, "SE_OJ_LOTUS_LEAF_WAVE", -1, -1);
        MR::emitEffect(this, "LotusLeafRipple");
    }
    f32 v1 = 6.2831855f / _9C;

    f32 value = JMath::sSinCosTable.cosLapRad( v1 * getNerveStep());
    //* 0.95f + -mPosition.y
    _9C = -_98 * 0.65f;
    _98 = v1;
    mVelocity.y = _9C - mPosition.y;
    if (value <=  _8C.y) {
        _8C.zero();
    }
    if (isNerve(&NrvLotusLeaf::HostTypeShake::sInstance)) {
        if (MR::isOnPlayer(this)) {
            LiveActor::setNerve(&NrvLotusLeaf::HostTypeShakeOnPlayer::sInstance);
        }
    } else if (!MR::isOnPlayer(this)) {
        setNerve(&NrvLotusLeaf::HostTypeShake::sInstance);
        return;
    }
    f32 f_a = value - mVelocity.y;
    if ( MR::isNearZero(f_a, 0.000099999997f) && MR::isNearZero(f_a, 0.000099999997f)) {
        mVelocity.zero();
        if (isNerve(&NrvLotusLeaf::HostTypeShakeOnPlayer::sInstance)) {
            setNerve(&NrvLotusLeaf::HostTypeWaitPlayerOn::sInstance);
        } else {
            setNerve(&NrvLotusLeaf::HostTypeWait::sInstance);
        }
    }
}

void LotusLeaf::convergeToInitPos() {
    if (mPosition.y >= _8C.y) {
        mVelocity.y -= 0.04f;
        if (mPosition.y + mVelocity.y <= _9C) {
            mVelocity.zero();
        }
    } else {
        mVelocity.y += 0.04f;
        f32 float_1 = mPosition.y + mVelocity.y;
        if (float_1 <= _9C) {
            mVelocity.zero();
        }
    }
}
