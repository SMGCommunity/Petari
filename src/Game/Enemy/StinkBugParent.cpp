#include "Game/Enemy/StinkBugParent.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

void StinkBugParent_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)3.0f;
    (void)-1.0f;
    (void)2.0f;
}

namespace {
    // sGravity
    static const f32 sTurnRateSearch = 1.0f;
    static const f32 sTurnRateDashSign = 3.0f;
    static const s32 sStepForWait = 1;
    static const s32 sStepForDashSignEnd = 10;
    static const s32 sStepForShakeStart = 40;
    static const s32 sStepForEnableShake = 25;
    static const s32 sStepForRecover = 60;
    static const s32 sStepToJumpBackPlayer = 22;
    // mSearchAngleOffset;
    // sTerritorySizeX
    // sTerritorySizeY
    // sTerritorySizeZPlus
    // sChildRadius
    // sAttackCylinderRadius
    static const s32 sStepForChildDeathSound = 48;
    static const s32 sStepForSurpriseSound = 65;
};  // namespace

namespace NrvStinkBugParent {
    NEW_NERVE(StinkBugParentNrvWait, StinkBugParent, Wait);
    NEW_NERVE(StinkBugParentNrvSearch, StinkBugParent, Search);
    NEW_NERVE(StinkBugParentNrvDashSign, StinkBugParent, DashSign);
    NEW_NERVE(StinkBugParentNrvDashSignEnd, StinkBugParent, DashSignEnd);
    NEW_NERVE(StinkBugParentNrvDash, StinkBugParent, Dash);
    NEW_NERVE(StinkBugParentNrvDashEnd, StinkBugParent, DashEnd);
    NEW_NERVE(StinkBugParentNrvBack, StinkBugParent, Back);
    NEW_NERVE(StinkBugParentNrvHipDropDown, StinkBugParent, HipDropDown);
    NEW_NERVE(StinkBugParentNrvAttack, StinkBugParent, Attack);
    NEW_NERVE(StinkBugParentNrvSpinReaction, StinkBugParent, SpinReaction);
    NEW_NERVE(StinkBugParentNrvPanic, StinkBugParent, Panic);
    NEW_NERVE(StinkBugParentNrvShakeStart, StinkBugParent, ShakeStart);
    NEW_NERVE(StinkBugParentNrvShake, StinkBugParent, Shake);
    NEW_NERVE(StinkBugParentNrvRecover, StinkBugParent, Recover);
    NEW_NERVE(StinkBugParentNrvDemoPrepare, StinkBugParent, DemoPrepare);
    NEW_NERVE(StinkBugParentNrvDemoChildDown, StinkBugParent, DemoChildDown);
    NEW_NERVE(StinkBugParentNrvDemoAnger, StinkBugParent, DemoAnger);
};  // namespace NrvStinkBugParent

namespace {
    const Param sParams[] = {{20.0f, 5.0f, 30}, {25.0f, 10.0f, 20}};

    const Param* getParam(bool isChildAlive) {
        if (!isChildAlive) {
            return &sParams[1];
        }
        return &sParams[0];
    }
};  // namespace

StinkBugParent::StinkBugParent(const char* pName) : StinkBugBase(pName) {
    mParent = nullptr;
    mIsChildAlive = true;
}

StinkBugParent::~StinkBugParent() {
}

void StinkBugParent::init(const JMapInfoIter& rIter) {
    StinkBugBase::init(rIter);
    initHitSensor(2);
    MR::addHitSensorEye(this, "body", 4, 250.0f, TVec3f(0.0f, 150.0f, 0.0f));
    MR::addHitSensorAtJointEnemyAttack(this, "head", "Face", 8, 160.0f, TVec3f(0.0f, -50.0f, 80.0f));
    MR::initCollisionParts(this, "SwitchChildJ", getSensor("body"), nullptr);
    mParent = MR::createCollisionPartsFromLiveActor(this, "Switch", getSensor("body"), getBaseMtx(), MR::CollisionScaleType_Unk2);
    initBinder(300.0f, 300.0f, 0);
    MR::setBinderExceptActor(this, this);
    MR::setBinderIgnoreMovingCollision(this);
    MR::declarePowerStar(this);
    MR::declareStarPiece(this, 8);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvStinkBugParent::StinkBugParentNrvDemoAnger::sInstance, "ダメージ中");
    }

    _B0 = 50.0f;
    mRadius = 1200.0f;
    MR::startBrk(this, "Anger");
    MR::setBrkFrameAndStop(this, 0.0f);
    initNerve(&NrvStinkBugParent::StinkBugParentNrvWait::sInstance);
    makeActorAppeared();
}

