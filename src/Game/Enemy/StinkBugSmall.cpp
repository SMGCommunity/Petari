#include "Game/Enemy/StinkBugSmall.hpp"

namespace NrvStinkBugSmall {
    NEW_NERVE(StinkBugSmallNrvWait, StinkBugSmall, Wait);
    NEW_NERVE(StinkBugSmallNrvSearch, StinkBugSmall, Search);
    NEW_NERVE(StinkBugSmallNrvDashSign, StinkBugSmall, DashSign);
    NEW_NERVE(StinkBugSmallNrvDashSignEnd, StinkBugSmall, DashSignEnd);
    NEW_NERVE(StinkBugSmallNrvDash, StinkBugSmall, Dash);
    NEW_NERVE(StinkBugSmallNrvDashEnd, StinkBugSmall, DashEnd);
    NEW_NERVE(StinkBugSmallNrvBack, StinkBugSmall, Back);
    NEW_NERVE(StinkBugSmallNrvHipDropDown, StinkBugSmall, HipDropDown);
    NEW_NERVE(StinkBugSmallNrvAttack, StinkBugSmall, Attack);
    NEW_NERVE(StinkBugSmallNrvSpinReaction, StinkBugSmall, SpinReaction);
    NEW_NERVE(StinkBugSmallNrvShakeStart, StinkBugSmall, ShakeStart);
    NEW_NERVE(StinkBugSmallNrvShake, StinkBugSmall, Shake);
    NEW_NERVE(StinkBugSmallNrvPanic, StinkBugSmall, Panic);
    NEW_NERVE(StinkBugSmallNrvRecover, StinkBugSmall, Recover);
    NEW_NERVE(StinkBugSmallNrvDPDSwoon, StinkBugSmall, DPDSwoon);
    NEW_NERVE(StinkBugSmallNrvForceFall, StinkBugSmall, ForceFall);
}  // namespace NrvStinkBugSmall

StinkBugSmall::StinkBugSmall(const char* pName) : StinkBugBase(pName), mScaleController(nullptr), mBindStarPointer(nullptr), _C4(nullptr) {
}

void StinkBugSmall::init(const JMapInfoIter& rIter) {
    StinkBugBase::init(rIter);
    MR::getJMapInfoArg0NoInit(rIter, &_B0);
    MR::getJMapInfoArg1NoInit(rIter, &mRadius);
    MR::getJMapInfoArg7NoInit(rIter, &_C4);
    initHitSensor(3);
    TVec3f v9;
    if (_C4) {
        v9.y = 10.0f * mScale.x;
        v9.x = 0.0f;
        v9.z = 0.0f;
        MR::addHitSensorEnemy(this, "body", 8, 110.0f * mScale.x, v9);
    } else {
        MR::addBodyMessageSensorEnemy(this);
    }
    TVec3f v8;
    v8.y = -30.0f * mScale.x;
    v8.x = 0.0f;
    v8.z = 0.0f;
    MR::addHitSensorAtJointEnemyAttack(this, "head", "Face", 8u, 100.0f * mScale.x, v8);
    if (!_C4) {
        MR::initCollisionParts(this, "Switch", getSensor("body"), nullptr);
    }
    initBinder(200.0f * mScale.x, 200.0f * mScale.x, nullptr);
    MR::setBinderExceptActor(this, this);
    MR::setBinderIgnoreMovingCollision(this);
    if (!MR::isValidSwitchDead(this)) {
        MR::declareStarPiece(this, 3);
    }
    TVec3f v7;
    v7.y = 50.0f * mScale.x;
    v7.x = 0.0f;
    v7.z = 0.0f;
    MR::initStarPointerTarget(this, 130.0f * mScale.x, v7);
    mScaleController = new AnimScaleController(nullptr);
    mBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
    MR::useStageSwitchReadA(this, rIter);
    MR::startBrk(this, "Death");
    MR::setBrkFrameAndStop(this, 0.0f);
    initNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance);
    makeActorAppeared();
}

void StinkBugSmall::exeWait() {
    if (MR::isFirstStep(this) && _B0 == 0.0f) {
        MR::tryStartBck(this, "Search", nullptr);
    }
    fixInitPos();
    if (isPlayerInTerritory(400.0f, 600.0f, 200.0f, 200.0f)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance);
    } else if (_B0 != 0.0f) {
        if (MR::isGreaterStep(this, 1)) {
            setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSearch::sInstance);
        }
    }
}

