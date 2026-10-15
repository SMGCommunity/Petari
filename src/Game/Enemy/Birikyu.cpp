#include "Game/Enemy/Birikyu.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    static const f32 sHitRadiusWithFace = 50.0f;
    static const f32 sHitRadiusWithoutFace = 120.0f;
    static const s32 sAttackTime = 90;
    static const f32 sShadowRadiusRatio = 0.8f;
    static const f32 sMoveRadius = 400.0f;
    static const f32 sMoveSpeedDefault = 10.0f;
};  // namespace

namespace NrvBirikyu {
    NEW_NERVE(HostTypeMove, Birikyu, Move);
    NEW_NERVE(HostTypeMoveCircle, Birikyu, MoveCircle);
    NEW_NERVE(HostTypeAttack, Birikyu, Attack);
    NEW_NERVE(HostTypeAttackWait, Birikyu, AttackWait);
    NEW_NERVE(HostTypeWaitAtEdge, Birikyu, WaitAtEdge);
    NEW_NERVE(HostTypeStopPointing, Birikyu, StopPointing);
};  // namespace NrvBirikyu

Birikyu::Birikyu(const char* pName)
    : LiveActor(pName), mObjectName(), mRailClipCenter(gZeroVec), mOrbitCenter(gZeroVec), mHasShadow(), mConnected(), mUpVec(0.0f, 1.0f, 0.0f),
      mFront(0.0f, 0.0f, 1.0f), mOrbitAngle(), mMoveSpeed(::sMoveSpeedDefault) {
}

void Birikyu::init(const JMapInfoIter& rIter) {
    MR::getObjectName(&mObjectName, rIter);
    initModelManagerWithAnm(mObjectName, nullptr, false);
    MR::connectToSceneEnemy(this);
    initFromJmpArgs(rIter);
    initRail(rIter);
    MR::initDefaultPos(this, rIter);
    initCollision();
    initShadow();
    initEffectKeeper(0, nullptr, false);
    MR::setGroupClipping(this, rIter, 64);
    MR::joinToGroupArray(this, rIter, nullptr, 64);
    initSound(4, false);
    TVec3f offset(0.0f);
    MR::initStarPointerTarget(this, 100.0f, offset);

    if (mConnected) {
        initNerve(GET_NERVE(Birikyu, HostTypeMove));
    } else {
        initNerve(GET_NERVE(Birikyu, HostTypeMoveCircle));
    }

    appear();
}

void Birikyu::initAfterPlacement() {
    if (mConnected) {
        MR::moveCoordAndTransToNearestRailPos(this);
    } else {
        mOrbitCenter.set(mPosition);
        TPos3f matrix;
        matrix.identity();
        MR::makeMtxRotate(matrix.toMtxPtr(), mRotation);
        matrix.getYDir(mUpVec);
        MR::normalize(&mUpVec);
        matrix.getZDir(mFront);
        MR::normalize(&mFront);
        mPosition.set(mOrbitCenter + mFront * ::sMoveRadius);
    }
}

void Birikyu::appear() {
    LiveActor::appear();
    MR::emitEffect(this, mObjectName);
}

f32 Birikyu::getHitRadius() const {
    return ::sHitRadiusWithoutFace;
}

char* Birikyu::getCenterJointName() const {
    return "Root";
}

void Birikyu::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver) || MR::isSensorEnemy(pReceiver)) {
        if (MR::sendMsgEnemyAttackElectric(pReceiver, pSender)) {
            MR::sendMsgToGroupMember(ACTMES_GROUP_MOVE_STOP, this, getSensor("body"), "body");
            setNerve(GET_NERVE(Birikyu, HostTypeAttack));
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool Birikyu::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

bool Birikyu::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_GROUP_MOVE_STOP) {
        bool bool4 = isNerve(GET_NERVE(Birikyu, HostTypeMove)) || isNerve(GET_NERVE(Birikyu, HostTypeMoveCircle));

        if (bool4) {
            setNerve(GET_NERVE(Birikyu, HostTypeAttackWait));

            return true;
        }
    }

    if (msg == ACTMES_GROUP_MOVE_START) {
        bool bool5 = isNerve(GET_NERVE(Birikyu, HostTypeMove)) || isNerve(GET_NERVE(Birikyu, HostTypeMoveCircle));

        if (!bool5) {
            goMove();

            return true;
        }
    }

    return false;
}

void Birikyu::startClipped() {
    LiveActor::startClipped();
    MR::forceDeleteEffect(this, mObjectName);
}

void Birikyu::endClipped() {
    LiveActor::endClipped();
    MR::emitEffect(this, mObjectName);
}

void Birikyu::initFromJmpArgs(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &mMoveSpeed);
    s32 num = -1;
    MR::getJMapInfoArg2NoInit(rIter, &num);
    mHasShadow = num == true;
}

void Birikyu::initRail(const JMapInfoIter& rIter) {
    mConnected = MR::isConnectedWithRail(rIter);
    if (mConnected) {
        initRailRider(rIter);
        MR::initAndSetRailClipping(&mRailClipCenter, this);
    }
}

