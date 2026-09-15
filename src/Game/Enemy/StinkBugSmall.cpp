#include "Game/Enemy/StinkBugSmall.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

void DUMMY() {
    TVec3f a, b;
    a.set(b);
}

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
    NEW_NERVE_ONEND(StinkBugSmallNrvDPDSwoon, StinkBugSmall, DPDSwoon, DPDSwoon);
    NEW_NERVE(StinkBugSmallNrvForceFall, StinkBugSmall, ForceFall);
};  // namespace NrvStinkBugSmall

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
    initNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvWait));
    makeActorAppeared();
}

void StinkBugSmall::exeWait() {
    if (MR::isFirstStep(this) && _B0 == 0.0f) {
        MR::tryStartBck(this, "Search", nullptr);
    }
    fixInitPos();
    if (isPlayerInTerritory(400.0f, 600.0f, 200.0f, 200.0f)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDashSign));
    } else if (_B0 != 0.0f) {
        if (MR::isGreaterStep(this, 1)) {
            setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvSearch));
        }
    }
}

void StinkBugSmall::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Search", nullptr);
    }
    fixInitPos();
    if (tryTurnSearch(1.0f)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvWait));
    } else if (isPlayerInTerritory(400.0f, 600.0f, 200.0f, 200.0f)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDashSign));
    }
}
void StinkBugSmall::exeDashSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStart", nullptr);
        MR::startSound(this, "SE_EV_STINKBUG_S_FIND");
    }
    MR::startLevelSound(this, "SE_EM_STINKBUG_S_DASH_SIGN");
    fixInitPos();
    tryTurnDashSign(3.0f);
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDashSignEnd));
    }
}
void StinkBugSmall::exeDashSignEnd() {
    if (MR::isStep(this, 10)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDash));
    }
}
void StinkBugSmall::exeDash() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Rush", nullptr);
        MR::validateHitSensors(this);
    }
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_DASH");
    if (!MR::isNear(this, _98, mRadius) || MR::isBindedWall(this)) {
        mVelocity.zero();
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDashEnd));
        MR::startSound(this, "SE_EM_STINKBUG_S_DASH_END");
        return;
    }
    setDashVelocity(20.0f);
}
void StinkBugSmall::exeDashEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStop", nullptr);
    }
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvBack));
    }
}
void StinkBugSmall::exeBack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Back", nullptr);
        MR::invalidateHitSensor(this, "head");
    }
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_BACK");
    if (MR::isNear(this, _98, 10.0f)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvWait));
        return;
    }
    TVec3f tvf;
    tvf.sub(_98, mPosition);
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
        MR::startSound(this, "SE_EM_STOMPED_S");
        MR::startSound(this, "SE_EV_STINKBUG_S_STOMPED");
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
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvBack));
    }
}
void StinkBugSmall::exeSpinReaction() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SpinAction", nullptr);
    }
    MR::startSound(this, "SE_EM_GUARD_S");
    mVelocity.zero();
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvBack));
    }
}
void StinkBugSmall::exeShakeStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "repel", nullptr);
    }
    if (MR::isStep(this, 40)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvShake));
    }
}
void StinkBugSmall::exeShake() {
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvBack));
    }
}
void StinkBugSmall::exePanic() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Loss", nullptr);
        MR::invalidateHitSensor(this, "head");
    }
    MR::startLevelSound(this, "SE_EV_LV_STINKBUG_S_PANIC", -1, -1, 15);
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_PANIC");
    mVelocity.zero();
    if (!MR::isOnPlayer(getSensor("body"))) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvRecover));
        return;
    }
    if (!MR::isStep(this, 90)) {
        return;
    }
    setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvShakeStart));
}

void StinkBugSmall::exeRecover() {
    mVelocity.zero();
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_S_PANID");
    if (MR::isStep(this, 60)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvBack));
    }
}
void StinkBugSmall::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffectAll(this);
    }
    if (MR::isEffectValid(this, "RushSmoke")) {
        MR::deleteEffect(this, "RushSmoke");
    }
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, GET_NERVE(StinkBugSmall, StinkBugSmallNrvBack));
}
void StinkBugSmall::endDPDSwoon() {
    mBindStarPointer->kill();
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
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    }
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");
    LiveActor::kill();
}

void StinkBugSmall::initAfterPlacement() {
    TVec3f v8;
    TVec3f v7;
    MR::calcUpVec(&v8, this);
    v7.scaleAdd(50.0f, v8, mPosition);
    TVec3f up;
    MR::calcUpVec(&up, this);
    up.scale(-200.0f);
    HitSensor* pSensor = getSensor("body");
    MR::getFirstPolyOnLineToMapExceptSensor(&_98, nullptr, v7, up, pSensor);
    MR::resetPosition(this, _98);
}

