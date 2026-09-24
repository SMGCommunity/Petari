#include "Game/MapObj/SphereRailDash.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"

namespace NrvSphereRailDash {
    NEW_NERVE(SphereRailDashNrvWait, SphereRailDash, Wait);
    NEW_NERVE(SphereRailDashNrvRailMove, SphereRailDash, RailMove);
}  // namespace NrvSphereRailDash

void SphereRailDash_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)2.0f;
}

SphereRailDash::~SphereRailDash() {
}

SphereRailDash::SphereRailDash(const char* pName)
    : LiveActor(pName), _8C(), _90(0.0f, 0.0f, 0.0f), _9C(200.0f), _A0(50.0f), _B0(), _B4(), _B8(), _BC() {
}

void SphereRailDash::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE(SphereRailDash, SphereRailDashNrvWait));
    initRailRider(rIter);
    MR::moveCoordToStartPos(this);
    TVec3f railPosition(MR::getRailPos(this));
    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_SPHERE_DASH, 5, 200.0f, railPosition);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::getJMapInfoArg0NoInit(rIter, &_9C);
    MR::getJMapInfoArg1NoInit(rIter, &_A0);
    appear();
}

bool SphereRailDash::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SPHERE_PLAYER_BINDED) {
        _8C = pSender;
        return true;
    }

    return false;
}

void SphereRailDash::exeWait() {
    if (_8C != nullptr) {
        MR::moveCoordToStartPos(this);
        const TVec3f& rPosition = _8C->mHost->mPosition;
        _90 = rPosition - MR::getRailPos(this);
        TVec3f direction(MR::getRailDirection(this));
        TVec3f velocity(_8C->mHost->mVelocity);
        f32 speed = direction.dot(velocity);

        if (speed < 0.0f) {
            speed = 0.0f;
            _A4 = velocity;
        } else {
            _A4 = velocity - direction * speed;
        }

        f32 endSpeed = _A0;
        f32 duration = 3.0f * MR::getRailTotalLength(this) / (speed + 2.0f * endSpeed);
        f32 acceleration = 2.0f * (endSpeed - speed) / duration;
        _B0 = -acceleration / (2.0f * duration) / 3.0f;
        _B4 = acceleration / 2.0f;
        _B8 = speed;
        _BC = duration + (duration - static_cast< s32 >(duration) > 0.0001f ? 1.0f : 0.0f);

        if (_BC < 0) {
            _BC = 0;
        }

        setNerve(GET_NERVE(SphereRailDash, SphereRailDashNrvRailMove));
    }
}

void SphereRailDash::exeRailMove() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(_8C->mHost);
    }

    if (getNerveStep() + 1 >= _BC) {
        MR::moveCoordToEndPos(this);
    } else {
        f32 time = getNerveStep() + 1;
        MR::setRailCoord(this, time * (_B8 + time * (_B4 + _B0 * time)));
    }

    f32 progress = MR::getRailCoord(this) / MR::getRailTotalLength(this);
    _90 += _A4;
    _A4.mult(0.85f);
    TVec3f previousPosition(_8C->mHost->mPosition);
    TVec3f position = MR::getRailPos(this) + _90.multInLine2(1.0f - progress);
    TVec3f& rVelocity = _8C->mHost->mVelocity;
    rVelocity.set(position - previousPosition);

    if (MR::isRailReachedGoal(this)) {
        _8C->mHost->mPosition.set(position);
        LiveActor* pActor = _8C->mHost;
        f32 endSpeed = _A0;
        pActor->mVelocity.set(MR::getRailDirection(this) * endSpeed);
        _8C->receiveMessage(ACTMES_END_RAIL_DASH, getSensor("body"));
        _8C = nullptr;
        setNerve(GET_NERVE(SphereRailDash, SphereRailDashNrvWait));
    }
}

void SphereRailDash_FORCE_MATCH(TVec3f* pVector, f32 scale) {
    *pVector *= scale;
}