void StinkBugSmall::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Search", nullptr);
    }
    fixInitPos();
    if (tryTurnSearch(1.0f)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance);
    } else if (isPlayerInTerritory(400.0f, 600.0f, 200.0f, 200.0f)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance);
    }
}
void StinkBugSmall::exeDashSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStart", nullptr);
        MR::startSound(this, "SE_EV_STINKBUG_S_FIND", -1, -1);
    }
    MR::startLevelSound(this, "SE_EM_STINKBUG_S_DASH_SIGN", -1, -1, -1);
    fixInitPos();
    tryTurnDashSign(3.0f);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSignEnd::sInstance);
    }
}
void StinkBugSmall::exeDash() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Rush", nullptr);
        MR::validateHitSensors(this);
    }
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_DASH", -1, -1, -1);
    if (MR::isNear(this, _98, mRadius)) {
        if (!MR::isBindedWall(this)) {
            return;
        }
    }
    mVelocity.zero();
    setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashEnd::sInstance);
    MR::startSound(this, "SE_EM_STINKBUG_S_DASH_END", -1, -1);
    setDashVelocity(20.0f);
}
void StinkBugSmall::exeDashEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStop", nullptr);
    }
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeBack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Back", nullptr);
        MR::invalidateHitSensor(this, "head");
    }
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_BACK", -1, -1, -1);
    if (MR::isNear(this, _98, 10.0f)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance);
        return;
    }
    TVec3f tvf(_98 - mPosition);
    MR::normalize(&tvf);
    MR::turnVecToPlane(&tvf, tvf, mGravity);
    mVelocity.scale(5.0f, tvf);
}

void StinkBugSmall::exeHipDropDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Death", nullptr);
        MR::startBrk(this, "Death");
        if (!_C4) {
            MR::invalidateCollisionParts(this);
        }
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
        MR::startSound(this, "SE_EV_STINKBUG_S_STOMPED", -1, -1);
    }
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        kill();
    }
}

void StinkBugSmall::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
    }
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeSpinReaction() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SpinAction", nullptr);
    }
    MR::startSound(this, "SE_EM_GUARD_S", -1, -1);
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeShakeStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "repel", nullptr);
    }
    if (MR::isStep(this, 40)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvShake::sInstance);
    }
}
void StinkBugSmall::exePanic() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Loss", nullptr);
        MR::invalidateHitSensor(this, "head");
    }
    MR::startLevelSound(this, "SE_EV_LV_STINKBUG_S_PANIC", -1, -1, 15);
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_PANIC", -1, -1, -1);
    mVelocity.zero();
    if (!MR::isOnPlayer(getSensor("body"))) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvRecover::sInstance);
        return;
    }
    if (!MR::isStep(this, 90)) {
        return;
    }
    setNerve(&NrvStinkBugSmall::StinkBugSmallNrvShakeStart::sInstance);
}
void StinkBugSmall::exeRecover() {
    mVelocity.zero();
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_PANID", -1, -1, -1);
    if (MR::isStep(this, 60)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
    }
}
void StinkBugSmall::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffectAll(this);
    }
    if (MR::isEffectValid(this, "RushSmoke")) {
        MR::deleteEffect(this, "RushSmoke");
    }
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, &NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
}
void StinkBugSmall::exeForceFall() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::calcGravity(this);
        MR::onBind(this);
    }
    mVelocity.scaleAdd(2.0f, mGravity, mVelocity);
}

void StinkBugSmall::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    if (!MR::isValidSwitchDead(this)) {
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    LiveActor::kill();
}

void StinkBugSmall::initAfterPlacement() {
    TVec3f v8;
    TVec3f v7;
    MR::calcUpVec(&v8, this);
    v8.scaleAdd(v7, mPosition, 50.0f);
    MR::calcUpVec(&v7, this);
    v7.x *= -200.0f;
    v7.y *= -200.0f;
    v7.z *= -200.0f;
    HitSensor* pSensor = getSensor("body");
    MR::getFirstPolyOnLineToMapExceptSensor(&_98, nullptr, v8, v7, pSensor);
    MR::resetPosition(this, _98);
}

void StinkBugSmall::control() {
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        kill();
        return;
    }
    mScaleController->updateNerve();
    if (tryDPDSwoon() || !tryForceFall()) {
        return;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvForceFall::sInstance)) {
        return;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDash::sInstance) || isNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance)) {
        MR::offBind(this);
        return;
    }
    TVec3f v9;
    MR::onBind(this);
    if (MR::isBindedGround(this)) {
        MR::getGroundNormal(this);
        v9.negate();
        v9.z = -v9.y;
        mGravity.set(v9);
    } else {
        mGravity.set(mGravity);
    }
    JMAVECScaleAdd(&mGravity, &mVelocity, &mVelocity, -mGravity.dot(mVelocity));
    JMAVECScaleAdd(&mGravity, &mVelocity, &mVelocity, 2.0f);
}

