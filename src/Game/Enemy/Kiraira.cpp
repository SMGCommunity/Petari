#include "Game/Enemy/Kiraira.hpp"
#include "Game/Enemy/KirairaChain.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    // static const f32 sDriftSpeedMax = 2.4f;
    static const f32 sTurnRate = 2.8f;
    // static const f32 sTurnRateFirst = 0.8f;
    static const s32 sStepToStare = 5;
    // static const f32 sDefaultSpeedOnRail = 100.0f;
    static const f32 sRailGoalOffset = 50.0f;
    static const s32 sIntervalToExplode = 10;
    static const f32 sEyeHitSize = 600.0f;
    static const f32 sMindEyeHitSize = 1500.0f;
    static const f32 sCameraShakeDistance = 3000.0f;
    static const f32 sNoCameraShakeDistance = FLOAT_MAX;
    static const s32 sStepToRecoverSign = 300;
    static const s32 sStepToRecover = 120;
    static const f32 sEyeSensorInRadius = 1500.0f;
    static const f32 sEyeSensorOutRadius = 2000.0f;
};  // namespace

namespace NrvKiraira {
    NEW_NERVE(KirairaNrvWait, Kiraira, Wait);
    NEW_NERVE(KirairaNrvFaceToMario, Kiraira, FaceToMario);
    NEW_NERVE(KirairaNrvFaceToMarioAndStare, Kiraira, FaceToMarioAndStare);
    NEW_NERVE(KirairaNrvExplode, Kiraira, Explode);
    NEW_NERVE(KirairaNrvBeExploded, Kiraira, BeExploded);
    NEW_NERVE(KirairaNrvDead, Kiraira, Dead);
    NEW_NERVE(KirairaNrvRecoverSign, Kiraira, RecoverSign);
    NEW_NERVE(KirairaNrvRecover, Kiraira, Recover);
};  // namespace NrvKiraira

Kiraira::Kiraira(const char* pName)
    : LiveActor(pName), mRailCoordSpeed(5.0f), mRailVec(0.0f, 0.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), _A8(0.0f), mIsRail(false),
      mIsForceDetonated(false), mEyesOpen(true), mSharedGroup(nullptr), mChain(nullptr) {
}

void Kiraira::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Kiraira", nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(3);
    MR::addHitSensorEnemy(this, "body", 8, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEye(this, "eye", 16, ::sEyeHitSize, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEye(this, "mind_eye", 8, ::sMindEyeHitSize, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(6, false);
    MR::initShadowVolumeSphere(this, 90.0f);
    MR::setShadowDropLength(this, nullptr, 4000.0f);
    MR::onCalcShadowOneTime(this, nullptr);
    MR::initLightCtrl(this);
    const char* objName;
    MR::getObjectName(&objName, rIter);
    mIsRail = MR::isEqualString(objName, "KirairaRail");
    if (mIsRail) {
        initRailRider(rIter);
        MR::initAndSetRailClipping(&mRailVec, this, 100.0f, 500.0f);
        MR::getJMapInfoArg2NoInit(rIter, &mRailCoordSpeed);
        MR::setRailCoordSpeed(this, mRailCoordSpeed);
    }
    initNerve(&NrvKiraira::KirairaNrvWait::sInstance);
    MR::useStageSwitchReadB(this, rIter);
    MR::setGroupClipping(this, rIter, 16);
    mSharedGroup = MR::joinToGroupArray(this, rIter, "キライラ軍団", 16);
    MR::addToAttributeGroupSearchTurtle(this);
    if (!mIsRail) {
        mChain = new KirairaChain(this);
        mChain->initWithoutIter();
    }
    MR::calcFrontVec(&mFront, this);
    makeActorAppeared();
}

void Kiraira::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, mFront);
    MR::setBaseTRMtx(this, mtx);
}

void Kiraira::exeWait() {
    if (MR::isFirstStep(this)) {
        if (MR::isValidSwitchB(this)) {
            MR::offSwitchB(this);
        }
        closeEyes();
        MR::startBrk(this, "Wait");
    }
    if (MR::isNearPlayer(this, ::sEyeSensorInRadius)) {
        setNerve(&NrvKiraira::KirairaNrvFaceToMario::sInstance);
    } else {
        drift();
    }
}

