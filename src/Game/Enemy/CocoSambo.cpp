#include "Game/Enemy/CocoSambo.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/SamboFunction.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void CocoSambo_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
}

namespace NrvCocoSamboHead {
    NEW_NERVE(CocoSamboHeadNrvHeadConnectedBody, CocoSamboHead, ConnectedBody);
    NEW_NERVE(CocoSamboHeadNrvHeadFall, CocoSamboHead, Fall);
    NEW_NERVE(CocoSamboHeadNrvHeadFallLand, CocoSamboHead, FallLand);
    NEW_NERVE(CocoSamboHeadNrvHeadSwoon, CocoSamboHead, Swoon);
    NEW_NERVE(CocoSamboHeadNrvHeadSwoonEnd, CocoSamboHead, SwoonEnd);
    NEW_NERVE(CocoSamboHeadNrvHeadBlow, CocoSamboHead, Blow);
}  // namespace NrvCocoSamboHead

namespace {
    const Vec cHeadSensorOffset = {60.0f, 0.0f, 0.0f};
    const Vec cBlowVelocity = {0.0f, 20.0f, -100.0f};
    const Vec cSensorOffset = {40.0f, 0.0f, 0.0f};
    const char* const cPointingJointName[] = {"Spine1", "Spine2", "Spine3", "Head"};
    const f32 cHeadSensorRadius = 100.0f;
    const f32 cTrampleSensorRadius = 150.0f;
    const s32 cFallFrame = 45;
    // const f32 cFallGravity =
    const s32 cSwoonFrame = 180;
    const s32 cSwoonEndDamagedFrame = 95;
    // const s32 cRecoverWaitFrame =
    const s32 cPressFrame = 35;
    // const s32 cAttackEffectEmitStep =
    // const s32 cHitStopSceneFrame =
    // const s32 cBlowStopSceneStep =
    // const s32 cBlowStopSceneFrame =
    // const f32 cBlowGravity =
    const s32 cBlowFrame = 20;
    // const f32 cSensorRadius =
    // const f32 cAttackDistance =
    // const s32 cAttackInterval =
    // const f32 cRotateSpeed =
    // const s32 cAttackRotateFrame =
    const s32 cAppearUpFrame = 60;
    const s32 cAppearLandFrame = 105;
    // const s32 cHideDownFrame =
    // const s32 cSwoonEndHideShadowFrame =
    const s32 cPointingActorNum = 4;
    // const f32 cPointingRadius =
    // const f32 cPointingOffset =
    // const f32 cAppearDistance =
}  // namespace

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
}  // namespace NrvCocoSambo

inline bool CocoSambo::isSensorTryToFallHead(HitSensor* pSensor) {
    return pSensor->isType(ATYPE_COCO_NUT) || pSensor->isType(ATYPE_KURIBO) || pSensor->isType(ATYPE_BEGOMAN);
}

inline bool CocoSambo::isNerveDown() {
    return isNerve(GET_NERVE(CocoSambo, CocoSamboNrvFallHead)) || isNerve(GET_NERVE(CocoSambo, CocoSamboNrvFallHeadHide)) ||
           isNerve(GET_NERVE(CocoSambo, CocoSamboNrvRecoverWait));
}

bool CocoSambo::isNerveDying() {
    return isNerve(GET_NERVE(CocoSambo, CocoSamboNrvPressY)) || isNerve(GET_NERVE(CocoSambo, CocoSamboNrvBlow));
}

CocoSamboHead::CocoSamboHead(LiveActor* pHost) : PartsModel(pHost, "ココサンボ[頭]", "CocoSamboHead", 0, 18, 0) {
    mFrontVec.set(0.0f, 0.0f, 1.0f);
}

void CocoSamboHead::init(const JMapInfoIter& rIter) {
    TVec3f sensorOffs(::cHeadSensorOffset);
    sensorOffs.scale(mScale.x);

    initHitSensor(2);
    MR::addHitSensorAtJoint(this, "body", "Head", ATYPE_SAMBO_HEAD, 8, (::cHeadSensorRadius * mScale.x), sensorOffs);
    MR::addHitSensorAtJoint(this, "trample", "Head", ATYPE_SAMBO_HEAD, 8, (::cTrampleSensorRadius * mScale.x), sensorOffs);
    initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, -90.0f, -90.0f), "Head");
    initNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadConnectedBody));
    PartsModel::init(rIter);
}