void StinkBugSmall::calcAndSetBaseMtx() {
    TPos3f v9;
    MR::calcMtxFromGravityAndZAxis(&v9, this, mGravity, _8C);
    MtxPtr v2 = getBaseMtx();
    MR::blendMtx(v2, v9, 0.3f, v9);
    MR::setBaseTRMtx(this, v9);
    TVec3f scale;
    JMathInlineVEC::PSVECMultiply(scale, mScaleController->_C, scale);

    MR::setBaseScale(this, scale);
}

void StinkBugSmall::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return;
    }
    if (_C4 && !getSensor("body")) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }
    if (!isNerve(&NrvStinkBugSmall::StinkBugSmallNrvAttack::sInstance) && !isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance) &&
        MR::isSensorPlayer(pSender) && MR::isSensorEnemyAttack(pReceiver)) {
        if (MR::isPlayerDamaging()) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }
        if (isHitHorn(pSender, pReceiver, mScale.x * 20.0f) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
            if (!isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDash::sInstance)) {
                setNerve(&NrvStinkBugSmall::StinkBugSmallNrvAttack::sInstance);
            }
        }
    }
}

bool StinkBugSmall::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (getSensor("body")) {
        return false;
    }
    if (MR::isMsgInvincibleAttack(msg)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance);
        return true;
    }
    if (MR::isMsgPlayerHipDropFloor(msg) || MR::isMsgPlayerHipDrop(msg)) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance);
        return true;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return false;
    }
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }
    if (MR::isMsgStarPieceReflect(msg) && !getSensor("body")) {
        if (isNrvEnableStarPieceAttack()) {
            setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance);
            return true;
        }
    }
    if (!isNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance) && !isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDash::sInstance) &&
        MR::isMsgPlayerSpinAttack(msg) && !getSensor("body")) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvSpinReaction::sInstance);
        return true;
    }
    return false;
}

bool StinkBugSmall::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (MR::isPlayerElementModeInvincible() && (MR::isMsgFloorTouch(msg) || MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg))) {
        setNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance);
        return true;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvPanic::sInstance)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvShakeStart::sInstance)) {
        return false;
    }

    if (MR::isMsgFloorTouch(msg)) {
        if (MR::isOnPlayer(getSensor("body"))) {
            if (StinkBugSmall::isShakeChance()) {
                MR::sendMsgEnemyAttackFlip(pReceiver, pSender);
            } else {
                setNerve(&NrvStinkBugSmall::StinkBugSmallNrvPanic::sInstance);
                return true;
            }
        }
    }
    return false;
}

bool StinkBugSmall::tryDPDSwoon() {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return false;
    }
    if (!mBindStarPointer->tryStartPointBind()) {
        return false;
    }
    setNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance);
    return true;
}

bool StinkBugSmall::tryForceFall() {
    if (!MR::isValidSwitchA(this) || !MR::isOnSwitchA(this)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDPDSwoon::sInstance)) {
        return false;
    }
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvForceFall::sInstance)) {
        return false;
    }
    setNerve(&NrvStinkBugSmall::StinkBugSmallNrvForceFall::sInstance);
    return true;
}

bool StinkBugSmall::isShakeChance() const {
    if (isNerve(&NrvStinkBugSmall::StinkBugSmallNrvHipDropDown::sInstance)) {
        return false;
    }
    if (!isNerve(&NrvStinkBugSmall::StinkBugSmallNrvShake::sInstance)) {
        return false;
    }
    if (MR::isPlayerHipDropFalling()) {
        return false;
    }
    return MR::isPlayerHipDropLand() == false;
}

bool StinkBugSmall::isNrvEnableStarPieceAttack() const {
    return isNerve(&NrvStinkBugSmall::StinkBugSmallNrvWait::sInstance) || isNerve(&NrvStinkBugSmall::StinkBugSmallNrvSearch::sInstance) ||
           isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSign::sInstance) || isNerve(&NrvStinkBugSmall::StinkBugSmallNrvDashSignEnd::sInstance) ||
           isNerve(&NrvStinkBugSmall::StinkBugSmallNrvBack::sInstance);
}

StinkBugSmall::~StinkBugSmall() {
}
