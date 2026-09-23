#include "Game/MapObj/ArrowSwitch.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

void ArrowSwitch_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sRotYTargetList[] = {0.0f, 90.0f, 180.0f, -90.0f};

    static const f32 sTestScale = 1.0f;
    static const f32 sPunchVelocit = 6.0f;
    static const s32 sFreqRotY = 4;
};  // namespace

namespace NrvArrowSwitch {
    NEW_NERVE(ArrowSwitchNrvWait, ArrowSwitch, Wait);
    NEW_NERVE(ArrowSwitchNrvRotate, ArrowSwitch, Rotate);
    NEW_NERVE(ArrowSwitchNrvLock, ArrowSwitch, Lock);
};  // namespace NrvArrowSwitch

ArrowSwitch::ArrowSwitch(const char* pName)
    : LiveActor(pName), mRotAngle(), mRotYTargetIndex(), mRotSpeed(), mSwitchType(-1), mLockAfterRotate(), mIsRotPlus(true), _9E(), mIsPunch() {
}

void ArrowSwitch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mScale.set(::sTestScale);
    initModelManagerWithAnm("ArrowSwitch", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    TVec3f up;
    MR::calcUpVec(&up, this);
    mGravity.set(-up);

    MR::initShadowFromCSV(this, "Shadow");
    MR::onCalcShadow(this, nullptr);
    MR::onCalcGravity(this);

    if (MR::isInAreaObj("PlaneModeCube", mPosition)) {
        initHitSensor(1);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 50.0f, 250.0f));
    } else {
        initHitSensor(2);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 50.0f, 250.0f));
        MR::addHitSensorMapObjMoveCollision(this, "collision", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
        MR::initCollisionParts(this, "ArrowSwitch", getSensor("collision"), nullptr);
    }

    initSound(4, false);
    initNerve(GET_NERVE(ArrowSwitch, ArrowSwitchNrvWait));
    MR::getJMapInfoArg0WithInit(rIter, &mSwitchType);
    MR::getJMapInfoArg1WithInit(rIter, &mLockAfterRotate);

    s32 shadowDropLength = -1;
    MR::getJMapInfoArg2WithInit(rIter, &shadowDropLength);

    if (shadowDropLength >= 0) {
        MR::setShadowDropLength(this, nullptr, shadowDropLength);
    }

    MR::needStageSwitchWriteA(this, rIter);

    if (!mLockAfterRotate) {
        MR::listenStageSwitchOnOffA(this, MR::Functor(this, &ArrowSwitch::listenOnSwitch), MR::Functor(this, &ArrowSwitch::listenOffSwitch));
    }

    makeActorAppeared();
}

void ArrowSwitch::control() {
}

void ArrowSwitch::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    TPos3f pos;
    pos.set(getBaseMtx());
    TPos3f rot;
    rot.identity();
    rot.setEulerY(MR::toRadian(mRotAngle));
    pos.concat(pos, rot);
    MR::setBaseTRMtx(this, pos);
}

void ArrowSwitch::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool ArrowSwitch::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHitAll(msg)) {
        return requestPunch(pSender, pReceiver);
    }

    return false;
}

bool ArrowSwitch::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool ArrowSwitch::requestPunch(HitSensor* pSender, HitSensor* pReceiver) {
    if (getSensor("body") != pReceiver) {
        return false;
    }

    if (!isNerve(GET_NERVE(ArrowSwitch, ArrowSwitchNrvWait))) {
        return false;
    }

    TVec3f side;
    MR::calcSideVec(&side, this);

    TVec3f sensorDir;
    MR::calcSensorDirection(&sensorDir, pSender, pReceiver);

    if (isPlusLimit()) {
        mIsRotPlus = false;
    } else if (isMinusLimit()) {
        mIsRotPlus = true;
    } else if (side.dot(sensorDir) > 0.0f) {
        mIsRotPlus = true;
    } else {
        mIsRotPlus = false;
    }

    mRotYTargetIndex += mIsRotPlus ? getOneStep() : -getOneStep();
    mRotYTargetIndex = (mRotYTargetIndex + ::sFreqRotY) % ::sFreqRotY;
    mRotSpeed = mIsRotPlus ? ::sPunchVelocit : -::sPunchVelocit;
    mIsPunch = true;

    MR::invalidateClipping(this);
    setNerve(GET_NERVE(ArrowSwitch, ArrowSwitchNrvRotate));

    return true;
}

void ArrowSwitch::listenOnSwitch() {
    if (mRotYTargetIndex != Direction_Up) {
        return;
    }

    switch (mSwitchType) {
    case -1:
    case SwitchType_HalfPlus:
        mIsRotPlus = true;
        mRotSpeed = ::sPunchVelocit;
        mRotYTargetIndex = Direction_Down;
        break;
    case SwitchType_HalfMinus:
        mIsRotPlus = false;
        mRotSpeed = -::sPunchVelocit;
        mRotYTargetIndex = Direction_Down;
        break;
    case SwitchType_QuarterPlus:
        mIsRotPlus = true;
        mRotSpeed = ::sPunchVelocit;
        mRotYTargetIndex = Direction_Right;
        break;
    case SwitchType_QuarterMinus:
        mIsRotPlus = false;
        mRotSpeed = -::sPunchVelocit;
        mRotYTargetIndex = Direction_Left;
        break;
    default:
        return;
    }

    MR::invalidateClipping(this);
    setNerve(GET_NERVE(ArrowSwitch, ArrowSwitchNrvRotate));
}

