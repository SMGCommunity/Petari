#include <revolution.h>
#include "Game/MapObj/PunchingKinoko.hpp"

PunchingKinoko::PunchingKinoko(const char *pName) : LiveActor(pName),
    mGroundChecker(nullptr), mScaleController(nullptr), mDelegator(nullptr), _98(-1), _9C(0, 0, 0), _A8(0, 1, 0) {
        mStarPointerHitCoolDown = 0;
        _B8 = true;
        mInvincibleHitCoolDown = -1;
}

void PunchingKinoko::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("PunchingKinoko", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObj(this);
    MR::calcGravity(this);
    _9C.set(mPosition);

    mGroundChecker = new GroundChecker("頭コリジョン", 70.0f, 0.0f);
    MR::calcPositionUpOffset(&mGroundChecker->mPosition, this, 130.0f);
    MR::resetPosition(mGroundChecker);
    MR::onCalcGravity(mGroundChecker);

    mScaleController = new AnimScaleController(nullptr);

    TVec3f *groundCheckerPos = &mGroundChecker->mPosition;
    MR::initStarPointerTargetAtPos(this, 70.0f, groundCheckerPos, TVec3f(0, 0, 0));

    initShadow();
    initSensor();
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initCamera(rIter);
    initJointControl();
    initNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance);

    makeActorAppeared();

    if (MR::useStageSwitchReadB(this, rIter) != 0) {
        MR::FunctorV0M<PunchingKinoko *, void (PunchingKinoko::*)()> functor = MR::Functor<PunchingKinoko>(this, &PunchingKinoko::kill);
        MR::listenStageSwitchOnB(this, functor);
    }
    MR::useStageSwitchSleep(this, rIter);
}

void PunchingKinoko::initSensor() {
    initHitSensor(2);
    MR::addHitSensorAtJointEnemy(this, "Head", "Ball", 0x10, 9.18355e-41f, TVec3f(0.0174533f, 0.0174533f, 0.0174533f));
    MR::addHitSensorEnemy(this, "Body", 0x10, 9.18355e-41f, TVec3f(0.0174533f, 9.18355e-41f, 0.0174533f));
}

void PunchingKinoko::initShadow() {
    MR::initShadowController(this, 3);
    MR::addShadowVolumeSphere(this, "体", 10.0f);
    MR::onCalcShadowOneTime(this, "体");
    MR::addShadowVolumeSphere(this, "頭", 10.0f);
    MR::setShadowDropPositionPtr(this, "頭", &mGroundChecker->mPosition);
    MR::setShadowDropDirectionPtr(this, "頭", &mGroundChecker->mGravity);
    MR::addShadowVolumeLine(this, "つた", this, "体", 10.0f, this, "頭", 10.0f);
}

void PunchingKinoko::initCamera(const JMapInfoIter &rIter) {
    MR::getJMapInfoArg7WithInit(rIter, &_98);
    if (_98 != -1) {
        MR::declareCameraRegisterVec(this, _98, &_9C);
    }
}

void PunchingKinoko::initJointControl() {
    mDelegator = MR::createJointDelegatorWithNullChildFunc(this, &PunchingKinoko::ballMtxCallBack, "Ball");
}

bool PunchingKinoko::ballMtxCallBack(TPos3f *a1, const JointControllerInfo &joint) {
    TVec3f stack_8;
    PSVECSubtract(mGroundChecker->mPosition.toCVec(), mPosition.toCVec(), stack_8.toVec());
    MR::orthogonalize(a1);
    if (!MR::normalizeOrZero(&stack_8)) {
        MR::turnMtxToYDirRate(a1, stack_8, 1.0f);
    }

    TPos3f stack_14;
    stack_14.identity();

    MR::scaleMtxToDir(&stack_14, _A8, mScaleController->_C);
    a1->concat(stack_14, *a1);

    TVec3f *groundCheckerPos = &mGroundChecker->mPosition;
    a1->mMtx[0][0] = groundCheckerPos->x;
    a1->mMtx[1][1] = groundCheckerPos->y;
    a1->mMtx[2][2] = groundCheckerPos->z;

    return true;
}

void PunchingKinoko::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    mGroundChecker->makeActorAppeared();
}

