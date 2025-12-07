#include "Game/Enemy/Petari.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    static const TVec3f sBodySensorOffset;
    static const TVec3f sSpinSensorOffset;
    static const char* sBodySensorName = "body";
    static const char* sSpinSensorName = "spin";

    NEW_NERVE(PetariNrvLurk, Petari, Lurk);
    NEW_NERVE_ONEND(PetariNrvWait, Petari, Wait, Wait);
    NEW_NERVE_ONEND(PetariNrvApproach, Petari, Approach, Approach);
    NEW_NERVE_ONEND(PetariNrvEscape, Petari, Escape, Escape);
    NEW_NERVE(PetariNrvSwoonStart, Petari, SwoonStart);
    NEW_NERVE(PetariNrvSwoon, Petari, Swoon);
    NEW_NERVE(PetariNrvSwoonEnd, Petari, SwoonEnd);
    NEW_NERVE(PetariNrvLand, Petari, Land);
    NEW_NERVE(PetariNrvSmash, Petari, Smash);
    NEW_NERVE(PetariNrvSpinOut, Petari, SpinOut);
    NEW_NERVE_ONEND(PetariNrvFreeze, Petari, Freeze, Freeze);
    NEW_NERVE(PetariNrvKickOut, Petari, KickOut);
    NEW_NERVE(PetariNrvJumpOut, Petari, JumpOut);
} // namespace

Petari::Petari(const char* pName) :
    LiveActor(pName),
    mAnimScaleCtrl(nullptr),
    mFootPrint(nullptr),
    mFootprintYOffs(0.0f),
    mRequestSmoke(true),
    mFront(0.0f, 0.0f, 1.0f),
    mTargetDir(0.0f, 0.0f, 1.0f),
    mMeanderStep(0),
    mMeanderAngle(0.0f),
    mStarPieceTotal(20),
    mStarPieceRemaining(20),
    _D0(0)
{
    mHeadMtx.identity();
}

// Non-matching because of stack size
void Petari::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Petari", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addHitSensorEnemy(this, sBodySensorName, 32, 60.0f, TVec3f(sBodySensorOffset));
    MR::addHitSensorEnemy(this, sSpinSensorName, 32, 100.0f, TVec3f(sSpinSensorOffset));
    initBinder(60.0f, 60.0f, 0);
    initEffectKeeper(1, nullptr, false);
    MR::addEffect(this, "PointerTouchManual");
    MR::setEffectHostMtx(this, "PointerTouchManual", mHeadMtx);
    initSound(4, false);
    MR::initShadowFromCSV(this, "Shadow");
    mAnimScaleCtrl = new AnimScaleController(nullptr);
    initFootPrint(rIter);
    initNerve(&PetariNrvLurk::sInstance);
    initFrontVec();
    MR::onCalcGravity(this);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
    }
    MR::useStageSwitchWriteDead(this, rIter);
    initStarPiece(rIter);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 70.0f, 0.0f));
    makeActorAppeared();
}

void Petari::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void Petari::makeActorAppeared() {
    MR::hideModel(this);
    setNerve(&PetariNrvLurk::sInstance);
    LiveActor::makeActorAppeared();
    mMeanderStep = 0;
}

void Petari::kill() {
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    MR::deleteEffect(this, "InnerLight");
    LiveActor::kill();
    mFootPrint->clear();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void Petari::calcAnim() {
    LiveActor::calcAnim();
    if (!MR::isDead(this) && !MR::isHiddenModel(this)) {
        mHeadMtx.set(MR::getJointMtx(this, "Head1"));
    }
}

// Non-matching because of .data addrs
void Petari::control() {
    TVec3f upVec(getBaseMtx()[0][1], getBaseMtx()[1][1], getBaseMtx()[2][1]);
    
    if (MR::isOnGround(this)) {
        TQuat4f groundQuat;
        groundQuat.setRotate(upVec, *MR::getGroundNormal(this), 0.1f);
        groundQuat.transform(upVec);
    } else {
        TVec3f invGravVec;
        JGeometry::negateInternal((f32*)&mGravity, (f32*)&invGravVec);
        TQuat4f gravityQuat;
        gravityQuat.setRotate(upVec, invGravVec, 0.1f);
        gravityQuat.transform(upVec);
    }
    TPos3f v11;
    MR::makeMtxUpFrontPos(&v11, upVec, mFront, mPosition);
    MR::setBaseTRMtx(this, v11);
    mAnimScaleCtrl->updateNerve();

    mHeadMtx.set(getBaseMtx());
}

void Petari::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pSender, sBodySensorName) && isSolidBody()) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