void StinkBugSmall::control() {
    // FIXME
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        kill();
        return;
    }
    mScaleController->updateNerve();
    if (!tryDPDSwoon() && !tryForceFall()) {
        bool b1 = isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvForceFall));
        if (b1 != false) {
            return;
        }
        bool b2 = isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDash)) || isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvBack));
        if (b2 == false) {
            MR::offBind(this);
            return;
        }

        MR::onBind(this);
        if (MR::isBindedGround(this)) {
            mGravity.set(-*MR::getGroundNormal(this));
        } else {
            mGravity.set(mGravity);
        }
        mVelocity.orthogonalize(mGravity);
        mVelocity.scaleAdd(2.0f, mGravity, mVelocity);
        TVec3f a;
        a = _8C;
        MR::turnVecToPlane(&_8C, a, mGravity);
    }
}

void StinkBugSmall::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, _8C);
    MR::blendMtx(getBaseMtx(), mtx, 0.3f, mtx);
    MR::setBaseTRMtx(this, mtx);
    TVec3f scale = mScaleController->_C * mScale;
    MR::setBaseScale(this, scale);
}

void StinkBugSmall::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvHipDropDown))) {
        return;
    }
    if (_C4 && !getSensor("body")) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }
    if (!isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvAttack)) && !isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDPDSwoon)) &&
        MR::isSensorPlayer(pSender) && MR::isSensorEnemyAttack(pReceiver)) {
        if (MR::isPlayerDamaging()) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }
        if (isHitHorn(pSender, pReceiver, mScale.x * 20.0f) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
            if (!isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDash))) {
                setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvAttack));
            }
        }
    }
}

bool StinkBugSmall::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvHipDropDown))) {
        return false;
    }
    if (getSensor("body")) {
        return false;
    }
    if (MR::isMsgInvincibleAttack(msg)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvHipDropDown));
        return true;
    }
    if (MR::isMsgPlayerHipDropFloor(msg) || MR::isMsgPlayerHipDrop(msg)) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvHipDropDown));
        return true;
    }
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDPDSwoon))) {
        return false;
    }
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }
    if (MR::isMsgStarPieceReflect(msg) && !getSensor("body")) {
        if (isNrvEnableStarPieceAttack()) {
            setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvSpinReaction));
            return true;
        }
    }
    if (!isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvSpinReaction)) && !isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDash)) &&
        MR::isMsgPlayerSpinAttack(msg) && !getSensor("body")) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvSpinReaction));
        return true;
    }
    return false;
}

bool StinkBugSmall::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvHipDropDown))) {
        return false;
    }
    if (MR::isPlayerElementModeInvincible() && (MR::isMsgFloorTouch(msg) || MR::isMsgWallTouch(msg) || MR::isMsgCeilTouch(msg))) {
        setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvHipDropDown));
        return true;
    }
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDPDSwoon))) {
        return false;
    }
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvPanic))) {
        return false;
    }
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvShakeStart))) {
        return false;
    }

    if (MR::isMsgFloorTouch(msg)) {
        if (MR::isOnPlayer(getSensor("body"))) {
            if (StinkBugSmall::isShakeChance()) {
                MR::sendMsgEnemyAttackFlip(pReceiver, pSender);
            } else {
                setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvPanic));
                return true;
            }
        }
    }
    return false;
}

bool StinkBugSmall::tryDPDSwoon() {
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvHipDropDown))) {
        return false;
    }
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDPDSwoon))) {
        return false;
    }
    if (!mBindStarPointer->tryStartPointBind()) {
        return false;
    }
    setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDPDSwoon));
    return true;
}

bool StinkBugSmall::tryForceFall() {
    if (!MR::isValidSwitchA(this) || !MR::isOnSwitchA(this)) {
        return false;
    }
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDPDSwoon))) {
        return false;
    }
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvForceFall))) {
        return false;
    }
    setNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvForceFall));
    return true;
}

bool StinkBugSmall::isShakeChance() const {
    if (isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvHipDropDown))) {
        return false;
    }
    if (!isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvShake))) {
        return false;
    }
    if (MR::isPlayerHipDropFalling()) {
        return false;
    }
    return MR::isPlayerHipDropLand() == false;
}

bool StinkBugSmall::isNrvEnableStarPieceAttack() const {
    return isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvWait)) || isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvSearch)) ||
           isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDashSign)) || isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvDashSignEnd)) ||
           isNerve(GET_NERVE(StinkBugSmall, StinkBugSmallNrvBack));
}

StinkBugSmall::~StinkBugSmall() {
}
