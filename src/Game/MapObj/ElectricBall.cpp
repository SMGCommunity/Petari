#include "Game/MapObj/ElectricBall.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <algorithm>

namespace {
    const f32 sMoveRadiusDefault = 750.0f;
    const f32 sMoveAngularSpeedDefault = 0.01f;
    const s32 sNumBallsDefault = 4;
    const s32 sAttackTime = 90;
    const f32 sSensorRadius = 75.0f;
    const f32 sShadowRadius = 75.0f;
    const s32 sNumBallsMax = 16;
};  // namespace

namespace NrvElectricBall {
    NEW_NERVE(HostTypeWait, ElectricBall, Wait);
    NEW_NERVE(HostTypeMove, ElectricBall, Move);
    NEW_NERVE(HostTypeAttack, ElectricBall, Attack);
};  // namespace NrvElectricBall

ElectricBall::Ball::Ball() : mHost(), mPosition(gZeroVec), mIsActive() {
}

void ElectricBall::Ball::init(LiveActor* pActor) {
    mHost = new PartsModel(pActor, "ビリビリボール", "ElectricBall", nullptr, -1, false);
    mHost->initWithoutIter();
    mHost->makeActorAppeared();

    MR::initShadowVolumeSphere(mHost, ::sShadowRadius);

    MR::onCalcGravity(mHost);
}

void ElectricBall::Ball::updatePosition(const TPos3f& rPos) {
    rPos.mult(mPosition, mHost->mPosition);
}

void ElectricBall::Ball::activate() {
    mIsActive = true;
    MR::emitEffect(mHost, "Electric");
}

ElectricBall::ElectricBall(const char* pName)
    : LiveActor(pName), mBalls(), mClosestBallPos(gZeroVec), mRadius(::sMoveRadiusDefault), mAngularSpeed(::sMoveAngularSpeedDefault), mRailMover() {
    mBaseMtx.identity();
}

void ElectricBall::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::makeMtxTR(mBaseMtx, this);

    MR::getJMapInfoArg1NoInit(rIter, &mRadius);
    MR::getJMapInfoArg2NoInit(rIter, &mAngularSpeed);

    mAngularSpeed *= ::sMoveAngularSpeedDefault;

    MR::connectToSceneMapObjMovement(this);

    initHitSensor(1);
    MR::addHitSensorPosMapObj(this, "body", 8, ::sSensorRadius, &mClosestBallPos, TVec3f(0.0f, 0.0f, 0.0f));

    initSound(4, false);
    initBalls(rIter);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);

        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
    }

    MR::setClippingTypeSphere(this, mRadius + ::sSensorRadius);
    MR::setGroupClipping(this, rIter, ::sNumBallsMax);

    initNerve(GET_NERVE(ElectricBall, HostTypeWait));

    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &ElectricBall::startMove));
    } else {
        startMove();
    }

    makeActorAppeared();
}

void ElectricBall::initAfterPlacement() {
    std::for_each(mBalls.begin(), mBalls.end(), std::bind2nd(std::mem_fun1_ref_t< void, Ball, const TPos3f& >(&Ball::updatePosition), mBaseMtx));

    Ball* ball = getNearestBall();
    mClosestBallPos.set(ball->mHost->mPosition);
}

void ElectricBall::control() {
    if (isNerve(GET_NERVE(ElectricBall, HostTypeMove)) || isNerve(GET_NERVE(ElectricBall, HostTypeAttack))) {
        if (mRailMover != nullptr) {
            mRailMover->movement();

            if (mRailMover->isWorking()) {
                mPosition.set(mRailMover->_28);
            }
        }

        TPos3f mtx;
        mtx.identity();
        mtx.setEulerY(mAngularSpeed * PI_180);
        mBaseMtx.concat(mBaseMtx, mtx);
        mBaseMtx.setTrans(mPosition);

        std::for_each(mBalls.begin(), mBalls.end(), std::bind2nd(std::mem_fun1_ref_t< void, Ball, const TPos3f& >(&Ball::updatePosition), mBaseMtx));

        MR::startLevelSound(this, "SE_OJ_LV_BIRIKYU_MOVE");
    }

    mClosestBallPos.set(getNearestBall()->mHost->mPosition);
}

void ElectricBall::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayerOrRide(pReceiver)) {
        return;
    }

    if (!isNerve(GET_NERVE(ElectricBall, HostTypeAttack)) && !isNerve(GET_NERVE(ElectricBall, HostTypeWait)) &&
        MR::sendMsgEnemyAttackElectric(pReceiver, pSender)) {
        setNerve(GET_NERVE(ElectricBall, HostTypeAttack));
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool ElectricBall::receiveMsgPlayerAttack(u32 msg, HitSensor*, HitSensor*) {
    return MR::isMsgStarPieceReflect(msg);
}

void ElectricBall::initBalls(const JMapInfoIter& rIter) {
    s32 arg0 = ::sNumBallsDefault;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    mBalls.init(arg0);

    std::for_each_array(mBalls.begin(), mBalls.end(), std::bind2nd(std::mem_func(&Ball::updatePosition), mBaseMtx));

    TVec3f vec(mRadius, 0.0f, 0.0f);
    f32 f1 = TWO_PI / mBalls.size();

    for (Ball* ball = mBalls.begin(); ball != mBalls.end(); ball++) {
        f32 angle = f1 * ((ball - mBalls.begin()) / sizeof(&ball));
        MR::rotateVecRadian(&ball->mPosition, vec, TVec3f(0.0f, 1.0f, 0.0f), angle);
    }
}

ElectricBall::Ball* ElectricBall::getNearestBall() {
    f32 minDistance = FLOAT_MAX;
    Ball* nearestBall = mBalls.begin();

    for (Ball* ball = mBalls.begin(); ball != mBalls.end(); ball++) {
        f32 distanceToPlayer = MR::calcDistanceToPlayer(ball->mHost->mPosition);
        if (distanceToPlayer < minDistance) {
            nearestBall = ball;
            minDistance = distanceToPlayer;
        }
    }

    return nearestBall;
}

void ElectricBall::startMove() {
    if (mRailMover != nullptr) {
        mRailMover->start();
    }

    std::for_each_array(mBalls.begin(), mBalls.end(), std::mem_func(&Ball::activate));

    setNerve(GET_NERVE(ElectricBall, HostTypeMove));
}

void ElectricBall::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(getNearestBall()->mHost, "ElectricHit");
    }

    if (MR::isStep(this, ::sAttackTime)) {
        MR::emitEffect(getNearestBall()->mHost, "ElectricHit");
        setNerve(GET_NERVE(ElectricBall, HostTypeMove));
    }
}