// Non-matching because of one instruction swap
bool Petari::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pReceiver, sBodySensorName)) {
        receivePlayerAttackAtBody(msg, pSender, pReceiver);
    }
    else if (MR::isSensor(pReceiver, sSpinSensorName)) {
        receivePlayerAttackAtSpin(msg, pSender, pReceiver);
    }
    
    return false;
}

// Non-matching because of one li r3, 0x0
bool Petari::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pReceiver, sBodySensorName) && isSolidBody() && MR::isMsgToEnemyAttackBlow(msg)) {
        MR::setVelocityBlowAttack(this, pSender, pReceiver, 10.0f, 0.0f, 4);
        setNerve(&PetariNrvSpinOut::sInstance);
        return true;
    }
    
    return false;
}

bool Petari::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pReceiver, sBodySensorName) && isSolidBody()) {
        MR::addVelocityFromPush(this, 3.0f, pSender, pReceiver);
        return true;
    }
    
    return false;
}

bool Petari::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pReceiver, sBodySensorName) && isNerve(&PetariNrvSwoon::sInstance) && MR::isMsgPlayerKick(msg)) {
        MR::setVelocityBlowAttack(this, pSender, pReceiver, 10.0f, 0.0f, 4);
        setNerve(&PetariNrvKickOut::sInstance);
        return true;
    }
    
    return false;
}

// Non-matching because of some branching stuff
bool Petari::receivePlayerAttackAtBody(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        if (isNerve(&PetariNrvSwoon::sInstance)) {
            mAnimScaleCtrl->startHitReaction();
            setNerve(&PetariNrvSwoon::sInstance);
            return true;
        }
        if (isNerve(&PetariNrvSwoonStart::sInstance) || isNerve(&PetariNrvSwoonEnd::sInstance) || isNerve(&PetariNrvWait::sInstance)) {
            mAnimScaleCtrl->startHitReaction();
            return true;
        }
    }
    if (isSolidBody() && (MR::isMsgJetTurtleAttack(msg) || MR::isMsgFireBallAttack(msg) || MR::isMsgInvincibleAttack(msg))) {
        MR::setVelocityBlowAttack(this, pSender, pReceiver, 10.0f, 0.0f, 4);
        setNerve(&PetariNrvKickOut::sInstance);
        return true;
    }
    else {
        if (isNerve(&PetariNrvFreeze::sInstance) && !MR::isHiddenModel(this) && (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg))) {
            setNerve(&PetariNrvSmash::sInstance);
            return false;
        }
        if (isNerve(&PetariNrvSwoon::sInstance)) {
            if (MR::isMsgPlayerSpinAttack(msg) || MR::isMsgFreezeAttack(msg)) {
                MR::setVelocityBlowAttack(this, pSender, pReceiver, 10.0f, 0.0f, 4);
                setNerve(&PetariNrvSpinOut::sInstance);
                return true;
            }
            if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
                setNerve(&PetariNrvSmash::sInstance);
                return true;
            }
        }
    }
    return false;
}

