#include "Game/Enemy/CocoSambo.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/SamboFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvCocoSamboHead {
    NEW_NERVE(CocoSamboHeadNrvHeadConnectedBody, CocoSamboHead, HeadConnectedBody);
    NEW_NERVE(CocoSamboHeadNrvHeadFall, CocoSamboHead, Fall);
    NEW_NERVE(CocoSamboHeadNrvHeadFallLand, CocoSamboHead, FallLand);
    NEW_NERVE(CocoSamboHeadNrvHeadSwoon, CocoSamboHead, Swoon);
    NEW_NERVE(CocoSamboHeadNrvHeadSwoonEnd, CocoSamboHead, SwoonEnd);
    NEW_NERVE(CocoSamboHeadNrvHeadBlow, CocoSamboHead, Blow);
};  // namespace NrvCocoSamboHead

namespace {
    const char* cPointingJointName[] = {"Spine1", "Spine2", "Spine3", "Head"};
    const Vec cHeadSensorOffset = {60.0f, 0.0f, 0.0f};
    const Vec cBlowVelocity = {0.0f, 20.0f, -100.0f};
    const Vec cSensorOffset = {40.0f, 0.0f, 0.0f};
    const s32 cPointingActorNum = 4;
};  // namespace

CocoSamboHead::CocoSamboHead(LiveActor* pHost) : PartsModel(pHost, "ココサンボ[頭]", "CocoSamboHead", 0, 18, 0) {
    mFrontVec.set(0.0f, 0.0f, 1.0f);
}

void CocoSamboHead::init(const JMapInfoIter& rIter) {
    TVec3f sensorOffs(cHeadSensorOffset);
    sensorOffs.scale(mScale.x);
    initHitSensor(2);
    MR::addHitSensorAtJoint(this, "body", "Head", 37, 8, (100.0f * mScale.x), sensorOffs);
    MR::addHitSensorAtJoint(this, "trample", "Head", 37, 8, (150.0f * mScale.x), sensorOffs);
    initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, -90.0f, -90.0f), "Head");
    initNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadConnectedBody::sInstance);
    PartsModel::init(rIter);
}

void CocoSamboHead::kill() {
    MR::startSound(this, "SE_EM_EXPLODE_S");
    LiveActor::kill();
    MR::emitEffect(this, "DeathSmokeHead");
}

void CocoSamboHead::calcAndSetBaseMtx() {
    if (mCalcOwnMtx != false) {
        PartsModel::calcAndSetBaseMtx();
    } else {
        TVec3f grav;
        MR::calcGravityVector(this, &grav, nullptr, 0);
        grav.negate();

        TPos3f mtx;

        if (MR::isSameDirection(mFrontVec, grav)) {
            MR::makeMtxUpNoSupportPos(&mtx, grav, mPosition);
        } else {
            MR::makeMtxUpFrontPos(&mtx, grav, mFrontVec, mPosition);
        }

        MR::setBaseTRMtx(this, mtx);
    }
}

// CocoSamboHead::attackSensor
// CocoSamboHead::receiveMsgPlayerAttack
// CocoSamboHead::receiveMsgEnemyAttack

void CocoSamboHead::updateFrontVecToPlayer(const TVec3f& rVec) {
    TVec3f v4 = -rVec;
    TVec3f v5;
    v5.sub(*MR::getPlayerPos(), mPosition);

    if (MR::isNearZero(v5) || MR::isSameDirection(v4, v5)) {
        if (MR::getMaxAbsElementIndex(v4) == 2) {
            v5.set< f32 >(0.0f, 1.0f, 0.0f);
        } else {
            v5.set< f32 >(0.0f, 0.0f, 1.0f);
        }
    }

    MR::normalize(&v5);
    MR::vecKillElement(v5, v4, &mFrontVec);
    MR::normalize(&mFrontVec);
}

void CocoSamboHead::exeFall() {
    TVec3f gravity(mHost->mGravity);
    TVec3f v16 = gravity * 2.0f;

    if (MR::isFirstStep(this)) {
        mFixedPos->copyTrans(&mPosition);
        mRotation.zero();
        mCalcOwnMtx = false;
        MR::startBck(this, "Fall", nullptr);
        updateFrontVecToPlayer(gravity);

        TVec3f v15 = mHost->mPosition - mPosition;
        TVec3f up;
        up = -gravity;

        MR::vecKillElement(v15, up, &v15);
        mVelocity = v15 * (1.0f / 45.0f) + ((up * up.dot(v15)) * 2.0f - ((v16 * 45.0f)) * (1.0f / 90.0f));
    }

    if (MR::isStep(this, 45)) {
        mPosition.set< f32 >(mHost->mPosition);
        mVelocity.zero();
        setNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadFallLand::sInstance);
    } else {
        mVelocity += v16;
    }
}

