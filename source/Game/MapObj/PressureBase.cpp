#include "Game/MapObj/PressureBase.h"

PressureMessenger::PressureMessenger(MsgSharedGroup *pGroup, const char *pName) : LiveActor(pName) {
    mSharedGroup = pGroup;
    _90 = 0;
}

void PressureMessenger::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovement(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::invalidateClipping(this);
    initNerve(&NrvPressureMessenger::PressureMessengerNrvSync::sInstance);
    makeActorAppeared();
}

void PressureMessenger::exeSync() {
    if (MR::isStep(this, _90)) {
        mSharedGroup->sendMsgToGroupMember(0x68, getSensor("body"), "body");
        setNerve(&NrvPressureMessenger::PressureMessengerNrvSync::sInstance);
    }
}

PressureBase::PressureBase(const char *pName) : LiveActor(pName) {
    mJointController = nullptr;
    mFront.x = 0.0f;
    mFront.y = 0.0f;
    mFront.z = 1.0f;
    _9C = 0.0f;
    mNozzleRotation = 0.0f;
    mWaitTime = 300;
    mBallSpeed = 30.0f;
    mShotType = 0;
    _B0 = 0;
    mMessenger = nullptr;
    mGroup = nullptr;
    _BC = 0;
}

void PressureBase::init(const JMapInfoIter &rIter) {
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
    mJointController = MR::createJointDelegatorWithNullChildFunc<PressureBase>(this, &PressureBase::calcJointCannonV, "Cannon1");
    MR::initJointTransform(this);
    MR::getJMapInfoArg0NoInit(rIter, &mNozzleRotation);
    MR::getJMapInfoArg1NoInit(rIter, &mWaitTime);
    s16 frame = MR::getBckFrameMax(this, "ShotStart");
    _B0 = (mWaitTime < frame);
    //_B0 = ((frame ^ mWaitTime >> 1) - (frame ^ mWaitTime) < 0;
    initBullet(rIter);
    MR::getJMapInfoArg2NoInit(rIter, &mBallSpeed);
    MR::getJMapInfoArg3NoInit(rIter, &mShotType); 
    MR::calcGravity(this);
    MR::setGroupClipping(this, rIter, 32);
    mGroup = MR::joinToGroupArray(this, rIter, "プレッシャー軍団", 0x20);

    if (mGroup != nullptr) {
        PressureBase* actor = (PressureBase*)mGroup->getActor(0);

        if (this == actor) {
            mMessenger = new PressureMessenger(mGroup, "プレッシャー同期メッセンジャー");
            mMessenger->initWithoutIter();
        }

    }

    MR::tryRegisterDemoCast(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    
    if (MR::useStageSwitchReadA(this, rIter)) {
        void (PressureBase::*relaxFunc)(void) = &PressureBase::startRelax;
        void (PressureBase::*waitFunc)(void) = &PressureBase::startWait;
        MR::listenStageSwitchOnOffA(this, MR::Functor(this, relaxFunc), MR::Functor(this, waitFunc));
        initNerve(&NrvPressureBase::PressureBaseNrvRelax::sInstance);
    }
    else {
        initNerve(&NrvPressureBase::PressureBaseNrvFirstWait::sInstance);
    }

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

#ifdef NON_MATCHING
// regswap
void PressureBase::initAfterPlacement() {
    if (mMessenger != nullptr) {
        s32 waitTime = -1;

        for (u16 i = 0; i < MR::getGroupFromArray(this)->mObjectCount; ++i) {
            PressureBase* actor = (PressureBase*)MR::getGroupFromArray(this)->getActor(i);

            if (actor->mWaitTime > waitTime) {
                waitTime = actor->mWaitTime;
            }
        }

        mMessenger->_90 = waitTime + 60;
    }
}
#endif

void PressureBase::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mJointController->registerCallBack();
}

void PressureBase::control() {
    if (mWaitTime == 3) {
        MR::turnDirectionToTargetDegree(this, &mFront, *MR::getPlayerPos(), 5.0f);
    }
}

void PressureBase::exeBound() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvPressureBase::PressureBaseNrvRelaxStart::sInstance)) {
            MR::startBck(this, "SwitchOff", nullptr);
        }
        else {
            MR::startBck(this, "SwitchOn", nullptr);
        }
    }

    f32 rate = MR::calcNerveRate(this, 0x14);
    f32 scale = MR::getScaleWithReactionValueZeroToOne(rate, 1.0f, -2.0f);
    scale *= (-45.0f - mNozzleRotation);

    if (isNerve(&NrvPressureBase::PressureBaseNrvRelaxStart::sInstance)) {
        _9C = mNozzleRotation + scale;
    }
    else {
        _9C = -45.0f - scale;
    }

    if (MR::isStep(this, 0x14)) {
        if (isNerve(&NrvPressureBase::PressureBaseNrvRelaxStart::sInstance)) {
            setNerve(&NrvPressureBase::PressureBaseNrvRelax::sInstance);
        }
        else {
            setNerve(&NrvPressureBase::PressureBaseNrvWait::sInstance);
        }
    }
}

void PressureBase::exeWait() {
    if (mWaitTime == MR::getBckFrameMax(this, "ShotStart") + getNerveStep()) {
        setNerve(&NrvPressureBase::PressureBaseNrvPrepareToShot::sInstance);
    }
    else if (MR::isStep(this, mWaitTime)) {
        setNerve(&NrvPressureBase::PressureBaseNrvShot::sInstance);
    }
}