void StinkBugParent::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::resetPosition(this, _98);
    }

    fixInitPos();

    if (isPlayerInTerritory(400.0, 200.0f, 600.0f, 200.0f)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvDashSign::sInstance);
    } else if (MR::isGreaterStep(this, ::sStepForWait)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvSearch::sInstance);
    }
}

void StinkBugParent::exeSearch() {
    if (MR::isFirstStep(this)) {
        if (mIsChildAlive) {
            MR::startBck(this, "Search", nullptr);
        } else {
            MR::startBck(this, "AngerSearch", nullptr);
        }
    }

    fixInitPos();

    if (tryTurnSearch(::sTurnRateSearch)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvWait::sInstance);
    } else if (isPlayerInTerritory(400.0f, 200.0f, 600.0f, 200.0f)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvDashSign::sInstance);
    }
}

void StinkBugParent::exeDashSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStart", nullptr);
        MR::startSound(this, "SE_EV_STINKBUG_L_FIND");
    }

    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_L_DASH_SIGN");
    fixInitPos();
    tryTurnDashSign(::sTurnRateDashSign);

    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvDashSignEnd::sInstance);
    }
}

void StinkBugParent::exeDashSignEnd() {
    if (MR::isStep(this, ::sStepForDashSignEnd)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvDash::sInstance);
    }
}

void StinkBugParent::exeDash() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Rush", nullptr);
        MR::validateHitSensors(this);
    }

    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_L_DASH");

    if (!MR::isNear(this, _98, mRadius) || MR::isBindedWall(this)) {
        mVelocity.zero();
        setNerve(&NrvStinkBugParent::StinkBugParentNrvDashEnd::sInstance);
        MR::startSound(this, "SE_EM_STINKBUG_L_DASH_END");
    } else {
        setDashVelocity(getParam(mIsChildAlive)->mDashVelocity);
    }
}

void StinkBugParent::exeDashEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "RushStop", nullptr);
    }

    mVelocity.zero();

    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvBack::sInstance);
    }
}

void StinkBugParent::exeBack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Back", nullptr);
        MR::invalidateHitSensor(this, "head");
    }

    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_L_BACK");

    if (MR::isNear(this, _98, (2.0f * getParam(mIsChildAlive)->mDashDistance))) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvWait::sInstance);
    } else {
        TVec3f a1;
        a1.sub(_98, mPosition);
        MR::normalize(&a1);
        MR::turnVecToPlane(&a1, a1, mGravity);
        mVelocity.scale(getParam(mIsChildAlive)->mDashDistance, a1);
    }
}

void StinkBugParent::exeHipDropDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Death", nullptr);
        MR::startBrk(this, "Death");
        MR::invalidateCollisionParts(mParent);
        MR::invalidateHitSensors(this);
        MR::startSound(this, "SE_EM_STINKBUG_STOMPED_S");
        MR::startSound(this, "SE_EV_STINKBUG_L_STOMPED");
    }

    mVelocity.zero();

    if (MR::isBckStopped(this)) {
        kill();
    }
}

void StinkBugParent::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
    }

    mVelocity.zero();

    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvBack::sInstance);
    }
}

void StinkBugParent::exeSpinReaction() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SpinAction", nullptr);
        MR::startSound(this, "SE_EM_GUARD_S");
    }

    mVelocity.zero();

    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvBack::sInstance);
    }
}

void StinkBugParent::exePanic() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Loss", nullptr);
        MR::invalidateHitSensor(this, "head");
        MR::startSound(this, "SE_EV_STINKBUG_L_PANIC");
    }

    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_L_PANIC");
    mVelocity.zero();

    if (!MR::isOnPlayer(getSensor("body"))) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvRecover::sInstance);
    } else if (MR::isStep(this, getParam(mIsChildAlive)->mPanicStep)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvShakeStart::sInstance);
    }
}

void StinkBugParent::exeRecover() {
    mVelocity.zero();
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_L_PANIC");

    if (MR::isStep(this, ::sStepForRecover)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvBack::sInstance);
    }
}

void StinkBugParent::exeShakeStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Repel", nullptr);
    }

    if (MR::isStep(this, ::sStepForShakeStart)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvShake::sInstance);
    }
}

void StinkBugParent::exeShake() {
    if (MR::isBckStopped(this)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvBack::sInstance);
    }
}