void CocoSamboHead::exeFallLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Land", nullptr);
        MR::startSound(this, "SE_EM_SFSAMBO_HEAD_LAND");
        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraWeak();
    }

    MR::setNerveAtBckStopped(this, &NrvCocoSamboHead::CocoSamboHeadNrvHeadSwoon::sInstance);
}

void CocoSamboHead::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swoon", nullptr);
    }

    if (MR::isStep(this, 0xB4)) {
        setNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadSwoonEnd::sInstance);
    }
}

void CocoSamboHead::exeSwoonEnd() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::startBck(this, "SwoonEnd", nullptr);
    }

    if (MR::isStep(this, 0x5F)) {
        MR::invalidateShadow(mHost, nullptr);
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormal();
    }

    MR::setNerveAtBckStopped(this, &NrvCocoSamboHead::CocoSamboHeadNrvHeadConnectedBody::sInstance);
}

void CocoSamboHead::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::calcGravity(this);
        updateFrontVecToPlayer(mGravity);
        TVec3f up = -mGravity;
        TPos3f mtx;
        mtx.identity();
        MR::makeMtxUpFront(&mtx, up, mFrontVec);
        mtx.mult33(TVec3f(::cBlowVelocity), mVelocity);
        MR::startBck(this, "Blow", nullptr);
    }
    TVec3f result;
    result.scale(1.0f, mGravity);
    mVelocity.add(mVelocity, result);
}

void CocoSamboHead::exeHeadConnectedBody() {
}

namespace NrvCocoSambo {
    NEW_NERVE(CocoSamboNrvHideWait, CocoSambo, HideWait);
    NEW_NERVE(CocoSamboNrvAppear, CocoSambo, Appear);
    NEW_NERVE(CocoSamboNrvWait, CocoSambo, Wait);
    NEW_NERVE(CocoSamboNrvHide, CocoSambo, Hide);
    NEW_NERVE(CocoSamboNrvAttack, CocoSambo, Attack);
    NEW_NERVE(CocoSamboNrvAttackInterval, CocoSambo, AttackInterval);
    NEW_NERVE(CocoSamboNrvFallHead, CocoSambo, FallHead);
    NEW_NERVE(CocoSamboNrvFallHeadHide, CocoSambo, FallHeadHide);
    NEW_NERVE(CocoSamboNrvRecoverWait, CocoSambo, RecoverWait);
    NEW_NERVE(CocoSamboNrvRecover, CocoSambo, Recover);
    NEW_NERVE(CocoSamboNrvPressY, CocoSambo, PressY);
    NEW_NERVE(CocoSamboNrvBlow, CocoSambo, Blow);
    NEW_NERVE_ONEND(CocoSamboNrvDpdPointing, CocoSambo, DpdPointing, DpdPointing);
};  // namespace NrvCocoSambo

CocoSambo::CocoSambo(const char* pName)
    : LiveActor(pName), mHead(), mHitEffectScale(gZeroVec), mFrontVec(0.0f, 0.0f, 1.0f), mUpVec(0.0f, 1.0f, 0.0f), mPlayerSearchDistance(1800.0f),
      mAnimScaleParam(), mAnimScaleController(), mDpdPointingEndNerve(), mPointingActorArray() {
}