void ArrowSwitch::listenOffSwitch() {
    if (mRotYTargetIndex == Direction_Up) {
        return;
    }

    mRotYTargetIndex = Direction_Up;

    switch (mSwitchType) {
    case -1:
    case SwitchType_QuarterPlus:
    case SwitchType_HalfPlus:
        mIsRotPlus = false;
        mRotSpeed = -::sPunchVelocit;
        break;
    case SwitchType_QuarterMinus:
    case SwitchType_HalfMinus:
        mIsRotPlus = true;
        mRotSpeed = ::sPunchVelocit;
        break;
    default:
        return;
    }

    MR::invalidateClipping(this);
    setNerve(GET_NERVE(ArrowSwitch, ArrowSwitchNrvRotate));
}

void ArrowSwitch::exeWait() {
    if (MR::isFirstStep(this)) {
        if (mRotYTargetIndex == Direction_Up) {
            MR::startBtk(this, "Off");
        } else {
            MR::startBtk(this, "On");
        }

        MR::validateClipping(this);
        MR::onCalcShadow(this, nullptr);
    }
}

void ArrowSwitch_FORCE_MATCH_SDATA2_2() {
    // TODO: why?
    (void)360.0f;
}

void ArrowSwitch::exeRotate() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ARROW_SWITCH_ON");
        MR::onCalcShadow(this, nullptr);

        if (mLockAfterRotate) {
            MR::invalidateHitSensors(this);

            if (MR::isExistCollisionParts(this)) {
                MR::invalidateCollisionParts(this);
            }
        }
    }

    MR::startLevelSound(this, "SE_OJ_LV_ARROW_SWITCH_MOVE");
    mRotAngle += mRotSpeed;
    mRotAngle = MR::repeat(mRotAngle, -180.0f, 360.0f);
    f32 diff = MR::repeat(::sRotYTargetList[mRotYTargetIndex] - mRotAngle, -180.0f, 360.0f);

    if (mIsRotPlus && diff < 0.0f || !mIsRotPlus && diff > 0.0f) {
        mRotAngle = ::sRotYTargetList[mRotYTargetIndex];
        mRotSpeed = 0.0f;

        if (MR::isValidSwitchA(this) && mIsPunch) {
            if (mRotYTargetIndex != 0) {
                MR::onSwitchA(this);
            } else {
                MR::offSwitchA(this);
            }

            MR::startSystemSE("SE_SY_GRAVITY_SWITCHED");
        }

        MR::startSound(this, "SE_OJ_ARROW_SWITCH_STOP");

        mIsPunch = false;

        if (mLockAfterRotate) {
            setNerve(GET_NERVE(ArrowSwitch, ArrowSwitchNrvLock));
        } else {
            setNerve(GET_NERVE(ArrowSwitch, ArrowSwitchNrvWait));
        }
    }
}

void ArrowSwitch::exeLock() {
    if (MR::isFirstStep(this)) {
        if (mRotYTargetIndex == Direction_Up) {
            MR::startBtk(this, "Off");
        } else {
            MR::startBtk(this, "On");
        }

        MR::validateClipping(this);
        MR::onCalcShadow(this, nullptr);
    }
}

bool ArrowSwitch::isPlusLimit() const {
    switch (mSwitchType) {
    case SwitchType_HalfPlus:
        return mRotYTargetIndex == Direction_Down;
    case SwitchType_QuarterPlus:
        return mRotYTargetIndex == Direction_Right;
    case SwitchType_QuarterMinus:
        return mRotYTargetIndex == Direction_Up;
    case SwitchType_HalfMinus:
        return mRotYTargetIndex == Direction_Up;
    default:
        return false;
    }
}

bool ArrowSwitch::isMinusLimit() const {
    switch (mSwitchType) {
    case SwitchType_HalfPlus:
        return mRotYTargetIndex == Direction_Up;
    case SwitchType_QuarterPlus:
        return mRotYTargetIndex == Direction_Up;
    case SwitchType_QuarterMinus:
        return mRotYTargetIndex == Direction_Left;
    case SwitchType_HalfMinus:
        return mRotYTargetIndex == Direction_Down;
    default:
        return false;
    }
}

s32 ArrowSwitch::getOneStep() const {
    switch (mSwitchType) {
    case -1:
    case SwitchType_HalfPlus:
    case SwitchType_HalfMinus:
        return 2;
    case SwitchType_QuarterPlus:
    case SwitchType_QuarterMinus:
        return 1;
    default:
        return 0;
    }
}
