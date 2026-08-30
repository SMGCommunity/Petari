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
};  // namespace

CocoSamboHead::CocoSamboHead(LiveActor* pHost) : PartsModel(pHost, "ココサンボ[頭]", "CocoSamboHead", 0, 18, 0) {
    mFrontVec.set(0.0f, 0.0f, 1.0f);
}

void CocoSamboHead::init(const JMapInfoIter& rIter) {
    TVec3f sensorOffs(cHeadSensorOffset);
    sensorOffs *= mScale;
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
        grav = -grav;

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
    TVec3f v5 = *MR::getPlayerPos() - mPosition;

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
        TVec3f v6 = -mGravity;
        TPos3f mtx;
        mtx.identity();
        MR::makeMtxUpFront(&mtx, v6, mFrontVec);
        TVec3f velocity(::cBlowVelocity);
        mtx.mult33(mVelocity);
        MR::startBck(this, "Blow", nullptr);
    }

    mVelocity += (mGravity * 1.0f);
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
    : LiveActor(pName), _8C(), _90(gZeroVec), _9C(0.0f, 0.0f, 1.0f), _A8(0.0f, 1.0f, 0.0f), mArg0(1800.0f), _B8(), _BC(), _C0() {
    _C4[0] = nullptr;
}
CocoSambo::~CocoSambo() {
}

void CocoSambo::init(const JMapInfoIter& rIter) {
    initMapToolInfo(rIter);
    MR::calcGravity(this);
    initModelManagerWithAnm("CocoSamboBody", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSensor();
    initEffectKeeper(1, nullptr, false);
    MR::setEffectHostSRT(this, "Hit", &_90, nullptr, nullptr);
    initSound(4, false);
    MR::initShadowFromCSV(this, "Shadow");
    _8C = new CocoSamboHead(this);
    _8C->initWithoutIter();
    MR::initLightCtrl(_8C);
    _B8 = new AnimScaleParam();
    _BC = SamboFunction::createAnimScaleController(_B8);
    for (s32 i = 0; i < ARRAY_SIZE(_C4); i++) {
        _C4[i] = new LiveActor("ポインティング用アクター");
        _C4[i]->initWithoutIter();
        MR::invalidateClipping(_C4[i]);
        TVec3f result;
        MR::initStarPointerTargetAtMtx(_C4[i], 80.0f * 100.0f, MR::getJointMtx(_C4[i], cPointingJointName[i]), result);
        _C4[i]->makeActorAppeared();
    }
    initNerve(&NrvCocoSambo::CocoSamboNrvHideWait::sInstance);
    appear();
}

void CocoSambo::appear() {
    setNerve(&NrvCocoSambo::CocoSamboNrvHideWait::sInstance);
    LiveActor::appear();
}

void CocoSambo::kill() {
    _8C->kill();
    LiveActor::kill();
    for (s32 i = 0; i < ARRAY_SIZE(_C4); i++) {
        _C4[i]->kill();
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
    _90.lerp(pReceiver->mPosition, pSender->mPosition, 0.75f);
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
        _BC->startHitReaction();
    }
    return true;
}

void CocoSambo::control() {
    _BC->updateNerve();
}

void CocoSambo::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mArg0);
    MR::useStageSwitchWriteDead(this, rIter);
    TRot3f mtx;
    MR::makeMtxRotate(mtx, mRotation);
    mtx.getYDir2(_A8);
    mtx.getZDir2(_9C);
}