void PunchingKinoko::kill() {
    LiveActor::kill();
}

void PunchingKinoko::makeActorDead() {
    LiveActor::makeActorDead();
    mGroundChecker->makeActorDead();
}

void PunchingKinoko::control() {
    if (mStarPointerHitCoolDown > 0) {
        mStarPointerHitCoolDown -= 1;
    }
    mScaleController->update();
    mGroundChecker->movement();
    MR::reboundVelocityFromCollision(mGroundChecker, 0.0f, 0.0f, 1.0f);
    if (mInvincibleHitCoolDown > -1) {
        mInvincibleHitCoolDown -= 1;
    }
}

void PunchingKinoko::calcAndSetBaseMtx() {
    TPos3f stack_14;
    TVec3f stack_8;
    stack_8.setNegatedInline(mGravity);
    MR::makeMtxUpNoSupportPos(&stack_14, stack_8, mPosition);
    MR::setBaseTRMtx(this, stack_14);
    mDelegator->registerCallBack();
}

void PunchingKinoko::attackSensor(HitSensor *pMySensor, HitSensor *pOtherSensor) {
    if (!isCrushed()) {
        if (pMySensor == getSensor("Body")) {
            if (isNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance)) {
                MR::sendMsgPush(pOtherSensor, pMySensor);
            }
        }
        else if (!isNerve(&NrvPunchingKinoko::PunchingKinokoNrvCrushedEnd::sInstance) || !MR::isSensorPlayer(pOtherSensor) || !MR::sendMsgEnemyAttackFlipWeakJump(pOtherSensor, pMySensor)) {
            TVec3f stack_3C;
            TVec3f stack_30;
            f32 stack_8;
            MR::separateScalarAndDirection(&stack_8, &stack_3C, mGroundChecker->mVelocity);
            bool hit = false;
            if (MR::isSensorPlayer(pOtherSensor)) {
                if (isEnableHitPlayer()) {
                    if (stack_8 >= 30.0f) {
                        if (stack_8 >= 45.0f) {
                            TVec3f stack_24(stack_3C);
                            stack_24.scaleInline(70.0f);
                            hit = MR::sendMsgEnemyAttackFlipMaximumToDir(pOtherSensor, pMySensor, stack_24);
                        }
                        else {
                            TVec3f stack_18(stack_3C);
                            stack_18.scale(70.0f);
                            hit = MR::sendMsgEnemyAttackFlipToDir(pOtherSensor, pMySensor, stack_18);
                        }
                        if (hit) {
                            MR::startSoundPlayer("SE_PM_WALL_HIT_BODY", -1);
                            MR::scatterStarPiecePlayer(10);
                        }
                    }
                    else {
                        if (stack_8 >= 15.0f) {
                            hit = MR::sendMsgEnemyAttackFlipWeak(pOtherSensor, pMySensor);
                        } else {
                            MR::sendMsgPush(pOtherSensor, pMySensor);
                        }
                    }
                }
                else {
                    MR::sendMsgPush(pOtherSensor, pMySensor);
                }
            }
            else {
                if (isEnableEnemyAttack()) {
                    hit = MR::sendMsgToEnemyAttackBlow(pOtherSensor, pMySensor);
                }
                if (!hit) {
                    MR::sendMsgPush(pOtherSensor, pMySensor);
                }
            }
            if (hit) {
                MR::emitEffectHitBetweenSensors(this, pMySensor, pOtherSensor, 0.0f, "Hit");
                MR::calcSensorDirectionNormalize(&stack_30, pOtherSensor, pMySensor);
                f32 dot = mGroundChecker->mVelocity.dot(stack_30) * 1.6f;
                TVec3f stack_C(stack_30);
                stack_C.scale(dot);
                TVec3f *groundvel = &mGroundChecker->mVelocity;
                PSVECSubtract(groundvel->toCVec(), stack_C.toVec(), groundvel->toVec());
                mGroundChecker->mVelocity.scaleInline(0.3f);
                setNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance);
            }
        }
    }
}