bool Petari::receivePlayerAttackAtSpin(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgPlayerSpinAttack(msg) && !MR::isMsgFreezeAttack(msg)) {
        return false;
    }
    
    bool v5 = false;
    if (isNerve(&PetariNrvWait::sInstance) || isNerve(&PetariNrvEscape::sInstance) || isNerve(&PetariNrvFreeze::sInstance)) {
        v5 = true;
    }
    if (v5) {
        setNerve(&PetariNrvSwoonStart::sInstance);
        return true;
    }
    return false;
}

// Non-matching because the li is too early
bool Petari::isSolidBody() const {
    bool solid = false;

    if (MR::isHiddenModel(this)) {
        solid = false;
    } else {
        if ( this->isNerve(&PetariNrvSwoonStart::sInstance)
          || this->isNerve(&PetariNrvSwoon::sInstance)
          || this->isNerve(&PetariNrvSwoonEnd::sInstance)
          || this->isNerve(&PetariNrvWait::sInstance)
          || this->isNerve(&PetariNrvFreeze::sInstance) ) {
            solid = true;
        }
    }

    return solid;
}

// Non-matching because of a cror and a mr
void Petari::calcSpinOutVelocity(f32 speed) {
    TVec3f* playerPos = MR::getPlayerCenterPos();

    TVec3f deltaDir(mBodyCenter);
    deltaDir -= *playerPos;
    
    JMAVECScaleAdd(&mGravity, &deltaDir, &deltaDir, -mGravity.dot(deltaDir));
    
    if (!MR::isNearZero(deltaDir, 0.001f)) {
        TVec3f negDir;
        JGeometry::negateInternal((f32*)&deltaDir, (f32*)&negDir);
        mFront = negDir;
        MR::normalize(&mFront);
    }
    f32 v7 = deltaDir.squared();
    if (v7 > 0.0000038146973f) {
        f32 v8 = JGeometry::TUtil<f32>::inv_sqrt(v7);
        deltaDir *= (v8 * speed);
    }
    mVelocity = deltaDir;
}

void Petari::calcApproachDirection() {
    TVec3f v4(*MR::getPlayerCenterPos());
    v4 -= mBodyCenter;
    mTargetDir = v4;
    if (MR::isNearZero(mTargetDir, 0.001f)) {
        mTargetDir = mFront;
    }
    MR::normalize(&mTargetDir);
}

void Petari::calcEscapeDirection() {
    TVec3f* playerCenterPos = MR::getPlayerCenterPos();
    TVec3f v4(mBodyCenter);
    v4 -= *playerCenterPos;
    mTargetDir = v4;
    if (MR::isNearZero(mTargetDir, 0.001f)) {
        mTargetDir = mFront;
    }
    MR::normalize(&mTargetDir);
}

// Non-matching because of stack size
void Petari::avoidPlayer() {
    TVec3f crossVec;
    PSVECCrossProduct(&mTargetDir, &mFront, &crossVec);
    f32 turnSign;
    if (crossVec.dot(mGravity) < 0.0f) {
        turnSign = -1.0f;
    } else {
        turnSign = 1.0f;
    }
    TVec3f dirToPlayer(*MR::getPlayerCenterPos());
    dirToPlayer -= mBodyCenter;
    if (!MR::isNearZero(dirToPlayer, 0.001f)) {
        TVec3f v11(dirToPlayer);
        v11 *= 0.01f;
        f32 avoidWeight = 3.0f / v11.squared();
        MR::clamp01(&avoidWeight);
        MR::normalize(&dirToPlayer);
        JGeometry::negateInternal((f32*)&dirToPlayer, (f32*)&dirToPlayer);
        TVec3f playerVelocity(*MR::getPlayerVelocity());
        MR::normalizeOrZero(&playerVelocity);
        f32 approaching = dirToPlayer.dot(playerVelocity);
        if (approaching < 0.0f) {
            approaching = 0.0f;
        }
        avoidWeight *= approaching;
        TQuat4f quat;
        quat.setRotate(mGravity, ((avoidWeight * ((90.0f * turnSign) * 3.1415927f)) / 180.0f));
        quat.transform(mTargetDir);
    }
}