void CocoSamboHead::kill() {
    MR::startSound(this, "SE_EM_EXPLODE_S");
    LiveActor::kill();
    MR::emitEffect(this, "DeathSmokeHead");
}

void CocoSamboHead::calcAndSetBaseMtx() {
    if (mIsCalcOwnMtx != false) {
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

bool CocoSamboHead::isBodySensor(const HitSensor* pSensor) const {
    return pSensor == getSensor("body");
}

inline bool CocoSamboHead::isNerveSwoon() const {
    return isNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadFallLand)) || isNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadSwoon)) ||
           (isNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadSwoonEnd)) && MR::isLessStep(this, 60));
}

void CocoSamboHead::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isBodySensor(pSender) && !static_cast< CocoSambo* >(mHost)->isNerveDying()) {
        if (MR::isSensorPlayer(pReceiver)) {
            if (!static_cast< CocoSambo* >(mHost)->isNerveDown() && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
                MR::sendMsgPush(pReceiver, pSender);
                return;
            }

            if (mHost->isNerve(GET_NERVE(CocoSambo, CocoSamboNrvAppear))) {
                MR::sendMsgJump(pReceiver, pSender);
            } else {
                MR::sendMsgPush(pReceiver, pSender);
            }

            return;
        }

        if (pReceiver->isType(ATYPE_KURIBO)) {
            if (mHost->isNerve(GET_NERVE(CocoSambo, CocoSamboNrvAttack)) && MR::sendMsgToEnemyAttackTrample(pReceiver, pSender)) {
                return;
            }

            MR::sendMsgPush(pReceiver, pSender);
            return;
        }

        if (pReceiver->isType(ATYPE_COCO_NUT)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool CocoSamboHead::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerveSwoon()) {
        if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
            return static_cast< CocoSambo* >(mHost)->tryToPress();
        }

        if (MR::isMsgPlayerHitAll(msg)) {
            return static_cast< CocoSambo* >(mHost)->tryToBlow();
        }
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return static_cast< CocoSambo* >(mHost)->tryStarPieceReflect();
    }

    return false;
}

bool CocoSamboHead::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (static_cast< CocoSambo* >(mHost)->isSensorTryToFallHead(pSender) && static_cast< CocoSambo* >(mHost)->tryToFallHead(pReceiver, pSender)) {
        return true;
    }

    return false;
}

void CocoSamboHead::updateFrontVecToPlayer(const TVec3f& rVec) {
    TVec3f v5;
    TVec3f v4 = -rVec;
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
    TVec3f acceleration;
    acceleration.scale(2.0f, gravity);

    if (MR::isFirstStep(this)) {
        mFixedPosition->copyTrans(&mPosition);
        mRotation.zero();
        mIsCalcOwnMtx = false;
        MR::startBck(this, "Fall");
        updateFrontVecToPlayer(gravity);

        TVec3f offset;
        f32 fallFrame = ::cFallFrame;
        offset.sub(mHost->mPosition, mPosition);
        TVec3f verticalOffset;
        TVec3f horizontalVelocity;
        TVec3f verticalVelocity;
        TVec3f up = -gravity;
        verticalOffset.scale(up.dot(offset), up);
        MR::vecKillElement(offset, up, &offset);
        horizontalVelocity.scale(1.0f / fallFrame, offset);
        verticalVelocity = (verticalOffset * 2.0f - acceleration * fallFrame * fallFrame) / (2.0f * fallFrame);
        mVelocity.add(horizontalVelocity, verticalVelocity);
    }

    if (MR::isStep(this, ::cFallFrame)) {
        mPosition.set(mHost->mPosition);
        mVelocity.zero();
        setNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadFallLand));
    } else {
        mVelocity += acceleration;
    }
}

void CocoSamboHead::exeFallLand() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Land");
        MR::startSound(this, "SE_EM_SFSAMBO_HEAD_LAND");
        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraWeak();
    }

    MR::setNerveAtBckStopped(this, GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadSwoon));
}

void CocoSamboHead::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swoon");
    }

    if (MR::isStep(this, ::cSwoonFrame)) {
        setNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadSwoonEnd));
    }
}