bool PunchingKinoko::receiveMsgPlayerAttack(u32 a1, HitSensor *pOtherSensor, HitSensor *pMySensor) {
    if (isCrushed()) {
        return false;
    }

    if (pMySensor == getSensor("Body")) {
        return false;
    }

    if (MR::isMsgInvincibleAttack(a1)) {
        if (mInvincibleHitCoolDown < 0) {
            MR::startBlowHitSound(this);
            mInvincibleHitCoolDown = 10;
        }
        return requestEnemyBlow(pOtherSensor, pMySensor);
    }

    if (MR::isMsgPlayerHipDrop(a1)) {
        return requestCrush();
    }

    if (MR::isMsgLockOnStarPieceShoot(a1)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(a1)) {
        TVec3f stack_14;
        MR::calcSensorDirectionNormalize(&stack_14, pOtherSensor, pMySensor);
        f32 dot = mGroundChecker->mVelocity.dot(stack_14);
        if (dot < 15.0f) {
            TVec3f stack_8(stack_14);
            stack_8.scale(15.0f);
            mGroundChecker->mVelocity.add(stack_8);
        }
        MR::startSound(this, "SE_OJ_PNC_KINOKO_BOUND", -1, -1);
        return true;
    }

    if (MR::isMsgPlayerSpinAttack(a1)) {
        return requestPunch(pOtherSensor, pMySensor);
    }

    if (MR::isMsgPlayerTrample(a1)) {
        return requestTrample(pOtherSensor, pMySensor);
    }

    return false;
}

bool PunchingKinoko::receiveMsgEnemyAttack(u32 a1, HitSensor *pOtherSensor, HitSensor *pMySensor) {
    if (isCrushed()) {
        return false;
    }

    if (pMySensor == getSensor("Body")) {
        return false;
    }

    if (MR::isMsgToEnemyAttackTrample(a1)) {
        return requestCrush();
    }

    if (MR::isMsgToEnemyAttackBlow(a1)) {
        return requestEnemyBlow(pOtherSensor, pMySensor);
    }

    return false;
}

bool PunchingKinoko::receiveMsgPush(HitSensor *pOtherSensor, HitSensor *pMySensor) {
    if (isCrushed()) {
        return false;
    }

    if (pMySensor == getSensor("Body")) {
        return false;
    }

    TVec3f stack_14;
    MR::calcSensorHorizonNormalize(&stack_14, mGravity, pOtherSensor, pMySensor);
    f32 fVar1 = stack_14.dot(mGroundChecker->mVelocity);
    if (fVar1 < 0.0f) {
        fVar1 = 3.0f;
    }
    else {
        fVar1 = 3.0f - fVar1;
    }

    if (fVar1 > 0.0f) {
        TVec3f stack_8(stack_14);
        stack_8.scale(fVar1);
        mGroundChecker->mVelocity.add(stack_8);
        return true;
    }

    return false;
}

bool PunchingKinoko::requestPunch(HitSensor *pOtherSensor, HitSensor *pMySensor) {
    if (isCrushed()) {
        return false;
    }

    if (pMySensor == getSensor("Body")) {
        return false;
    }

    if (isEnablePunched()) {
        MR::invalidateClipping(this);
        TVec3f stack_14;
        MR::calcSensorHorizonNormalize(&stack_14, mGravity, pOtherSensor, pMySensor);
        TVec3f stack_8(stack_14);
        stack_8.scale(50.0f);
        mGroundChecker->mVelocity.add(stack_8);
        MR::startSpinHitSound(this);
        MR::startBlowHitSound(this);
        MR::tryRumblePadStrong(this, 0);
        MR::stopScene(5);
        setNerve(&NrvPunchingKinoko::PunchingKinokoNrvPunched::sInstance);

        return true;
    }

    return false;
}

bool PunchingKinoko::requestEnemyBlow(HitSensor *pOtherSensor, HitSensor *pMySensor) {
    if (!isEnableBlowed()) {
        return false;
    }

    MR::invalidateClipping(this);
    TVec3f stack_14;
    MR::calcSensorHorizonNormalize(&stack_14, mGravity, pOtherSensor, pMySensor);
    TVec3f stack_8(stack_14);
    stack_8.scale(25.0f);
    mGroundChecker->mVelocity.add(stack_8);
    MR::startSound(this, "SE_OJ_PNC_KINOKO_HIT_SELF", -1, -1);
    setNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance);

    return true;
}