// Non-matching because of stack size and regswap
void Petari::avoidWall() {
    TVec3f planarDir(mTargetDir);
    JMAVECScaleAdd(&mGravity, &planarDir, &planarDir, -mGravity.dot(planarDir));
    if (!MR::isNearZero(planarDir, 0.001f)) {
        MR::normalize(&planarDir);
        Triangle rayHitTri;
        TVec3f v17(planarDir);
        v17 *= 600.0f;
        TVec3f v19;
        if (MR::getFirstPolyOnLineToMapAndMoveLimit(&v19, &rayHitTri, mBodyCenter, v17)) {
            TVec3f wallNormal(*rayHitTri.getNormal(0));
            JMAVECScaleAdd(&mGravity, &wallNormal, &wallNormal, -mGravity.dot(wallNormal));
            if (!MR::isNearZero(wallNormal, 0.001f)) {
                MR::normalize(&wallNormal);
                f32 proximity = 1.0f - (PSVECDistance(&v19, &mBodyCenter) / 600.0f);
                MR::clamp01(&proximity);
                f32 v8 = (proximity * proximity);
                proximity = proximity * proximity;
                
                f32 v9 = wallNormal.dot(mTargetDir);
                TVec3f v13(wallNormal);
                v13 *= v9;
                TVec3f v14(v13);
                v14 *= 2.0;
                TVec3f v15(v14);
                v15 *= v8;
                TVec3f v16(v15);
                v16 *= 2.0;
                mTargetDir -= v16;
            }
        }
    }
}

// Non-matching because of stack size
void Petari::calcCenter() {
    TVec3f v3(mGravity);
    v3 *= -60.0f;
    TVec3f v4(mPosition);
    v4 += v3;
    mBodyCenter = v3;
}

// Non-matching because of regswap
f32 Petari::calcBoost(f32 baseSpeed, f32 boostMultiplier, f32 effectiveRange) const {
    f32 distToPlayer = MR::calcDistanceToPlayer(mBodyCenter);
    
    f32 scaledDist = distToPlayer * 0.01f;
    
    f32 distSquared = scaledDist * scaledDist;
    f32 intensity = effectiveRange / distSquared;
    
    if (intensity > 1.0f) {
        intensity = 1.0f;
    }
    
    f32 speedIncrease = baseSpeed * boostMultiplier;
    return baseSpeed + (speedIncrease * intensity);
}

void Petari::appearStarPieceGradually() {
    if (getNerveStep() % 5) {
        return;
    }
    
    s32 v1 = 2;
    if (mStarPieceRemaining < 2) {
        v1 = mStarPieceRemaining;
    }
    
    TVec3f mouthPos;
    calcMouthPos(&mouthPos);
    MR::appearStarPiece(this, mouthPos, v1, 20.0f, 40.0f, false);
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    mStarPieceRemaining -= v1;
}