void CocoSamboHead::exeSwoonEnd() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::startBck(this, "SwoonEnd");
    }

    if (MR::isStep(this, ::cSwoonEndDamagedFrame)) {
        MR::invalidateShadow(mHost, nullptr);
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormal();
    }

    MR::setNerveAtBckStopped(this, GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadConnectedBody));
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
        MR::startBck(this, "Blow");
    }

    TVec3f result;
    result.scale(1.0f, mGravity);
    mVelocity.add(mVelocity, result);
}

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
        MR::initStarPointerTargetAtMtx(mPointingActorArray[i], 80.0f * mScale.x, MR::getJointMtx(this, ::cPointingJointName[i]),
                                       TVec3f(mScale.x * 100.0f, 0.0f, 0.0f));
        mPointingActorArray[i]->makeActorAppeared();
    }

    initNerve(GET_NERVE(CocoSambo, CocoSamboNrvHideWait));
    appear();
}

void CocoSambo::appear() {
    setNerve(GET_NERVE(CocoSambo, CocoSamboNrvHideWait));
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

bool CocoSambo::tryToFallHead(const HitSensor* pReceiver, const HitSensor* pSender) {
    if (!(isNerve(GET_NERVE(CocoSambo, CocoSamboNrvAppear)) || isNerve(GET_NERVE(CocoSambo, CocoSamboNrvWait)) ||
          isNerve(GET_NERVE(CocoSambo, CocoSamboNrvAttack)) || isNerve(GET_NERVE(CocoSambo, CocoSamboNrvAttackInterval)) ||
          isNerve(GET_NERVE(CocoSambo, CocoSamboNrvDpdPointing)))) {
        return false;
    }

    MR::deleteEffectAll(this);
    mHitEffectScale.lerp(pReceiver->mPosition, pSender->mPosition, 0.75f);
    setNerve(GET_NERVE(CocoSambo, CocoSamboNrvFallHead));
    return true;
}

bool CocoSambo::tryToPress() {
    if (isNerveDying()) {
        return false;
    }

    MR::deleteEffectAll(this);
    setNerve(GET_NERVE(CocoSambo, CocoSamboNrvPressY));
    return true;
}

bool CocoSambo::tryToBlow() {
    if (isNerveDying()) {
        return false;
    }

    MR::deleteEffectAll(this);
    setNerve(GET_NERVE(CocoSambo, CocoSamboNrvBlow));
    return true;
}

bool CocoSambo::tryStarPieceReflect() {
    if (isNerveDying()) {
        return false;
    }

    if (!isNerve(GET_NERVE(CocoSambo, CocoSamboNrvDpdPointing))) {
        mAnimScaleController->startHitReaction();
    }

    return true;
}

void CocoSambo::control() {
    mAnimScaleController->updateNerve();
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
    TVec3f offset(::cSensorOffset);
    offset.scale(mScale.x);
    initHitSensor(3);
    MR::addHitSensorMtx(this, "spine1", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine1"), offset);
    MR::addHitSensorMtx(this, "spine2", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine2"), offset);
    MR::addHitSensorMtx(this, "spine3", ATYPE_SAMBO_BODY, 8, f, MR::getJointMtx(this, "Spine3"), offset);
}

void CocoSambo::dirToPlayer(f32 angle) {
    TVec3f toPlayer;
    toPlayer.sub(*MR::getPlayerPos(), mPosition);

    if (!MR::isNearZero(toPlayer) && !MR::isSameDirection(mUpVec, toPlayer)) {
        TVec3f front;
        TVec3f targetFront;
        front.set(mFrontVec);
        MR::normalize(&toPlayer);
        MR::vecKillElement(toPlayer, mUpVec, &targetFront);
        MR::normalize(&targetFront);
        MR::turnVecToVecCos(&mFrontVec, front, targetFront, MR::cosDegree(angle), mUpVec);
    }
}

bool CocoSambo::tryDpdPointing(const Nerve* pNerve) {
    if (isPointing()) {
        mDpdPointingEndNerve = pNerve;
        setNerve(GET_NERVE(CocoSambo, CocoSamboNrvDpdPointing));
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
        setNerve(GET_NERVE(CocoSambo, CocoSamboNrvAppear));
    }
}

void CocoSambo::startBckThisAndHead(const char* pBckName) {
    MR::startBck(this, pBckName);
    MR::startBck(mHead, pBckName);
}

void CocoSambo::exeAppear() {
    if (MR::isFirstStep(this)) {
        startBckThisAndHead("Appear");
        MR::validateHitSensors(mHead);
        dirToPlayer(180.0f);
        MR::showMaterial(mHead, "SanboNeedleMat_v");
    }

    if (MR::isStep(this, ::cAppearUpFrame)) {
        MR::emitEffect(this, "CocoSamboSmoke");
        MR::startSound(this, "SE_EM_SFSAMBO_APPEAR");
        MR::validateShadow(this, nullptr);
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, ::cAppearLandFrame)) {
        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraWeak();
    }

    dirToPlayer(1.0f);
    if (!MR::isGreaterStep(this, ::cAppearLandFrame) || !tryDpdPointing(GET_NERVE(CocoSambo, CocoSamboNrvWait))) {
        MR::setNerveAtBckStopped(this, GET_NERVE(CocoSambo, CocoSamboNrvWait));
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
    if (tryDpdPointing(GET_NERVE(CocoSambo, CocoSamboNrvWait))) {
        return;
    }

    if (MR::isNearPlayer(this, 700.0f)) {
        setNerve(GET_NERVE(CocoSambo, CocoSamboNrvAttack));
    } else if (!MR::isNearPlayer(this, mPlayerSearchDistance + 100.0f)) {
        setNerve(GET_NERVE(CocoSambo, CocoSamboNrvHide));
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

    MR::setNerveAtBckStopped(this, GET_NERVE(CocoSambo, CocoSamboNrvHideWait));
}

void CocoSambo::exeAttack() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Attack")) {
        startBckThisAndHead("Attack");
    }

    if (MR::getBckFrame(this) < 30.0f) {
        dirToPlayer(1.0f);
    }

    if (!tryDpdPointing(GET_NERVE(CocoSambo, CocoSamboNrvAttack))) {
        if (MR::checkPassBckFrame(this, 115.0f)) {
            MR::emitEffect(mHead, "CocoSamboAttack");
            MR::tryRumblePadStrong(this, WPAD_CHAN0);
            MR::shakeCameraNormal();
        }

        MR::setNerveAtBckStopped(this, GET_NERVE(CocoSambo, CocoSamboNrvAttackInterval));
    }
}

void CocoSambo::exeAttackInterval() {
    if (MR::isFirstStep(this)) {
        startBckThisAndHead("Wait");
    }

    dirToPlayer(1.0f);
    if (!tryDpdPointing(GET_NERVE(CocoSambo, CocoSamboNrvWait))) {
        MR::setNerveAtStep(this, GET_NERVE(CocoSambo, CocoSamboNrvWait), 60);
    }
}

void CocoSambo::exeFallHead() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit");
        mHead->setNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadFall));
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
        setNerve(GET_NERVE(CocoSambo, CocoSamboNrvFallHeadHide));
    }
}