void CocoSambo::init(const JMapInfoIter& rIter) {
    initMapToolInfo(rIter);
    MR::calcGravity(this);
    initModelManagerWithAnm("CocoSamboBody", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSensor();
    initEffectKeeper(1, nullptr, false);
    MR::setEffectHostSRT(this, "Hit", &mHitEffectScale, nullptr, nullptr);
    initSound(4, false);
    MR::initShadowFromCSV(this, "Shadow");
    mHead = new CocoSamboHead(this);
    mHead->initWithoutIter();
    MR::initLightCtrl(mHead);
    mAnimScaleParam = new AnimScaleParam();
    mAnimScaleController = SamboFunction::createAnimScaleController(mAnimScaleParam);
    mPointingActorArray = new LiveActor*[::cPointingActorNum];
    for (s32 i = 0; i < ::cPointingActorNum; i++) {
        mPointingActorArray[i] = new LiveActor("ポインティング用アクター");
        mPointingActorArray[i]->initWithoutIter();
        MR::invalidateClipping(mPointingActorArray[i]);
        MR::initStarPointerTargetAtMtx(mPointingActorArray[i], 80.0f * mScale.x, MR::getJointMtx(this, cPointingJointName[i]),
                                       TVec3f(mScale.x * 100.0f, 0.0f, 0.0f));
        mPointingActorArray[i]->makeActorAppeared();
    }
    initNerve(&NrvCocoSambo::CocoSamboNrvHideWait::sInstance);
    appear();
}

void CocoSambo::appear() {
    setNerve(&NrvCocoSambo::CocoSamboNrvHideWait::sInstance);
    LiveActor::appear();
}

void CocoSambo::kill() {
    mHead->kill();
    LiveActor::kill();
    for (s32 i = 0; i < ::cPointingActorNum; i++) {
        mPointingActorArray[i]->kill();
    }
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

inline bool CocoSambo::isSensorTryToFallHead(HitSensor* pSensor) {
    return pSensor->isType(ATYPE_COCO_NUT) || pSensor->isType(ATYPE_KURIBO) || pSensor->isType(ATYPE_BEGOMAN);
}

bool CocoSambo::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return tryStarPieceReflect();
    }
    if (MR::isMsgInvincibleAttack(msg)) {
        return tryToFallHead(pReceiver, pSender);
    }
    return false;
}

bool CocoSambo::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isSensorTryToFallHead(pSender)) {
        if (tryToFallHead(pReceiver, pSender)) {
            return true;
        }
        return false;
    }
    return false;
}

inline bool CocoSambo::isNerveDown() {
    return isNerve(&NrvCocoSambo::CocoSamboNrvFallHead::sInstance) || isNerve(&NrvCocoSambo::CocoSamboNrvFallHeadHide::sInstance) ||
           isNerve(&NrvCocoSambo::CocoSamboNrvRecoverWait::sInstance);
}

void CocoSambo::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::makeMtxUpFrontPos(&mtx, mUpVec, mFrontVec, mPosition);
    MR::setBaseTRMtx(this, mtx);
    MR::setBaseScale(this, mAnimScaleController->_C * mScale);
}

void CocoSambo::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerveDying() && !isNerveDown()) {
        if (MR::isSensorPlayer(pReceiver)) {
            MR::sendMsgEnemyAttackStrong(pReceiver, pSender);
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }
        if (pReceiver->isType(ATYPE_KURIBO) || pReceiver->isType(ATYPE_COCO_NUT)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool CocoSambo::tryToFallHead(const HitSensor* pReceiver, const HitSensor* pSender) {
    if (!(isNerve(&NrvCocoSambo::CocoSamboNrvAppear::sInstance) || isNerve(&NrvCocoSambo::CocoSamboNrvWait::sInstance) ||
          isNerve(&NrvCocoSambo::CocoSamboNrvAttack::sInstance) || isNerve(&NrvCocoSambo::CocoSamboNrvAttackInterval::sInstance) ||
          isNerve(&NrvCocoSambo::CocoSamboNrvDpdPointing::sInstance))) {
        return false;
    }
    MR::deleteEffectAll(this);
    mHitEffectScale.lerp(pReceiver->mPosition, pSender->mPosition, 0.75f);
    setNerve(&NrvCocoSambo::CocoSamboNrvFallHead::sInstance);
    return true;
}

bool CocoSambo::tryToPress() {
    if (isNerveDying()) {
        return false;
    }
    MR::deleteEffectAll(this);
    setNerve(&NrvCocoSambo::CocoSamboNrvPressY::sInstance);
    return true;
}

bool CocoSambo::tryToBlow() {
    if (isNerveDying()) {
        return false;
    }
    MR::deleteEffectAll(this);
    setNerve(&NrvCocoSambo::CocoSamboNrvBlow::sInstance);
    return true;
}

bool CocoSambo::tryStarPieceReflect() {
    if (isNerveDying()) {
        return false;
    }
    if (!isNerve(&NrvCocoSambo::CocoSamboNrvDpdPointing::sInstance)) {
        mAnimScaleController->startHitReaction();
    }
    return true;
}

void CocoSambo::control() {
    mAnimScaleController->updateNerve();
}

void CocoSambo::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mPlayerSearchDistance);
    MR::useStageSwitchWriteDead(this, rIter);
    TRot3f mtx;
    MR::makeMtxRotate(mtx, mRotation);
    mtx.getYDir2(mUpVec);
    mtx.getZDir2(mFrontVec);
}