bool PunchingKinoko::requestTrample(HitSensor *pOtherSensor, HitSensor *pMySensor) {
    if (isEnableTrample()) {
        TVec3f stack_14;
        MR::calcSensorDirectionNormalize(&stack_14, pOtherSensor, pMySensor);

        TVec3f stack_8(stack_14);
        stack_8.scale(5.0f);

        mGroundChecker->mVelocity.add(stack_8);
        return true;
    }
    return false;
}

bool PunchingKinoko::requestCrush() {
    if (isEnableCrushed()) {
        MR::invalidateClipping(this);
        setNerve(&NrvPunchingKinoko::PunchingKinokoNrvCrushed::sInstance);
        return true;
    }
    return false;
}

void PunchingKinoko::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Normal");
        MR::validateClipping(this);
        _B8 = true;
    }

    TVec3f stack_38;
    TVec3f stack_2C;
    MR::calcPositionUpOffset(&stack_38, this, 130.0f);

    TVec3f *groundCheckerPos = &mGroundChecker->mPosition;
    TVec3f stack_20;
    TVec3f stack_14(stack_38);
    PSVECSubtract(stack_14.toCVec(), groundCheckerPos->toCVec(), stack_14.toVec());

    stack_20 = stack_14;
    stack_20.scale(0.008f);
    MR::addVelocity(mGroundChecker, stack_20);
    MR::attenuateVelocity(mGroundChecker, 0.94f);

    if (MR::isStarPointerPointing2POnPressButton(this, nullptr, false, false)) {
        if (mStarPointerHitCoolDown == 0 && !_B8 && MR::getStarPointerScreenSpeed(1) > 5.0f) {
            stack_2C.set(0.0f, 0.0f, 0.0f);
            if (MR::calcStarPointerWorldVelocityDirectionOnPlane(&stack_2C, mGroundChecker->mPosition, mGravity, 1)) {
                TVec3f stack_8(stack_2C);
                stack_8.scale(35.0f);
                mGroundChecker->mVelocity.add(stack_8);
                MR::startBlowHitSound(this);
                MR::start2PAttackAssistSound();
                MR::tryRumblePadWeak(this, 1);
                mStarPointerHitCoolDown = 30;
                setNerve(&NrvPunchingKinoko::PunchingKinokoNrvPointSnaped::sInstance);
                return;
            }
        }
        _B8 = true;
    }
    else {
        _B8 = false;
    }
    MR::vecBlend(_9C, mPosition, &_9C, 0.05f);
}

void PunchingKinoko::exeSwing() {
    float var2 = MR::calcVelocityLength(mGroundChecker);
    TVec3f stack_20;
    MR::calcPositionUpOffset(&stack_20, this, 130.0f);

    TVec3f *groundCheckerPos = &mGroundChecker->mPosition;
    TVec3f stack_14(stack_20);
    PSVECSubtract(stack_14.toCVec(), groundCheckerPos->toCVec(), stack_14.toVec());

    TVec3f stack_8(stack_14);
    stack_8.scale(0.008f);

    MR::addVelocity(mGroundChecker, stack_8);
    MR::attenuateVelocity(mGroundChecker, 0.97f);
    float var3 = MR::calcVelocityLength(mGroundChecker);

    const float f0 = 20.0f;
    if (var2 < f0 && var3 >= f0) {
        MR::startSound(this, "SE_OJ_PNC_KINOKO_BOUND", -1, -1);
    }

    MR::vecBlend(_9C, mPosition, &_9C, 0.05f);

    if (var3 < 40.0f && PSVECMag(stack_14.toCVec()) < 50.0f) {
        setNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance);
    }
}

void PunchingKinoko::exePointSnaped() {
    MR::startLevelSound(this, "SE_OJ_LV_PNC_KINOKO_PUNCHED", -1, -1, -1);
    addVelocityKeepHeight();
    MR::attenuateVelocity(mGroundChecker, 0.99f);
    if (MR::isGreaterStep(this, 6)) {
        setNerve(&NrvPunchingKinoko::PunchingKinokoNrvSwing::sInstance);
    }
}