void CocoSambo::exeFallHeadHide() {
    if (MR::isFirstStep(this)) {
        MR::startBckNoInterpole(this, "HideWait");
    }

    if (mHead->isNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadConnectedBody))) {
        setNerve(GET_NERVE(CocoSambo, CocoSamboNrvRecoverWait));
    }
}

void CocoSambo::exeRecoverWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Recover");
        MR::setBckRate(this, 0.0f);
        MR::invalidateHitSensors(mHead);
    }

    MR::setNerveAtStep(this, GET_NERVE(CocoSambo, CocoSamboNrvRecover), 90);
}

void CocoSambo::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 1.0f);
        MR::startBck(mHead, "Recover");
        mHead->mIsCalcOwnMtx = true;
        MR::showMaterial(mHead, "SanboNeedleMat_v");
    }

    dirToPlayer(180.0f);
    MR::setNerveAtBckStopped(this, GET_NERVE(CocoSambo, CocoSamboNrvAppear));
}

void CocoSambo::exePressY() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 0.0f);
        MR::startBck(mHead, "PressY");
        MR::startSound(this, "SE_EM_STOMPED_S");
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }

    if (MR::isStep(this, ::cPressFrame)) {
        kill();
    }
}

void CocoSambo::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::invalidateShadow(this, nullptr);
        MR::tryRumblePadVeryStrong(this, WPAD_CHAN0);
        MR::startBlowHitSound(this);
        mHead->setNerve(GET_NERVE(CocoSamboHead, CocoSamboHeadNrvHeadBlow));
    }

    if (MR::isStep(this, 1)) {
        MR::stopScene(6);
    }

    if (MR::isStep(this, 2)) {
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, ::cBlowFrame)) {
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