void CocoSambo::initSensor() {
    f32 f = mScale.x * 80.0f;
    TVec3f offset(cSensorOffset);
    offset.scale(mScale.x);
    initHitSensor(3);
    MR::addHitSensorMtx(this, "spine1", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine1"), offset);
    MR::addHitSensorMtx(this, "spine2", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine2"), offset);
    MR::addHitSensorMtx(this, "spine3", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine3"), offset);
}

void CocoSambo::dirToPlayer(f32 f) {
    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), TVec3f(f));
    if (!MR::isNearZero(vec) && !MR::isSameDirection(mUpVec, vec, 0.01f)) {
        TVec3f vec2;
        TVec3f vec3;
        vec2.set(mFrontVec);
        MR::normalize(&vec);
        MR::vecKillElement(vec, mUpVec, &vec3);
        MR::normalize(&vec3);
        if (f < 0.0f) {
        }
        MR::turnVecToVecCos(&mFrontVec, vec2, vec3, 45.511112f, mUpVec);
    }
}

bool CocoSambo::tryDpdPointing(const Nerve* pNerve) {
    if (isPointing()) {
        mDpdPointingEndNerve = pNerve;
        setNerve(&NrvCocoSambo::CocoSamboNrvDpdPointing::sInstance);
        return true;
    }
    return false;
}

bool CocoSambo::isPointing() {
    for (s32 i = 0; i < ::cPointingActorNum; i++) {
        if (MR::isStarPointerPointing2POnPressButton(mPointingActorArray[i], "弱", true, false)) {
            return true;
        }
    }
    return false;
}

void CocoSambo::exeHideWait() {
    if (MR::isFirstStep(this)) {
        startBckThisAndHead("HideWait");
        MR::invalidateShadow(this, nullptr);
        MR::invalidateHitSensors(mHead);
    }
    if (MR::isNearPlayer(this, mPlayerSearchDistance)) {
        setNerve(&NrvCocoSambo::CocoSamboNrvAppear::sInstance);
    }
}

bool CocoSambo::isNerveDying() {
    return isNerve(&NrvCocoSambo::CocoSamboNrvPressY::sInstance) || isNerve(&NrvCocoSambo::CocoSamboNrvBlow::sInstance);
}

void CocoSambo::startBckThisAndHead(const char* pBckName) {
    MR::startBck(this, pBckName, nullptr);
    MR::startBck(mHead, pBckName, nullptr);
}

void CocoSambo::exeAppear() {
    if (MR::isFirstStep(this)) {
        startBckThisAndHead("Appear");
        MR::validateHitSensors(mHead);
        dirToPlayer(180.0f);
        MR::showMaterial(mHead, "SanboNeedleMat_v");
    }

    if (MR::isStep(this, 60)) {
        MR::emitEffect(this, "CocoSamboSmoke");
        MR::startSound(this, "SE_EM_SFSAMBO_APPEAR");
        MR::validateShadow(this, nullptr);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 105)) {
        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraWeak();
    }

    dirToPlayer(1.0f);
    if (!MR::isGreaterStep(this, 105) || !tryDpdPointing(&NrvCocoSambo::CocoSamboNrvWait::sInstance)) {
        MR::setNerveAtBckStopped(this, &NrvCocoSambo::CocoSamboNrvWait::sInstance);
    }
}

void CocoSambo::exeWait() {
    if (MR::isFirstStep(this)) {
        if (!MR::isBckPlaying(this, "Wait")) {
            startBckThisAndHead("Wait");
        }
        MR::deleteEffectAll(this);
    }
    dirToPlayer(1.0f);
    if (tryDpdPointing(&NrvCocoSambo::CocoSamboNrvWait::sInstance)) {
        return;
    }
    if (MR::isNearPlayer(this, 700.0f)) {
        setNerve(&NrvCocoSambo::CocoSamboNrvAttack::sInstance);
        return;
    } else if (!MR::isNearPlayer(this, mPlayerSearchDistance + 100.0f)) {
        setNerve(&NrvCocoSambo::CocoSamboNrvHide::sInstance);
    }
}

void CocoSambo::exeHide() {
    if (MR::isFirstStep(this)) {
        startBckThisAndHead("Hide");
    }
    if (MR::isStep(this, 65)) {
        MR::emitEffect(this, "CocoSamboSmoke");
        MR::startSound(this, "SE_EM_SFSAMBO_HIDE");
        MR::invalidateShadow(this, nullptr);
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::shakeCameraWeak();
    }
    MR::setNerveAtBckStopped(this, &NrvCocoSambo::CocoSamboNrvHideWait::sInstance);
}