void Kiraira::exeFaceToMario() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Wait");
    }
    if (!MR::isNearPlayer(this, ::sEyeSensorOutRadius)) {
        setNerve(&NrvKiraira::KirairaNrvWait::sInstance);
    } else {
        MR::turnDirectionToPlayerDegree(this, &mFront, ::sTurnRate);
        if (MR::isFaceToPlayerHorizontalDegree(this, mFront, ::sTurnRate)) {
            MR::startBck(this, "Stop", static_cast< const char* >(nullptr));
            setNerve(&NrvKiraira::KirairaNrvFaceToMarioAndStare::sInstance);
        } else {
            drift();
        }
    }
}

void Kiraira::exeFaceToMarioAndStare() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Wait");
    }
    if (MR::isStep(this, ::sStepToStare)) {
        openEyes();
    }
    if (!MR::isNearPlayer(this, ::sEyeSensorOutRadius)) {
        setNerve(&NrvKiraira::KirairaNrvWait::sInstance);
    } else {
        MR::turnDirectionToTargetDegree(this, &mFront, *MR::getPlayerPos(), ::sTurnRate);
        drift();
    }
}

void Kiraira::exeBeExploded() {
    if (MR::isFirstStep(this)) {
        mIsForceDetonated = true;
    }
    if (MR::isStep(this, ::sIntervalToExplode)) {
        explode();
    }
}

void Kiraira::exeDead() {
    if (MR::isFirstStep(this)) {
        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }
        if (mIsRail) {
            MR::startBck(this, "RailDown", static_cast< const char* >(nullptr));
        } else {
            MR::startBck(this, "Down", static_cast< const char* >(nullptr));
        }
        MR::startBrk(this, "Down");
        MR::setShadowVolumeSphereRadius(this, static_cast< const char* >(nullptr), 50.0f);
    }
    if (mSharedGroup != nullptr) {
        mSharedGroup->sendMsgToGroupMember(ACTMES_GROUP_ATTACK, getSensor("body"), "body");
    }
    if (MR::isStep(this, ::sStepToRecoverSign)) {
        setNerve(&NrvKiraira::KirairaNrvRecoverSign::sInstance);
    } else {
        drift();
    }
}

void Kiraira::exeRecoverSign() {
    if (MR::isFirstStep(this)) {
        if (mIsRail) {
            MR::startBck(this, "RailRevivalStart", static_cast< const char* >(nullptr));
        } else {
            MR::startBck(this, "RevivalStart", static_cast< const char* >(nullptr));
        }
        MR::startBrk(this, "RevivalStart");
        MR::validateClipping(this);
    }
    if (MR::isStep(this, ::sStepToRecover)) {
        setNerve(&NrvKiraira::KirairaNrvRecover::sInstance);
    } else {
        drift();
    }
}

void Kiraira::exeRecover() {
    if (MR::isFirstStep(this)) {
        if (mIsRail) {
            MR::startBck(this, "Revival", static_cast< const char* >(nullptr));
        } else {
            MR::startBck(this, "RailRevival", static_cast< const char* >(nullptr));
        }
        MR::startBrk(this, "Revival");
        MR::startSound(this, "SE_OJ_KIRAIRA_RECOVER");

        if (MR::isNearPlayer(this, ::sEyeSensorOutRadius)) {
            openEyes();
        }
        MR::setShadowVolumeSphereRadius(this, static_cast< const char* >(nullptr), 90.0f);
        mIsForceDetonated = false;
    }
    if (MR::isNearPlayer(this, ::sEyeSensorOutRadius)) {
        MR::turnDirectionToTargetDegree(this, &mFront, *MR::getPlayerPos(), ::sTurnRate);
    }
    if (MR::isBckStopped(this)) {
        if (mChain != nullptr) {
            mChain->_90 = false;
        }
        if (!MR::isNearPlayer(this, ::sEyeSensorOutRadius)) {
            setNerve(&NrvKiraira::KirairaNrvFaceToMarioAndStare::sInstance);
        } else {
            setNerve(&NrvKiraira::KirairaNrvWait::sInstance);
        }
    }
}

void Kiraira::exeExplode() {
    if (MR::isFirstStep(this)) {
        explode();
    }
}

bool Kiraira::isDown() const {
    return isNerve(&NrvKiraira::KirairaNrvDead::sInstance) || isNerve(&NrvKiraira::KirairaNrvRecoverSign::sInstance) ||
           isNerve(&NrvKiraira::KirairaNrvRecover::sInstance);
}