void Petari::appearStarPieceAll() {
    if (mStarPieceRemaining > 0) {
        TVec3f mouthPos;
        calcMouthPos(&mouthPos);
        MR::appearStarPiece(this, mPosition, mStarPieceRemaining, 20.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
}

// Non-matching because of instruction swap
void Petari::calcMouthPos(TVec3f* pOut) const {
    TMtx34f mtx;
    JMath::gekko_ps_copy12(&mtx, MR::getJointMtx(this, "Head3"));
    
    pOut->set(mtx.mMtx[0][3], mtx.mMtx[1][3], mtx.mMtx[2][3]);
}

void Petari::tryEmitFirstFootPrintEffect() {
    if (mRequestSmoke) {
        MR::emitEffect(this, "StartSmoke");
        mRequestSmoke = false;
    }
}

void Petari::initFootPrint(const JMapInfoIter& rIter) {
    mFootPrint = new FootPrint("ペタリの足跡", 32);
    
    ResTIMG* texture = MR::getTexFromArc("Footprint.bti", this);
    mFootPrint->setTexture(texture);
    mFootPrint->_2C = 25.0f;
    mFootPrint->_30 = 30.0f;
    mFootPrint->_34 = 40.0f;
    mFootPrint->_38 = 100.0f;
    
    MR::getJMapInfoArg0NoInit(rIter, &mFootprintYOffs);
}

// Non-matching because of stack
void Petari::updateFootPrint() {
    if (MR::isBindedGround(this)) {
        f32 yOffs = mFootprintYOffs;
        TVec3f v8(*MR::getGroundNormal(this));
        v8 *= yOffs;
        TVec3f v9(mPosition);
        v9 -= v8;
        mFootPrint->addPrint(v9, mFront, *MR::getGroundNormal(this), false);
    }
}

// Non-matching because of one instruction swap
void Petari::initFrontVec() {
    TMtx34f mtx;
    MR::makeMtxTR(mtx, mPosition, mRotation);
    mFront.set(mtx.mMtx[0][2], mtx.mMtx[1][2], mtx.mMtx[2][2]);
}

void Petari::initStarPiece(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg1NoInit(rIter, &mStarPieceTotal);
    MR::declareStarPiece(this, mStarPieceTotal);
    mStarPieceRemaining = mStarPieceTotal;
}

bool Petari::tryShiftJumpOut() {
    TVec3f screenPosition;
    if (MR::calcDistanceToPlayer(mPosition) < 1500.0f && MR::calcScreenPosition(&screenPosition, mPosition)) {
        setNerve(&PetariNrvJumpOut::sInstance);
        return true;
        
    }
    
    return false;
}

bool Petari::tryShiftEscape() {
    if (MR::calcDistanceToPlayer(mPosition) < 500.0f) {
        setNerve(&PetariNrvEscape::sInstance);
        return true;
    }
    
    return false;
}

bool Petari::tryShiftApproach() {
    if (MR::calcDistanceToPlayer(mPosition) > 700.0f) {
        setNerve(&PetariNrvApproach::sInstance);
        return true;
    }
    
    return false;
}

bool Petari::tryEscapeEnd() {
    if (MR::calcDistanceToPlayer(mPosition) > 710.0f) {
        setNerve(&PetariNrvWait::sInstance);
        return true;
    }
    
    return false;
}

bool Petari::tryApproachEnd() {
    if (MR::calcDistanceToPlayer(mPosition) < 600.0f) {
        setNerve(&PetariNrvWait::sInstance);
        return true;
    }
    
    return false;
}

// Non-matching because of .data addrs
void Petari::meander() {
    if (MR::isStep(this, mMeanderStep)) {
        mMeanderAngle = MR::getRandom(-90.0f, 90.0f);
        mMeanderStep = getNerveStep() + MR::getRandom((s32)60, 120);
    }
    
    TVec3f v4;
    JGeometry::negateInternal((f32*)&mGravity, (f32*)&v4);
    
    TQuat4f v5;
    v5.setRotate(v4, (mMeanderAngle * 3.1415927f) / 180.0f);
    v5.transform(mTargetDir);
}

bool Petari::reflectStarPointer2P() {
    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        return false;
    }
    
    setNerve(&PetariNrvFreeze::sInstance);
    return true;
}

// Non-matching because of .data addrs
void Petari::exeWait() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
    }
    
    f32 v3 = 0.0f;
    if (!MR::isBindedGround(this)) {
        v3 = 0.0f;
    }
    MR::moveAndTurnToDirection(this, &mFront, mTargetDir, v3, 1.3, 0.92f, 3.0f);
    
    if (MR::isStep(this, 60)) {
        if (MR::isHiddenModel(this)) {
            MR::showModel(this);
            MR::emitEffect(this, "LeaveAppearanceSmoke");
            MR::startBckNoInterpole(this, "Wait");
        } else {
            MR::startBckWithInterpole(this, "Wait", 30);
        }
        MR::startSound(this, "SE_EM_PETARI_WAIT_APPEAR", -1, -1);
    }
    if (!reflectStarPointer2P() && (tryShiftApproach() || tryShiftEscape()) && !MR::isHiddenModel(this)) {
        MR::emitEffect(this, "LeaveAppearanceSmoke");
        MR::forceDeleteEffect(this, "LeaveAppearanceInnerLight");
        MR::startSound(this, "SE_EM_PETARI_WAIT_HIDE", -1, -1);
    }
}