void CocoSambo::initSensor() {
    f32 f = mScale.x * 80.0f;
    TVec3f vec(cSensorOffset);
    vec.scale(mScale.x);
    initHitSensor(3);
    MR::addHitSensorMtx(this, "spine1", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine1"), vec);
    MR::addHitSensorMtx(this, "spine2", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine2"), vec);
    MR::addHitSensorMtx(this, "spine3", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine3"), vec);
}

/* void CocoSambo::dirToPlayer(f32 f) {
    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), TVec3f(f));
    if (!MR::isNearZero(vec) && !MR::isSameDirection(_A8, vec, 0.01f)) {
        TVec3f vec2;
        TVec3f vec3;
        vec2.set(_9C);
        MR::normalize(&vec);
        MR::vecKillElement(vec, _A8, &vec3);
        MR::normalize(&vec3);
        if (f < 0.0f) {

        }
    }
} */

bool CocoSambo::tryDpdPointing(const Nerve* pNerve) {
    if (isPointing()) {
        _C0 = pNerve;
        setNerve(&NrvCocoSambo::CocoSamboNrvDpdPointing::sInstance);
        return true;
    }
    return false;
}

bool CocoSambo::isPointing() {
    for (s32 i = 0; i < ARRAY_SIZE(_C4); i++) {
        if (MR::isStarPointerPointing2POnPressButton(_C4[i], "弱", true, false)) {
            return true;
        }
    }
    return false;
}

void CocoSambo::exeHideWait() {
    if (MR::isFirstStep(this)) {
        startBckThisAndHead("HideWait");
        MR::invalidateShadow(this, nullptr);
        MR::invalidateHitSensors(_8C);
    }
    if (MR::isNearPlayer(this, mArg0)) {
        setNerve(&NrvCocoSambo::CocoSamboNrvAppear::sInstance);
    }
}

bool CocoSambo::isNerveDying() {
    return isNerve(&NrvCocoSambo::CocoSamboNrvPressY::sInstance) || isNerve(&NrvCocoSambo::CocoSamboNrvBlow::sInstance);
}

void CocoSambo::startBckThisAndHead(const char* pBckName) {
    MR::startBck(this, pBckName, nullptr);
    MR::startBck(_8C, pBckName, nullptr);
}

void CocoSambo::exeAppear() {
    if (MR::isFirstStep(this)) {
        startBckThisAndHead("Appear");
        MR::validateHitSensors(_8C);
        dirToPlayer(180.0f);
        MR::showMaterial(_8C, "SanboNeedleMat_v");
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
    if (!tryDpdPointing(&NrvCocoSambo::CocoSamboNrvWait::sInstance)) {
        if (MR::isNearPlayer(this, 700.0f)) {
            setNerve(&NrvCocoSambo::CocoSamboNrvAttack::sInstance);
            return;
        }
        if (!MR::isNearPlayer(this, mArg0 + 100.0f)) {
            setNerve(&NrvCocoSambo::CocoSamboNrvHide::sInstance);
        }
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
            MR::emitEffect(_8C, "CocoSamboAttack");
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
        _8C->setNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadFall::sInstance);
        MR::emitEffect(this, "Hit");
        MR::startSound(this, "SE_EM_SFSAMBO_DAMAGE");
        MR::startSound(this, "SE_EM_SFSAMBO_BLOW_HEAD");
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::hideMaterial(_8C, "SanboNeedleMat_v");
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
    if (_8C->isNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadConnectedBody::sInstance)) {
        setNerve(&NrvCocoSambo::CocoSamboNrvRecoverWait::sInstance);
    }
}

void CocoSambo::exeRecoverWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recover", nullptr);
        MR::setBckRate(this, 0.0f);
        MR::invalidateHitSensors(_8C);
    }
    MR::setNerveAtStep(this, &NrvCocoSambo::CocoSamboNrvRecover::sInstance, 90);
}

void CocoSambo::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 1.0f);
        MR::startBck(_8C, "Recover", nullptr);
        _8C->mCalcOwnMtx = true;
        MR::showMaterial(_8C, "SanboNeedleMat_v");
    }
    dirToPlayer(180.0f);
    MR::setNerveAtBckStopped(this, &NrvCocoSambo::CocoSamboNrvAppear::sInstance);
}

void CocoSambo::exePressY() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 0.0f);
        MR::startBck(_8C, "PressY", nullptr);
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
        _8C->setNerve(&NrvCocoSamboHead::CocoSamboHeadNrvHeadBlow::sInstance);
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
        MR::setBckRate(_8C, 0.0f);
        MR::emitEffect(this, "Touch1");
        MR::emitEffect(this, "Touch2");
        MR::emitEffect(this, "Touch3");
        MR::emitEffect(_8C, "Touch");
        _BC->startDpdHitVibration();
        MR::startDPDHitSound();
    }
    MR::startDPDFreezeLevelSound(this);
    if (!isPointing()) {
        setNerve(_C0);
    }
}

void CocoSambo::endDpdPointing() {
    MR::setBckRate(this, 1.0f);
    MR::setBckRate(_8C, 1.0f);
    MR::deleteEffect(this, "Touch1");
    MR::deleteEffect(this, "Touch2");
    MR::deleteEffect(this, "Touch3");
    MR::deleteEffect(_8C, "Touch");
    _BC->stopAndReset();
}

CocoSamboHead::~CocoSamboHead() {
}