void Kiraira::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isDown() && (pSender == getSensor("mind_eye")) && pReceiver->mType == ATYPE_JET_TURTLE) {
        MR::turnDirectionToTargetDegree(this, &mFront, pReceiver->mPosition, 20.0f);
        openEyes();
    }
    if (!isDown() && MR::isSensorEnemy(pSender) && MR::isSensorPlayerOrRide(pReceiver)) {
        if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
            setNerve(&NrvKiraira::KirairaNrvExplode::sInstance);
            return;
        }
    }
    if (isDown() && (pSender == getSensor("body")) && MR::isSensorPlayerOrRide(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool Kiraira::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }
    if (MR::isMsgJetTurtleAttack(msg) && MR::isSensorEnemy(pReceiver)) {
        mIsForceDetonated = true;
        setNerve(&NrvKiraira::KirairaNrvExplode::sInstance);
        return true;
    }
    if (MR::isSensorEnemy(pReceiver) && MR::isMsgPlayerHitAll(msg)) {
        if (MR::sendMsgEnemyAttackExplosion(pSender, pReceiver)) {
            setNerve(&NrvKiraira::KirairaNrvExplode::sInstance);
            return true;
        }
    }
    if (MR::isSensorEnemy(pReceiver) && MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvKiraira::KirairaNrvExplode::sInstance);
        return true;
    }

    return false;
}

bool Kiraira::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isDown() && MR::isMsgExplosionAttack(msg) && MR::isSensorEnemy(pReceiver)) {
        MR::invalidateClipping(this);
        setNerve(&NrvKiraira::KirairaNrvBeExploded::sInstance);
        return true;
    }

    return false;
}

bool Kiraira::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isDown()) {
        return false;
    }
    if (msg == ACTMES_GROUP_ATTACK) {
        MR::turnDirectionToTargetDegree(this, &mFront, pSender->mPosition, 20.0f);
        openEyes();
        return true;
    }

    return false;
}

void Kiraira::openEyes() {
    if (!mEyesOpen) {
        MR::startBtp(this, "EyeOpen");
        MR::emitEffect(this, "SearchLight");
        MR::startSound(this, "SE_OJ_KIRAIRA_STARE");
    }
    mEyesOpen = true;
}

void Kiraira::closeEyes() {
    if (mEyesOpen) {
        MR::startBtp(this, "EyeClose");
        MR::deleteEffect(this, "SearchLight");
    }
    mEyesOpen = false;
}

void Kiraira::drift() {
    if (mIsRail) {
        bool alive = true;
        bool dead = true;
        if (isNerve(&NrvKiraira::KirairaNrvDead::sInstance) || !isNerve(&NrvKiraira::KirairaNrvRecoverSign::sInstance)) {
            alive = false;
        }

        if (alive || !isNerve(&NrvKiraira::KirairaNrvRecover::sInstance)) {
            dead = false;
        }
        if (!dead) {
            driftOnRail();
            return;
        }

        if (isNerve(&NrvKiraira::KirairaNrvRecover::sInstance)) {
            mPosition.set(MR::getRailPos(this));
            return;
        }
        if (mChain != nullptr) {
            mChain->_94.set(mPosition);
        }

    } else {
        mVelocity.scale(1.5f * MR::sin(45.0f + 2.0f * _A8), mGravity);
    }
    _A8++;
}

void Kiraira::driftOnRail() {
    MR::moveCoordAndFollowTrans(this, mRailCoordSpeed);
    if (MR::isRailReachedNearGoal(this, ::sRailGoalOffset)) {
        MR::reverseRailDirection(this);
    }
    MR::startLevelSound(this, "SE_OJ_LV_KIRAIRA_CHAIN");
}

void Kiraira::explode() {
    closeEyes();
    MR::startRumbleWithShakeCameraStrong(this, "強", "中", ::sCameraShakeDistance, ::sNoCameraShakeDistance);
    if (mIsForceDetonated) {
        MR::sendMsgExplosionToNearActor(getSensor("eye"), -1.0f);
    }
    MR::emitEffect(this, "Explosion");
    MR::startSound(this, "SE_OJ_KIRAIRA_EXPLODE");
    if (mChain != nullptr) {
        mChain->_90 = true;
    }
    setNerve(&NrvKiraira::KirairaNrvDead::sInstance);
}
