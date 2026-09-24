#include "Game/MapObj/ArrowSwitchMulti.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/MapObj/ArrowSwitchMultiHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void ArrowSwitchMulti_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sRotYTargetList[] = {0.0f, 90.0f, 180.0f, -90.0f};

    static const f32 sPunchVelocit = 6.0f;
    static const s32 sFreqRotY = 4;
};  // namespace

namespace NrvArrowSwitchMulti {
    NEW_NERVE(ArrowSwitchMultiNrvWait, ArrowSwitchMulti, Wait);
    NEW_NERVE(ArrowSwitchMultiNrvRotate, ArrowSwitchMulti, Rotate);
};  // namespace NrvArrowSwitchMulti

ArrowSwitchTarget::ArrowSwitchTarget(const char* pName) : NameObj(pName), mIdInfo(), mStageSwitchCtrl(), mTargetIndex(-1) {
    MR::createArrowSwitchMultiHolder();
}

void ArrowSwitchTarget::init(const JMapInfoIter& rIter) {
    s32 arg0;
    MR::getJMapInfoArg0WithInit(rIter, &arg0);

    mIdInfo = new JMapIdInfo(arg0, rIter);

    MR::getJMapInfoArg1WithInit(rIter, &mTargetIndex);
    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void ArrowSwitchTarget::initAfterPlacement() {
    MR::registerArrowSwitchTarget(this);
}

void ArrowSwitchTarget::onTarget() {
    if (mStageSwitchCtrl->isValidSwitchA()) {
        mStageSwitchCtrl->onSwitchA();
    }

    if (mStageSwitchCtrl->isValidSwitchB()) {
        mStageSwitchCtrl->offSwitchB();
    }
}

void ArrowSwitchTarget::offTarget() {
    if (mStageSwitchCtrl->isValidSwitchA()) {
        mStageSwitchCtrl->offSwitchA();
    }

    if (mStageSwitchCtrl->isValidSwitchB()) {
        mStageSwitchCtrl->onSwitchB();
    }
}

ArrowSwitchMulti::ArrowSwitchMulti(const char* pName)
    : LiveActor(pName), mIdInfo(), mRotAngle(), mRotSpeed(), mActiveTargetIndex(), mRotYTargetIndex(), mIsRotPlus(true) {
    MR::createArrowSwitchMultiHolder();

    for (u32 i = 0; i < ARRAY_SIZE(mTargetArray); i++) {
        mTargetArray[i] = nullptr;
    }
}

void ArrowSwitchMulti::registerTarget(ArrowSwitchTarget* pTarget) {
    mTargetArray[pTarget->mTargetIndex] = pTarget;
}

void ArrowSwitchMulti::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("ArrowSwitch", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    s32 arg0;
    MR::getJMapInfoArg0WithInit(rIter, &arg0);

    mIdInfo = new JMapIdInfo(arg0, rIter);

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
    MR::registerArrowSwitchMulti(this);
    initNerve(GET_NERVE(ArrowSwitchMulti, ArrowSwitchMultiNrvWait));
    makeActorAppeared();
}

void ArrowSwitchMulti::control() {
}

void ArrowSwitchMulti::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    TPos3f pos;
    pos.set(getBaseMtx());
    TPos3f rot;
    rot.identity();
    rot.setEulerY(MR::toRadian(mRotAngle));
    pos.concat(pos, rot);
    MR::setBaseTRMtx(this, pos);
}

bool ArrowSwitchMulti::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerHitAll(msg)) {
        return requestPunch(pSender, pReceiver);
    }

    return false;
}

bool ArrowSwitchMulti::requestPunch(HitSensor* pSender, HitSensor* pReceiver) {
    if (getSensor("body") != pReceiver) {
        return false;
    }

    if (!isNerve(GET_NERVE(ArrowSwitchMulti, ArrowSwitchMultiNrvWait))) {
        return false;
    }

    if (mIsRotPlus) {
        mRotYTargetIndex++;
        mRotSpeed = ::sPunchVelocit;
    } else {
        mRotYTargetIndex--;
        mRotSpeed = -::sPunchVelocit;
    }

    mRotYTargetIndex = (mRotYTargetIndex + ::sFreqRotY) % ::sFreqRotY;
    MR::invalidateClipping(this);
    setNerve(GET_NERVE(ArrowSwitchMulti, ArrowSwitchMultiNrvRotate));

    return true;
}

void ArrowSwitchMulti::exeWait() {
    if (MR::isFirstStep(this)) {
        if (mRotYTargetIndex % 2 == 1) {
            MR::startBtk(this, "On");
        } else {
            MR::startBtk(this, "Off");
        }

        MR::validateClipping(this);
    }
}

void ArrowSwitchMulti_FORCE_MATCH_SDATA2_2() {
    // TODO: why?
    (void)360.0f;
}

void ArrowSwitchMulti::exeRotate() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ARROW_SWITCH_ON");
    }

    MR::startLevelSound(this, "SE_OJ_LV_ARROW_SWITCH_MOVE");
    mRotAngle += mRotSpeed;
    mRotAngle = MR::repeat(mRotAngle, -180.0f, 360.0f);
    f32 diff = MR::repeat(::sRotYTargetList[mRotYTargetIndex] - mRotAngle, -180.0f, 360.0f);

    if (mIsRotPlus && diff < 0.0f || !mIsRotPlus && diff > 0.0f) {
        mRotAngle = ::sRotYTargetList[mRotYTargetIndex];
        mRotSpeed = 0.0f;

        if (mTargetArray[mActiveTargetIndex] != nullptr) {
            mTargetArray[mActiveTargetIndex]->offTarget();
        }
        if (mTargetArray[mRotYTargetIndex] != nullptr) {
            mTargetArray[mRotYTargetIndex]->onTarget();
        }
        mActiveTargetIndex = mRotYTargetIndex;

        MR::startSystemSE("SE_SY_GRAVITY_SWITCHED");
        MR::startSound(this, "SE_OJ_ARROW_SWITCH_STOP");

        setNerve(GET_NERVE(ArrowSwitchMulti, ArrowSwitchMultiNrvWait));
    }
}
