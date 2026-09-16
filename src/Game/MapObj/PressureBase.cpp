#include "Game/MapObj/PressureBase.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void PressureBase_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const s32 sDefaultShotInterval = 300;
    static const f32 sDefaultFirstSpeed = 30.0f;
    static const s32 sStepForChargeToShot = 16;
    static const s32 sStepForChargeToShortShot = 54;
    static const s32 sSyncWaitTime = 60;
    static const f32 sAimTurnRate = 5.0f;
    static const s32 sStepForBound = 20;
    static const f32 sBoundFreq = 1.0f;
    static const f32 sBoundAmp = 0.2f;
    static const f32 sRelaxAngleFix = -45.0f;
};  // namespace

namespace NrvPressureMessenger {
    NEW_NERVE(PressureMessengerNrvSync, PressureMessenger, Sync);
};  // namespace NrvPressureMessenger

namespace NrvPressureBase {
    NEW_NERVE(PressureBaseNrvRelaxStart, PressureBase, Bound);
    NEW_NERVE(PressureBaseNrvWaitStart, PressureBase, Bound);
    NEW_NERVE(PressureBaseNrvRelax, PressureBase, Relax);
    NEW_NERVE(PressureBaseNrvSyncWait, PressureBase, SyncWait);
    NEW_NERVE(PressureBaseNrvFirstWait, PressureBase, FirstWait);
    NEW_NERVE(PressureBaseNrvWait, PressureBase, Wait);
    NEW_NERVE(PressureBaseNrvPrepareToShot, PressureBase, PrepareToShot);
    NEW_NERVE(PressureBaseNrvShot, PressureBase, Shot);
};  // namespace NrvPressureBase

PressureMessenger::PressureMessenger(MsgSharedGroup* pGroup, const char* pName) : LiveActor(pName), mSharedGroup(pGroup), mWaitSyncTime() {
}

void PressureMessenger::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::invalidateClipping(this);
    initNerve(GET_NERVE(PressureMessenger, PressureMessengerNrvSync));
    makeActorAppeared();
}

void PressureMessenger::exeSync() {
    if (MR::isStep(this, mWaitSyncTime)) {
        mSharedGroup->sendMsgToGroupMember(ACTMES_GROUP_MOVE_START, getSensor("body"), "body");
        setNerve(GET_NERVE(PressureMessenger, PressureMessengerNrvSync));
    }
}

PressureBase::PressureBase(const char* pName)
    : LiveActor(pName), mJointController(), mFront(0.0f, 0.0f, 1.0f), mRelaxAngle(), mBaseAngle(), mWaitTime(::sDefaultShotInterval),
      mBallSpeed(::sDefaultFirstSpeed), mShotType(), mIsShortShot(), mMessenger(), mGroup(), _BC() {
}

void PressureBase::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* objName;
    MR::getObjectName(&objName, rIter);
    initModelManagerWithAnm(objName, nullptr, false);
    MR::calcFrontVec(&mFront, this);
    MR::connectToSceneNoShadowedMapObjStrongLight(this);
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 8, 70.0f, TVec3f(0.0f, 30.0f, 0.0f));
    MR::addHitSensorAtJointMapObjSimple(this, "cannon", "Cannon1", 8, 70.0f, TVec3f(40.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(6, false);
    MR::initShadowVolumeSphere(this, 75.0f);
    MR::invalidateShadow(this, nullptr);

    mJointController = MR::createJointDelegatorWithNullChildFunc(this, &PressureBase::calcJointCannonV, "Cannon1");
    MR::initJointTransform(this);

    MR::getJMapInfoArg0NoInit(rIter, &mBaseAngle);
    MR::getJMapInfoArg1NoInit(rIter, &mWaitTime);

    s16 frame = MR::getBckFrameMax(this, "ShotStart");
    mIsShortShot = (mWaitTime < frame);

    initBullet(rIter);
    MR::getJMapInfoArg2NoInit(rIter, &mBallSpeed);
    MR::getJMapInfoArg3NoInit(rIter, &mShotType);
    MR::calcGravity(this);

    MR::setGroupClipping(this, rIter, 32);
    mGroup = MR::joinToGroupArray(this, rIter, "プレッシャー軍団", 32);
    if (mGroup != nullptr) {
        PressureBase* actor = static_cast< PressureBase* >(mGroup->getActor(0));

        if (this == actor) {
            mMessenger = new PressureMessenger(mGroup, "プレッシャー同期メッセンジャー");
            mMessenger->initWithoutIter();
        }
    }

    MR::tryRegisterDemoCast(this, rIter);
    MR::useStageSwitchSleep(this, rIter);

    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnOffA(this, MR::Functor(this, &PressureBase::startWait), MR::Functor(this, &PressureBase::startRelax));
        initNerve(GET_NERVE(PressureBase, PressureBaseNrvRelax));
    } else {
        initNerve(GET_NERVE(PressureBase, PressureBaseNrvFirstWait));
    }

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void PressureBase::initAfterPlacement() {
    if (mMessenger != nullptr) {
        s32 waitTime = -1;

        for (u16 i = 0; i < MR::getGroupFromArray(this)->getObjNum(); i++) {
            PressureBase* actor = static_cast< PressureBase* >(MR::getGroupFromArray(this)->getActor(i));

            if (actor->mWaitTime > waitTime) {
                waitTime = actor->mWaitTime;
            }
        }

        mMessenger->mWaitSyncTime = waitTime + ::sSyncWaitTime;
    }
}

void PressureBase::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mJointController->registerCallBack();
}