void Petari::exeKickOut() {
    exeSpinOut();
}

void Petari::endWait() {
    
}

void Petari::endFreeze() {
    mAnimScaleCtrl->startAnim();
    MR::deleteEffect(this, "PointerTouchManual");
}

void Petari::endApproach() {
    MR::deleteEffect(this, "WalkSmoke");
}

void Petari::exeLurk() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
    }
    tryShiftJumpOut();
}

void Petari::endEscape() {
    MR::deleteEffect(this, "RunSmoke");
}

// Non-matching because of .data addrs and a single cmpwi
void Petari::exeApproach() {
    if (MR::isFirstStep(this)) {
        if (!MR::isHiddenModel(this)) {
            MR::hideModel(this);
            MR::emitEffect(this, "StartSmoke");
        }
        tryEmitFirstFootPrintEffect();
    }
    calcCenter();
    calcApproachDirection();
    
    f32 v3 = 0.0f;
    if (!MR::isBindedGround(this)) {
        v3 = 0.0f;
    }
    MR::moveAndTurnToDirection(this, &mFront, mTargetDir, v3, 1.3, 0.92f, 3.0f);
    
    updateFootPrint();
    
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "WalkSmoke");
    }
    
    MR::startLevelSound(this, "SE_EM_LV_PETARI_RUN", -1, -1, -1);
    if (!reflectStarPointer2P()) {
        tryApproachEnd();
    }
}

// Non-matching because of .data addrs and a single cmpwi
void Petari::exeEscape() {
    if (MR::isFirstStep(this)) {
        if (!MR::isHiddenModel(this)) {
            MR::hideModel(this);
            MR::emitEffect(this, "StartSmoke");
        }
        
        mMeanderStep = 0;
        MR::emitEffect(this, "RunSmoke");
        tryEmitFirstFootPrintEffect();
    }
    calcCenter();
    calcEscapeDirection();
    meander();
    avoidPlayer();
    avoidWall();
    
    f32 v3 = 10.0f;
    if (MR::isFirstStep(this)) {
        v3 = 180.0f;
    }
    
    f32 boost = calcBoost(0.45f, 0.5f, 4.0f);
    moveTowardTargetDirection(boost, 0.97f, v3);
    updateFootPrint();
    MR::startLevelSound(this, "SE_EM_LV_PETARI_RUN", -1, -1, -1);
    
    if (!reflectStarPointer2P()) {
        tryEscapeEnd();
    }
}

void Petari::exeSwoonStart() {
    if (MR::isFirstStep(this)) {
        calcSpinOutVelocity(5.0f);
        MR::showModel(this);
        MR::startBck(this, "Appearance", nullptr);
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_EM_PETARI_APPEAR", -1, -1);
        mRequestSmoke = true;
    }
    calcCenter();
    
    f32 v3 = 0.0f;
    if (!MR::isBindedGround(this)) {
        v3 = 0.0f;
    }
    MR::moveAndTurnToDirection(this, &mFront, mTargetDir, v3, 1.3, 0.92f, 3.0f);
    
    MR::setNerveAtBckStopped(this, &PetariNrvSwoon::sInstance);
}

void Petari::exeSwoon() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::startBck(this, "Swoon", nullptr);
    }
    calcCenter();
    
    f32 v3 = 0.0f;
    if (!MR::isBindedGround(this)) {
        v3 = 0.0f;
    }
    MR::moveAndTurnToDirection(this, &mFront, mTargetDir, v3, 1.3, 0.92f, 3.0f);
    
    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);
    MR::setNerveAtStep(this, &PetariNrvSwoonEnd::sInstance, 180);
}