void PunchingKinoko::exePunched() {
    MR::isFirstStep(this);
    MR::startLevelSound(this, "SE_OJ_LV_PNC_KINOKO_PUNCHED", -1, -1, -1);
    addVelocityKeepHeight();
    MR::attenuateVelocity(mGroundChecker, 0.99f);
    HitSensor *sensor = getSensor("Head");
    MR::sendMsgEnemyAttackToBindedSensor(mGroundChecker, sensor);
    _9C.set(mGroundChecker->mPosition);
    if (MR::isGreaterStep(this, 5)) {
        setNerve(&NrvPunchingKinoko::PunchingKinokoNrvPunchedBrake::sInstance);
    }
}

void PunchingKinoko::exePunchedBrake() {
    addVelocityKeepHeight();
    MR::attenuateVelocity(mGroundChecker, 0.9f);
    MR::startLevelSound(this, "SE_OJ_LV_PNC_KINOKO_PUNCHED", -1, -1, -1);
    _9C.set(mGroundChecker->mPosition);
    if (!MR::isGreaterStep(this, 40)) {
        HitSensor *sensor = getSensor("Head");
        if (!MR::sendMsgEnemyAttackToBindedSensor(mGroundChecker, sensor)) {
            return;
        }
    }
    MR::startSound(this, "SE_OJ_PNC_KINOKO_RETURN", -1, -1);
    setNerve(&NrvPunchingKinoko::PunchingKinokoNrvSwing::sInstance);
}

void PunchingKinoko::exeHitted() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_PNC_KINOKO_HIT_SELF", -1, -1);
    }
    addVelocityKeepHeight();
    MR::attenuateVelocity(mGroundChecker, 0.99f);
    if (MR::isGreaterStep(this, 5)) {
        setNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance);
    }
}

void PunchingKinoko::exeCrushed() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Press");
        if (MR::isShadowProjected(this, "頭")) {
            MR::getShadowProjectionNormal(this, "頭", &_A8);
            MR::getShadowProjectionPos(this, "頭", &mGroundChecker->mPosition);
            TVec3f stack_8(_A8);
            stack_8.scale(20.0f);
            mGroundChecker->mPosition.add(stack_8);
        }
        else {
            _A8.set(mGravity);
        }
        MR::offBind(mGroundChecker);
        mScaleController->startCrush();
        MR::startSound(this, "SE_OJ_PNC_KINOKO_CRASH", -1, -1);
    }
    MR::zeroVelocity(mGroundChecker);
    if (MR::isGreaterStep(this, 180)) {
        setNerve(&NrvPunchingKinoko::PunchingKinokoNrvCrushedEnd::sInstance);
    }
}

void PunchingKinoko::exeCrushedEnd() {
    MR::isFirstStep(this);
    if (MR::isStep(this, 10)) {
        MR::startBrk(this, "Revival");
        mScaleController->startAnim();
        MR::startSound(this, "SE_OJ_PNC_KINOKO_RECOVER", -1, -1);
    }
    TVec3f stack_20;
    MR::calcPositionUpOffset(&stack_20, this, 130.0f);
    TVec3f *groundCheckerPos = &mGroundChecker->mPosition;
    TVec3f stack_14;
    TVec3f stack_8(stack_20);
    PSVECSubtract(stack_8.toCVec(), groundCheckerPos->toCVec(), stack_8.toVec());
    stack_14 = stack_8;
    stack_14.scale(0.008f);
    MR::addVelocity(mGroundChecker, stack_14);
    MR::attenuateVelocity(mGroundChecker, 0.94f);
    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance);
        MR::onBind(mGroundChecker);
    }
}

void PunchingKinoko::addVelocityKeepHeight() {
    if (MR::isShadowProjected(this, "頭")) {
        TVec3f stack_8;
        MR::getShadowProjectionPos(this, "頭", &stack_8);
        MR::addVelocityKeepHeight(mGroundChecker, stack_8, 130.0f, 0.5f, 60.0f);
    }
}

bool PunchingKinoko::isEnablePunched() const {
    if (isNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance) ||
        isNerve(&NrvPunchingKinoko::PunchingKinokoNrvSwing::sInstance)) {
        return true;
    }
    return false;
}