void Birikyu::initCollision() {
    initHitSensor(1);
    TVec3f offset(0.0f, 0.0f, 0.0f);
    f32 radius = getHitRadius();
    radius *= mScale.x;
    MR::addHitSensorAtJointEnemy(this, "body", getCenterJointName(), 16, radius, offset);
}

void Birikyu::initShadow() {
    if (!mHasShadow) {
        return;
    }

    f32 hitRadius = getHitRadius();
    MR::initShadowVolumeSphere(this, (::sShadowRadiusRatio * (hitRadius * mScale.x)));
    MR::onCalcShadowDropPrivateGravity(this, nullptr);
    MR::onCalcShadow(this, nullptr);
    MR::onCalcGravity(this);
    MR::setShadowDropPositionPtr(this, nullptr, &getSensor("body")->mPosition);
}

bool Birikyu::tryStopPointing() {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_MOVE_STOP, this, getSensor("body"), "body");
        setNerve(GET_NERVE(Birikyu, HostTypeStopPointing));

        return true;
    }

    return false;
}

void Birikyu::goMove() {
    if (mConnected) {
        setNerve(GET_NERVE(Birikyu, HostTypeMove));
    } else {
        setNerve(GET_NERVE(Birikyu, HostTypeMoveCircle));
    }
}

void Birikyu::exeMove() {
    MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE");

    if (tryStopPointing()) {
        return;
    }

    if (MR::isRailReachedGoal(this)) {
        MR::reverseRailDirection(this);

        s32 waitTime = 0;
        MR::getCurrentRailPointArg0NoInit(this, &waitTime);
        if (waitTime > 0) {
            setNerve(GET_NERVE(Birikyu, HostTypeWaitAtEdge));
            return;
        }

        MR::emitEffect(this, "Clash");
    }

    MR::moveCoordAndFollowTrans(this, mMoveSpeed);
}

void Birikyu::exeMoveCircle() {
    MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE");
    if (tryStopPointing()) {
        return;
    }

    mOrbitAngle = MR::repeat(mOrbitAngle + (mMoveSpeed / ::sMoveRadius), 0.0f, TWO_PI);

    TPos3f rotationMtx;
    rotationMtx.identity();
    rotationMtx.makeRotate(mUpVec, mOrbitAngle);

    TVec3f orbitOffset = mFront * ::sMoveRadius;
    rotationMtx.mult(orbitOffset, orbitOffset);

    TVec3f finalOrbitPos = (mOrbitCenter + orbitOffset);
    mPosition.set(finalOrbitPos);
}

void Birikyu::exeWaitAtEdge() {
    MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE");

    s32 waitTime = 0;
    MR::getCurrentRailPointArg0NoInit(this, &waitTime);

    if (MR::isStep(this, waitTime)) {
        setNerve(GET_NERVE(Birikyu, HostTypeMove));
    }
}

void Birikyu::exeAttack() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::emitEffect(this, "Hit");
    }

    MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE");

    if (MR::isStep(this, ::sAttackTime)) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_MOVE_START, this, getSensor("body"), "body");
        goMove();
    }
}

void Birikyu::exeAttackWait() {
}

void Birikyu::exeStopPointing() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        if (MR::isRegisteredEffect(this, "Touch")) {
            MR::emitEffect(this, "Touch");
        }
    }

    MR::startDPDFreezeLevelSound(this);

    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        if (MR::isRegisteredEffect(this, "Touch")) {
            MR::deleteEffect(this, "Touch");
        }

        MR::sendMsgToGroupMember(ACTMES_GROUP_MOVE_START, this, getSensor("body"), "body");
        goMove();
    }
}

BirikyuWithFace::BirikyuWithFace(const char* pName) : Birikyu(pName), _CC() {
}

void BirikyuWithFace::init(const JMapInfoIter& rIter) {
    Birikyu::init(rIter);
    s32 num = -1;
    MR::getJMapInfoArg1NoInit(rIter, &num);
    _CC = num == 1;
}

void BirikyuWithFace::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.identity();

    if (mConnected) {
        TVec3f sideVec(MR::getRailDirection(this));
        if (_CC) {
            sideVec.negate();
        }

        if (!MR::isRailGoingToEnd(this)) {
            sideVec.negate();
        }

        TVec3f upVec(mUpVec);
        if (MR::isSameDirection(upVec, sideVec)) {
            MR::makeMtxSideFront(&mtx, sideVec, mFront);
        } else {
            MR::makeMtxSideUp(&mtx, sideVec, upVec);
        }
    } else {
        mtx.makeRotate(mUpVec, mOrbitAngle);
    }

    mtx.setTrans(mPosition);
    MR::setBaseTRMtx(this, mtx);
}

f32 BirikyuWithFace::getHitRadius() const {
    return ::sHitRadiusWithFace;
}

char* BirikyuWithFace::getCenterJointName() const {
    return "Center";
}