void CocoSambo::exeAttack() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Attack")) {
        startBckThisAndHead("Attack");
    }
    if (MR::getBckFrame(this) < 30.0f) {
        dirToPlayer(1.0f);
    }
    if (!tryDpdPointing(&NrvCocoSambo::CocoSamboNrvAttack::sInstance)) {
        if (MR::checkPassBckFrame(this, 115.0f)) {
            MR::emitEffect(mHead, "CocoSamboAttack");
            MR::tryRumblePadStrong(this, WPAD_CHAN0);
            MR::shakeCameraNormal();
        }
        MR::setNerveAtBckStopped(this, &NrvCocoSambo::CocoSamboNrvAttackInterval::sInstance);
    }
}

void CocoSambo::exeAttackInterval() {
    if (MR::isFirstStep(this)) {
        startBckThisAndHead("AttackInterval");
    }
    dirToPlayer(1.0f);
    if (tryDpdPointing(&NrvCocoSambo::CocoSamboNrvWait::sInstance)) {
        MR::setNerveAtStep(this, &NrvCocoSambo::CocoSamboNrvWait::sInstance, 60);
    }
}

void CocoSambo::exeFallHead() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit", nullptr);
        mHead->setNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadFall::sInstance);
        MR::emitEffect(this, "Hit");
        MR::startSound(this, "SE_EM_SFSAMBO_DAMAGE");
        MR::startSound(this, "SE_EM_SFSAMBO_BLOW_HEAD");
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::hideMaterial(mHead, "SanboNeedleMat_v");
    }
    if (MR::isStep(this, 2)) {
        MR::stopScene(10);
    }
    if (MR::isStep(this, 3)) {
        MR::shakeCameraNormal();
    }
    if (MR::isBckStopped(this)) {
        MR::emitEffect(this, "DeathSmokeSpine1");
        MR::emitEffect(this, "DeathSmokeSpine2");
        MR::emitEffect(this, "DeathSmokeSpine3");
        MR::startSound(this, "SE_EM_EXPLODE_S");
        setNerve(&NrvCocoSambo::CocoSamboNrvFallHeadHide::sInstance);
    }
}

void CocoSambo::exeFallHeadHide() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "HideWait");
    }
    if (mHead->isNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadConnectedBody::sInstance)) {
        setNerve(&NrvCocoSambo::CocoSamboNrvRecoverWait::sInstance);
    }
}

void CocoSambo::exeRecoverWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recover", nullptr);
        MR::setBckRate(this, 0.0f);
        MR::invalidateHitSensors(mHead);
    }
    MR::setNerveAtStep(this, &NrvCocoSambo::CocoSamboNrvRecover::sInstance, 90);
}

void CocoSambo::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 1.0f);
        MR::startBck(mHead, "Recover", nullptr);
        mHead->mCalcOwnMtx = true;
        MR::showMaterial(mHead, "SanboNeedleMat_v");
    }
    dirToPlayer(180.0f);
    MR::setNerveAtBckStopped(this, &NrvCocoSambo::CocoSamboNrvAppear::sInstance);
}

void CocoSambo::exePressY() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 0.0f);
        MR::startBck(mHead, "PressY", nullptr);
        MR::startSound(this, "SE_EM_STOMPED_S");
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }
    if (MR::isStep(this, 35)) {
        kill();
    }
}

void CocoSambo::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::invalidateShadow(this, nullptr);
        MR::tryRumblePadVeryStrong(this, WPAD_CHAN0);
        MR::startBlowHitSound(this);
        mHead->setNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadBlow::sInstance);
    }
    if (MR::isStep(this, 1)) {
        MR::stopScene(6);
    }
    if (MR::isStep(this, 2)) {
        MR::shakeCameraNormal();
    }
    if (MR::isStep(this, 20)) {
        kill();
    }
}

void CocoSambo::exeDpdPointing() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 0.0f);
        MR::setBckRate(mHead, 0.0f);
        MR::emitEffect(this, "Touch1");
        MR::emitEffect(this, "Touch2");
        MR::emitEffect(this, "Touch3");
        MR::emitEffect(mHead, "Touch");
        mAnimScaleController->startDpdHitVibration();
        MR::startDPDHitSound();
    }
    MR::startDPDFreezeLevelSound(this);
    if (!isPointing()) {
        setNerve(mDpdPointingEndNerve);
    }
}

void CocoSambo::endDpdPointing() {
    MR::setBckRate(this, 1.0f);
    MR::setBckRate(mHead, 1.0f);
    MR::deleteEffect(this, "Touch1");
    MR::deleteEffect(this, "Touch2");
    MR::deleteEffect(this, "Touch3");
    MR::deleteEffect(mHead, "Touch");
    mAnimScaleController->stopAndReset();
}