// Non-matching because of .data addrs
void Petari::exeSwoonEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hide", nullptr);
        MR::startSound(this, "SE_EM_PETARI_WAKE_UP", -1, -1);
    }
    if (MR::checkPassBckFrame(this, 60.0f)) {
        MR::startSound(this, "SE_EM_PETARI_JUMP", -1, -1);
    }
    calcCenter();
    
    f32 v3 = 0.0f;
    if (!MR::isBindedGround(this)) {
        v3 = 0.0f;
    }
    MR::moveAndTurnToDirection(this, &mFront, mTargetDir, v3, 1.3, 0.92f, 3.0f);
    
    if (MR::isBckOneTimeAndStopped(this)) {
        MR::startSound(this, "SE_EM_CHANGE_SMOKE", -1, -1);
        MR::hideModel(this);
        setNerve(&PetariNrvLand::sInstance);
    }
}

void Petari::exeLand() {
    if (MR::isFirstStep(this)) {
        mMeanderStep = 0;
    }
    calcCenter();
    calcEscapeDirection();
    meander();
    avoidPlayer();
    avoidWall();

    f32 v3 = 10.0f;
    if (MR::isFirstStep(this)) {
        v3 = 180.0f;
    }
    moveTowardTargetDirection(1.0f, 0.99f, v3);
    
    MR::setNerveAtStep(this, &PetariNrvEscape::sInstance, 45);
}

// Non-matching because of .data addrs and a single cmpwi
void Petari::exeSmash() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "Press", nullptr);
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
    
    calcCenter();
    appearStarPieceGradually();
    
    if (MR::isBckOneTimeAndStopped(this)) {
        appearStarPieceAll();
        kill();
    }
}

// Non-matching because of a stack load/store and some mr instrs
void Petari::exeSpinOut() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "Damage", nullptr);
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
    
    TVec3f v7;
    JGeometry::negateInternal((f32*)&mVelocity, (f32*)&v7);
    MR::turnDirectionDegree(this, &mFront, v7, 180.0f);
    TVec3f v8;
    if (MR::isOnGround(this)) {
        TVec3f groundNormalInv;
        const TVec3f* groundNormal = MR::getGroundNormal(this);
        JGeometry::negateInternal((f32*)groundNormal, (f32*)&groundNormalInv);
        v8 = groundNormalInv;
    } else {
        v8 = mGravity;
    }
    
    JMAVECScaleAdd(&v8, &mVelocity, &mVelocity, -v8.dot(mVelocity));
    TVec3f v5(v8);
    v5 *= 1.3f;
    mVelocity += v5;
    
    calcCenter();
    appearStarPieceGradually();
    if (MR::isBckOneTimeAndStopped(this)) {
        appearStarPieceAll();
        kill();
    }
}

void Petari::exeFreeze() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        MR::emitEffectWithScale(this, "PointerTouchManual", 0.8f, -1);
        mVelocity.zero();
        mAnimScaleCtrl->startDpdHitVibration();
    }
    
    f32 v3 = 0.0f;
    if (!MR::isBindedGround(this)) {
        v3 = 0.0f;
    }
    MR::moveAndTurnToDirection(this, &mFront, mTargetDir, v3, 1.3, 0.92f, 3.0f);
    
    MR::startDPDFreezeLevelSound(this);
    
    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&PetariNrvWait::sInstance);
    }
}

void Petari::exeJumpOut() {
    setNerve(&PetariNrvWait::sInstance);
}

void MR::clamp01(f32* pX) {
    f32 x = *pX;
    f32 out;

    if (x < 0.0f) {
        out = 0.0f;
    } else {
        if (x > 1.0f) {
            out = 1.0f;
        } else {
            out = x;
        }
    }

    *pX = out;
}