void StinkBugParent::exeDemoPrepare() {
    if (MR::tryStartDemoRegisteredMarioPuppetable(this, nullptr)) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvDemoChildDown::sInstance);
    }
}

void StinkBugParent::exeDemoChildDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ChildDeath", nullptr);
        MR::startBva(this, "ChildDeath");
        MR::startBrk(this, "ChildDeath");
        MR::startSound(this, "SE_EV_STINKBUG_S_STOMPED");
        MR::startSound(this, "SE_EM_STINKBUG_STOMPED_S");
        mIsChildAlive = false;
        MR::invalidateCollisionParts(this);
        MR::validateCollisionParts(mParent);
    }

    if (MR::isStep(this, ::sStepToJumpBackPlayer)) {
        jumpBackPlayerFromChild();
    }

    if (MR::isStep(this, ::sStepForChildDeathSound)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
    }

    if (MR::isStep(this, ::sStepForSurpriseSound)) {
        MR::startSound(this, "SE_EV_STINKBUG_L_SURPRISE");
    }

    mVelocity.zero();
}

void StinkBugParent::exeDemoAnger() {
    if (MR::isFirstStep(this)) {
        TVec3f v1;
        MR::overlayWithPreviousScreen(2);
        MR::startBck(this, "Anger", nullptr);
        MR::startBrk(this, "Anger");
        MR::resetPosition(this, _98);
        MR::setPlayerPosAndWait("子連れカメムシデモ後ポイント");
        MR::findNamePos("子連れカメムシデモ後ポイント", &v1, nullptr);
        MR::turnDirectionToTargetUseGroundNormalDegree(this, &_8C, v1, 180.0f);
    }

    MR::startLevelSound(this, "SE_EV_LV_STINKBUG_L_ANGRY");
    MR::startLevelSound(this, "SE_EM_LV_STINKBUG_L_ANGRY");

    if (MR::isBckStopped(this)) {
        TVec3f v2, v3;
        v2.scaleAdd(200.0f, _8C, mPosition);
        v3.scaleAdd(-1.0f, mGravity, _8C);
        MR::normalize(&v3);
        MR::appearStarPieceToDirection(this, v2, v3, 8, 25.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_F");
        MR::validateHitSensors(this);
        setNerve(&NrvStinkBugParent::StinkBugParentNrvWait::sInstance);
        MR::startSound(this, "SE_EM_STINKBUG_L_ANGRY_END");
    }
}

void StinkBugParent::kill() {
    MR::startSound(this, "SE_EM_EXPLODE_S");
    MR::requestAppearPowerStar(this, mPosition);
    LiveActor::kill();
}

void StinkBugParent::control() {
    if (!mIsChildAlive) {
        mParent->setMtx();
    }

    if (!isNrvMoving()) {
        MR::offBind(this);
    } else {
        MR::onBind(this);
        TVec3f& vel = mVelocity;
        TVec3f& grav = mGravity;
        vel.scaleAdd(-grav.dot(vel), grav, vel);
        mVelocity.scaleAdd(2.0f, mGravity, mVelocity);
    }
}

void StinkBugParent::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, _8C);
    MR::setBaseTRMtx(this, mtx);
}

void StinkBugParent::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvStinkBugParent::StinkBugParentNrvHipDropDown::sInstance) && !isNrvDemo() &&
        !isNerve(&NrvStinkBugParent::StinkBugParentNrvAttack::sInstance) && MR::isSensorPlayer(pReceiver)) {
        if (MR::isSensorEnemyAttack(pSender)) {
            if (!isHitHorn(pSender, pReceiver, 25.0f)) {
                return;
            }

            if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
                MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
                if (!isNerve(&NrvStinkBugParent::StinkBugParentNrvDash::sInstance)) {
                    setNerve(&NrvStinkBugParent::StinkBugParentNrvAttack::sInstance);
                    return;
                }
            }
        }

        if (MR::isSensorEye(pSender) && isShakeChance() && !MR::isPlayerHipDropFalling()) {
            MR::sendMsgEnemyAttackFlipToDir(pReceiver, pSender, _8C);
            return;
        }
    }
}