void PressureBase::control() {
    if (mShotType == ShotType_AimTurn) {
        MR::turnDirectionToTargetDegree(this, &mFront, *MR::getPlayerPos(), ::sAimTurnRate);
    }
}

void PressureBase::exeBound() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelaxStart))) {
            MR::startBck(this, "SwitchOff", nullptr);
        } else {
            MR::startBck(this, "SwitchOn", nullptr);
        }
    }

    f32 rate = MR::calcNerveRate(this, ::sStepForBound);
    f32 scale = MR::getScaleWithReactionValueZeroToOne(rate, ::sBoundFreq, -::sBoundAmp);
    scale *= (::sRelaxAngleFix - mBaseAngle);

    if (isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelaxStart))) {
        mRelaxAngle = mBaseAngle + scale;
    } else {
        mRelaxAngle = ::sRelaxAngleFix - scale;
    }

    if (MR::isStep(this, ::sStepForBound)) {
        if (isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelaxStart))) {
            setNerve(GET_NERVE(PressureBase, PressureBaseNrvRelax));
        } else {
            setNerve(GET_NERVE(PressureBase, PressureBaseNrvWait));
        }
    }
}

void PressureBase::exeRelax() {
    if (MR::isFirstStep(this)) {
        mRelaxAngle = ::sRelaxAngleFix;
    }
}

void PressureBase::exeSyncWait() {
}

void PressureBase::exeFirstWait() {
    if (MR::isStep(this, mWaitTime)) {
        setNerve(GET_NERVE(PressureBase, PressureBaseNrvPrepareToShot));
    }
}

void PressureBase::exeWait() {
    if (mWaitTime == MR::getBckFrameMax(this, "ShotStart") + getNerveStep()) {
        setNerve(GET_NERVE(PressureBase, PressureBaseNrvPrepareToShot));
    } else if (MR::isStep(this, mWaitTime)) {
        setNerve(GET_NERVE(PressureBase, PressureBaseNrvShot));
    }
}

void PressureBase::exePrepareToShot() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShotStart", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PressureBase, PressureBaseNrvShot));
    }
}

void PressureBase::exeShot() {
    if (MR::isFirstStep(this)) {
        if (mIsShortShot) {
            MR::startBck(this, "ShortShot", nullptr);
        } else {
            MR::startBck(this, "Shot", nullptr);
        }
    }

    if (mIsShortShot) {
        if (MR::isStep(this, ::sStepForChargeToShortShot)) {
            shotBullet(mBallSpeed);
        }
    } else {
        if (MR::isStep(this, ::sStepForChargeToShot)) {
            shotBullet(mBallSpeed);
        }
    }

    if (MR::isBckStopped(this)) {
        if (mGroup != nullptr) {
            setNerve(GET_NERVE(PressureBase, PressureBaseNrvSyncWait));
        } else {
            setNerve(GET_NERVE(PressureBase, PressureBaseNrvWait));
        }
    }
}

void PressureBase::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) || MR::isSensorEnemy(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool PressureBase::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

bool PressureBase::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_GROUP_MOVE_START) {
        bool isRelax = isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelaxStart)) || isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelax));

        if (isRelax) {
            return false;
        }

        setNerve(GET_NERVE(PressureBase, PressureBaseNrvWait));

        return true;
    }

    return false;
}

void PressureBase::startWait() {
    if (isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelax))) {
        setNerve(GET_NERVE(PressureBase, PressureBaseNrvWaitStart));
    }
}

void PressureBase::startRelax() {
    bool isRelax = isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelaxStart)) || isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelax));

    if (!isRelax) {
        MR::startSound(this, "SE_OJ_W_PRESS_HEAD_OFF");
        setNerve(GET_NERVE(PressureBase, PressureBaseNrvRelaxStart));
    }
}

void PressureBase::initBullet(const JMapInfoIter&) {
}

bool PressureBase::shotBullet(f32) {
    return false;
}

bool PressureBase::calcJointCannonV(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    TVec3f front(0.0f, 0.0f, 1.0f);
    bool isRelax = isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelaxStart)) || isNerve(GET_NERVE(PressureBase, PressureBaseNrvRelax)) ||
                   isNerve(GET_NERVE(PressureBase, PressureBaseNrvWaitStart));

    f32 angle = isRelax ? mRelaxAngle : mBaseAngle;

    TPos3f mtx;
    mtx.makeRotate(front, MR::toRadian(angle));
    pMtx->concat(*pMtx, mtx);
    return true;
}

bool PressureBase::isShotTypeOnGravity() const {
    return mShotType == ShotType_OnGravity;
}

bool PressureBase::isShotTypeFollow() const {
    return mShotType == ShotType_Follow;
}