bool PunchingKinoko::isEnableHitPlayer() const {
    return isNerve(&NrvPunchingKinoko::PunchingKinokoNrvSwing::sInstance);
}

bool PunchingKinoko::isEnableEnemyAttack() const {
    if (isNerve(&NrvPunchingKinoko::PunchingKinokoNrvSwing::sInstance) ||
        isNerve(&NrvPunchingKinoko::PunchingKinokoNrvPunched::sInstance) ||
        isNerve(&NrvPunchingKinoko::PunchingKinokoNrvPunchedBrake::sInstance) ||
        isNerve(&NrvPunchingKinoko::PunchingKinokoNrvPointSnaped::sInstance)) {
            return true;
    }
    return false;
}

bool PunchingKinoko::isEnableCrushed() const {
    if (isNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance) ||
        isNerve(&NrvPunchingKinoko::PunchingKinokoNrvCrushedEnd::sInstance)) {
        return true;
    }
    return false;
}

bool PunchingKinoko::isEnableTrample() const {
    if (isNerve(&NrvPunchingKinoko::PunchingKinokoNrvCrushed::sInstance) ||
        isNerve(&NrvPunchingKinoko::PunchingKinokoNrvCrushedEnd::sInstance)) {
        return false;
    }
    return true;
}

bool PunchingKinoko::isEnableBlowed() const {
    return isNerve(&NrvPunchingKinoko::PunchingKinokoNrvWait::sInstance);
}

bool PunchingKinoko::isCrushed() const {
    return isNerve(&NrvPunchingKinoko::PunchingKinokoNrvCrushed::sInstance);
}

namespace NrvPunchingKinoko {
    INIT_NERVE(PunchingKinokoNrvWait);
    INIT_NERVE(PunchingKinokoNrvSwing);
    INIT_NERVE(PunchingKinokoNrvPunched);
    INIT_NERVE(PunchingKinokoNrvPunchedBrake);
    INIT_NERVE(PunchingKinokoNrvHitted);
    INIT_NERVE(PunchingKinokoNrvPointSnaped);
    INIT_NERVE(PunchingKinokoNrvCrushed);
    INIT_NERVE(PunchingKinokoNrvCrushedEnd);

    void PunchingKinokoNrvWait::execute(Spine *pSpine) const {
        PunchingKinoko* kinoko = reinterpret_cast<PunchingKinoko*>(pSpine->mExecutor);
        kinoko->exeWait();
    };
    void PunchingKinokoNrvSwing::execute(Spine *pSpine) const {
        PunchingKinoko* kinoko = reinterpret_cast<PunchingKinoko*>(pSpine->mExecutor);
        kinoko->exeSwing();
    };
    void PunchingKinokoNrvPunched::execute(Spine *pSpine) const {
        PunchingKinoko* kinoko = reinterpret_cast<PunchingKinoko*>(pSpine->mExecutor);
        kinoko->exePunched();
    };
    void PunchingKinokoNrvPunchedBrake::execute(Spine *pSpine) const {
        PunchingKinoko* kinoko = reinterpret_cast<PunchingKinoko*>(pSpine->mExecutor);
        kinoko->exePunchedBrake();
    };
    void PunchingKinokoNrvHitted::execute(Spine *pSpine) const {
        PunchingKinoko* kinoko = reinterpret_cast<PunchingKinoko*>(pSpine->mExecutor);
        kinoko->exeHitted();
    };
    void PunchingKinokoNrvPointSnaped::execute(Spine *pSpine) const {
        PunchingKinoko* kinoko = reinterpret_cast<PunchingKinoko*>(pSpine->mExecutor);
        kinoko->exePointSnaped();
    };
    void PunchingKinokoNrvCrushed::execute(Spine *pSpine) const {
        PunchingKinoko* kinoko = reinterpret_cast<PunchingKinoko*>(pSpine->mExecutor);
        kinoko->exeCrushed();
    };
    void PunchingKinokoNrvCrushedEnd::execute(Spine *pSpine) const {
        PunchingKinoko* kinoko = reinterpret_cast<PunchingKinoko*>(pSpine->mExecutor);
        kinoko->exeCrushedEnd();
    };
};

PunchingKinoko::~PunchingKinoko() {

}
