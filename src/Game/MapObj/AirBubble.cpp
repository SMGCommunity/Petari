#include "Game/MapObj/AirBubble.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const f32 cHitSensorRadius = 130.0f;
    const f32 cSwingRange = 250.0f;
    // const s32 cGrowTime = _;
    const f32 cCorioriRotSpeed = 1.5f;
    // const f32 cCoriolisAccel = _;
    const f32 cMoveStartAccel = 7.0f;
    // const f32 cFloatAccel = _;
    const s32 cDefaultLife = 360;
};  // namespace

namespace NrvAirBubble {
    NEW_NERVE(AirBubbleNrvWait, AirBubble, Wait);
    NEW_NERVE(AirBubbleNrvMove, AirBubble, Move);
    NEW_NERVE(AirBubbleNrvBreak, AirBubble, Break);
    NEW_NERVE(AirBubbleNrvKillWait, AirBubble, KillWait);
};  // namespace NrvAirBubble

AirBubble::AirBubble(const char* pName) : LiveActor(pName), _A4(0.0f, 0.0f, 1.0f), _B0(), mLife(::cDefaultLife) {
}

void AirBubble::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    initModelManagerWithAnm("AirBubble", nullptr, false);
    MR::connectToSceneItem(this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, mScale.x * ::cHitSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    initNerve(&NrvAirBubble::AirBubbleNrvWait::sInstance);
    mIsFollowRail = false;
    makeActorAppeared();
    MR::setClippingFar100m(this);
    MR::startBck(this, "Move", nullptr);
    _C9 = false;
}

void AirBubble::initAfterPlacement() {
    if (mIsFollowRail) {
        MR::moveCoordAndTransToNearestRailPos(this);
    }

    _8C = mPosition;
}

void AirBubble::appear() {
    LiveActor::appear();
}

void AirBubble::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    LiveActor::kill();
}

void AirBubble::control() {
}

void AirBubble::appearMove(const TVec3f& rTrans, s32 life) {
    _8C.set(rTrans);
    mPosition.set(rTrans);
    appear();
    MR::showModel(this);
    setNerve(&NrvAirBubble::AirBubbleNrvMove::sInstance);
    MR::invalidateClipping(this);

    mLife = life > 0 ? life : ::cDefaultLife;
}

void AirBubble::exeWait() {
    if (MR::isFirstStep(this)) {
    }

    _B0 = MR::sin(MR::calcNerveRate(this, 120) * MR::pi() * 2.0f);

    if (mIsFollowRail) {
        if (MR::isRailReachedGoal(this)) {
            MR::reverseRailDirection(this);
        }

        f32 arg0;

        if (MR::getCurrentRailPointArg0WithInit(this, &arg0)) {
            mRailMoveSpeed = arg0;
        }

        MR::moveCoordAndFollowTrans(this, mRailMoveSpeed);
    }

    if (_C9) {
        return;
    }

    mPosition = _8C + TVec3f(0.0f, 1.0f, 0.0f) * _B0 * 1.0f;
}

void AirBubble::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::onCalcGravity(this);

        mVelocity = -mGravity * ::cMoveStartAccel;
    }

    MR::rotateVecDegree(&_A4, mVelocity, ::cCorioriRotSpeed);
    _A4.killElement(mVelocity);

    if (MR::isNearZero(_A4)) {
        MR::getRandomVector(&_A4, 1.0f);
    }

    MR::normalizeOrZero(&_A4);
    mGravity += _A4 * 0.1f;
    mVelocity -= mGravity * 0.3f;
    mVelocity *= 0.85f;

    if (MR::isGreaterStep(this, mLife)) {
        MR::hideModel(this);
        MR::startSound(this, "SE_OJ_AIR_BUBBLE_BREAK");
        MR::emitEffect(this, "RecoveryBubbleBreak");
        MR::offCalcGravity(this);
        setNerve(&NrvAirBubble::AirBubbleNrvKillWait::sInstance);
    }
}

void AirBubble::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_AIR_BUBBLE_BREAK");
        MR::emitEffect(this, "RecoveryBubbleBreak");
        MR::incPlayerOxygen(8);
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        kill();
    }
}

void AirBubble::exeKillWait() {
    if (MR::isGreaterStep(this, 90)) {
        kill();
    }
}

bool AirBubble::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        if (isNerve(&NrvAirBubble::AirBubbleNrvKillWait::sInstance)) {
            MR::incPlayerOxygen(8);
            kill();

            return true;
        }

        setNerve(&NrvAirBubble::AirBubbleNrvBreak::sInstance);

        return true;
    }

    return false;
}

bool AirBubble::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgItemShow(msg)) {
        return MR::receiveItemShowMsg(msg, pSender, pReceiver);
    }

    if (MR::isMsgItemHide(msg)) {
        return MR::receiveItemHideMsg(msg, pSender, pReceiver);
    }

    if (MR::isMsgItemStartMove(msg)) {
        _C9 = true;

        return true;
    }

    if (MR::isMsgItemEndMove(msg)) {
        _C9 = false;
        _8C = mPosition;

        return true;
    }

    if (MR::isMsgSpinStormRange(msg) && canSpinGet()) {
        if ((pSender->mPosition - mPosition).length() < ::cSwingRange) {
            setNerve(&NrvAirBubble::AirBubbleNrvBreak::sInstance);
            return true;
        }
    }

    return false;
}

bool AirBubble::canSpinGet() const {
    if (isNerve(&NrvAirBubble::AirBubbleNrvWait::sInstance) || isNerve(&NrvAirBubble::AirBubbleNrvMove::sInstance)) {
        return true;
    }

    return false;
}