bool StinkBugParent::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvStinkBugParent::StinkBugParentNrvHipDropDown::sInstance)) {
        return false;
    }

    if (isNrvDemo()) {
        return false;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceReflect(msg) && pReceiver == getSensor("body")) {
        if (isNrvEnableStarPieceAttack()) {
            setNerve(&NrvStinkBugParent::StinkBugParentNrvSpinReaction::sInstance);
        }
        return true;
    } else if (MR::isMsgPlayerHipDropFloor(msg)) {
        if (mIsChildAlive) {
            if (!isHitChild()) {
                return false;
            } else {
                setNerve(&NrvStinkBugParent::StinkBugParentNrvDemoPrepare::sInstance);
            }
        } else {
            setNerve(&NrvStinkBugParent::StinkBugParentNrvHipDropDown::sInstance);
        }
        return true;
    } else if (!isNerve(&NrvStinkBugParent::StinkBugParentNrvSpinReaction::sInstance) &&
               !isNerve(&NrvStinkBugParent::StinkBugParentNrvDash::sInstance) && MR::isMsgPlayerSpinAttack(msg) && pReceiver == getSensor("body")) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvSpinReaction::sInstance);
        return true;
    }

    return false;
}

bool StinkBugParent::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvStinkBugParent::StinkBugParentNrvHipDropDown::sInstance)) {
        return false;
    }

    if (isNrvDemo()) {
        return false;
    }

    if (isNerve(&NrvStinkBugParent::StinkBugParentNrvPanic::sInstance)) {
        return false;
    }

    if (isNerve(&NrvStinkBugParent::StinkBugParentNrvShakeStart::sInstance)) {
        return false;
    }

    if (isNerve(&NrvStinkBugParent::StinkBugParentNrvShake::sInstance)) {
        return false;
    }

    if (MR::isMsgFloorTouch(msg) && MR::isOnPlayer(getSensor("body"))) {
        setNerve(&NrvStinkBugParent::StinkBugParentNrvPanic::sInstance);
        return true;
    }

    return false;
}

void StinkBugParent::jumpBackPlayerFromChild() const {
    TVec3f v10, v9, v8;
    MR::findNamePos("子連れカメムシデモ後ポイント", &v10, nullptr);
    v9.sub(v10, *MR::getPlayerPos());
    MR::normalize(v9, &v8);
    TVec3f v7 = -v8;
    MR::offBind(MR::getPlayerDemoActor());
    MR::unlockPlayerAnimation();
    MR::forceFlyPlayer(v10, v7, 0);
}

bool StinkBugParent::isShakeChance() const {
    if (isNerve(&NrvStinkBugParent::StinkBugParentNrvHipDropDown::sInstance)) {
        return false;
    }

    if (isNrvDemo()) {
        return false;
    }

    if (!isNerve(&NrvStinkBugParent::StinkBugParentNrvShake::sInstance)) {
        return false;
    }

    if (MR::isGreaterStep(this, ::sStepForEnableShake)) {
        return false;
    }

    return MR::isOnPlayer(getSensor("body"));
}

bool StinkBugParent::isHitChild() const {
    if (!mIsChildAlive) {
        return false;
    }

    TVec3f v1, v2, v3, v4;
    MR::copyJointPos(this, "ChildCenter", &v1);
    v2.sub(*MR::getPlayerPos(), v1);
    MR::calcUpVec(&v3, this);
    f32 a = v3.dot(v2);
    v4.scaleAdd(-a, v3, v2);
    return v4.length() <= 150.0f;
}

bool StinkBugParent::isNrvEnableStarPieceAttack() const {
    return isNerve(&NrvStinkBugParent::StinkBugParentNrvWait::sInstance) || isNerve(&NrvStinkBugParent::StinkBugParentNrvSearch::sInstance) ||
           isNerve(&NrvStinkBugParent::StinkBugParentNrvDashSign::sInstance) ||
           isNerve(&NrvStinkBugParent::StinkBugParentNrvDashSignEnd::sInstance) || isNerve(&NrvStinkBugParent::StinkBugParentNrvBack::sInstance);
}

inline bool StinkBugParent::isNrvMoving() const {
    return isNerve(&NrvStinkBugParent::StinkBugParentNrvDash::sInstance) || isNerve(&NrvStinkBugParent::StinkBugParentNrvBack::sInstance);
}

inline bool StinkBugParent::isNrvDemo() const {
    return isNerve(&NrvStinkBugParent::StinkBugParentNrvDemoPrepare::sInstance) ||
           isNerve(&NrvStinkBugParent::StinkBugParentNrvDemoChildDown::sInstance) ||
           isNerve(&NrvStinkBugParent::StinkBugParentNrvDemoAnger::sInstance);
}