void PressureBase::exePrepareToShot() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ShotStart", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvPressureBase::PressureBaseNrvShot::sInstance);
    }
}

void PressureBase::exeShot() {
    if (MR::isFirstStep(this)) {
        if (_B0) {
            MR::startBck(this, "ShortShot", nullptr);
        }
        else {
            MR::startBck(this, "Shot", nullptr);
        }
    }

    if (_B0) {
        if (MR::isStep(this, 0x36)) {
            shotBullet(mBallSpeed);
        }
    }
    else {
        if (MR::isStep(this, 0x10)) {
            shotBullet(mBallSpeed);
        }
    }

    if (MR::isBckStopped(this)) {
        if (mGroup != nullptr) {
            setNerve(&NrvPressureBase::PressureBaseNrvSyncWait::sInstance);
        }
        else {
            setNerve(&NrvPressureBase::PressureBaseNrvWait::sInstance);
        }
    }
}

void PressureBase::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2) || MR::isSensorEnemy(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

bool PressureBase::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
    return MR::isMsgStarPieceReflect(msg);
}

bool PressureBase::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    if (msg == 0x68) {
        bool v5 = false;
        if (isNerve(&NrvPressureBase::PressureBaseNrvRelaxStart::sInstance) || isNerve(&NrvPressureBase::PressureBaseNrvRelax::sInstance)) {
            v5 = true;
        }

        if (v5) {
            return false;
        }

        setNerve(&NrvPressureBase::PressureBaseNrvWait::sInstance);
        return true;
    }

    return false;
}

void PressureBase::startWait() {
    if (isNerve(&NrvPressureBase::PressureBaseNrvRelax::sInstance)) {
        setNerve(&NrvPressureBase::PressureBaseNrvWaitStart::sInstance);
    }
}

void PressureBase::startRelax() {
    bool dontStartRelax = false;

    if (isNerve(&NrvPressureBase::PressureBaseNrvRelaxStart::sInstance) || isNerve(&NrvPressureBase::PressureBaseNrvRelax::sInstance)) {
        dontStartRelax = true;
    }

    if (!dontStartRelax) {
        MR::startSound(this, "SE_OJ_W_PRESS_HEAD_OFF", -1, -1);
        setNerve(&NrvPressureBase::PressureBaseNrvRelaxStart::sInstance);
    }
}

void PressureBase::initBullet(const JMapInfoIter &) {

}

bool PressureBase::shotBullet(f32) {
    return false;
}

// PressureBase::calcJointCannonV

bool PressureBase::isShotTypeOnGravity() const {
    return mShotType == 0;
}

bool PressureBase::isShotTypeFollow() const {
    return mShotType == 2;
}

namespace NrvPressureMessenger {
    INIT_NERVE(PressureMessengerNrvSync);
};

namespace NrvPressureBase {
    INIT_NERVE(PressureBaseNrvRelaxStart);
    INIT_NERVE(PressureBaseNrvWaitStart);
    INIT_NERVE(PressureBaseNrvRelax);
    INIT_NERVE(PressureBaseNrvSyncWait);
    INIT_NERVE(PressureBaseNrvFirstWait);
    INIT_NERVE(PressureBaseNrvWait);
    INIT_NERVE(PressureBaseNrvPrepareToShot);
    INIT_NERVE(PressureBaseNrvShot);

    void PressureBaseNrvShot::execute(Spine *pSpine) const {
        PressureBase* pressure = reinterpret_cast<PressureBase*>(pSpine->mExecutor);
        pressure->exeShot();
    }

    void PressureBaseNrvPrepareToShot::execute(Spine *pSpine) const {
        PressureBase* pressure = reinterpret_cast<PressureBase*>(pSpine->mExecutor);
        pressure->exePrepareToShot();
    }

    void PressureBaseNrvWait::execute(Spine *pSpine) const {
        PressureBase* pressure = reinterpret_cast<PressureBase*>(pSpine->mExecutor);
        pressure->exeWait();
    }

    void PressureBaseNrvFirstWait::execute(Spine *pSpine) const {
        PressureBase* pressure = reinterpret_cast<PressureBase*>(pSpine->mExecutor);

        if (MR::isStep(pressure, pressure->mWaitTime)) {
            pressure->setNerve(&NrvPressureBase::PressureBaseNrvPrepareToShot::sInstance);
        }
    }

    void PressureBaseNrvSyncWait::execute(Spine *pSpine) const {

    }

    void PressureBaseNrvRelax::execute(Spine *pSpine) const {
        PressureBase* pressure = reinterpret_cast<PressureBase*>(pSpine->mExecutor);

        if (MR::isFirstStep(pressure)) {
            pressure->_9C = -45.0f;
        }
    }

    void PressureBaseNrvWaitStart::execute(Spine *pSpine) const {
        PressureBase* pressure = reinterpret_cast<PressureBase*>(pSpine->mExecutor);
        pressure->exeBound();
    }

    void PressureBaseNrvRelaxStart::execute(Spine *pSpine) const {
        PressureBase* pressure = reinterpret_cast<PressureBase*>(pSpine->mExecutor);
        pressure->exeBound();
    }
};

namespace NrvPressureMessenger {
    void PressureMessengerNrvSync::execute(Spine *pSpine) const {
        PressureMessenger* mess = reinterpret_cast<PressureMessenger*>(pSpine->mExecutor);
        mess->exeSync();
    }
};

PressureMessenger::~PressureMessenger() {

}