#include "Game/Enemy/BasaBasa.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/AnimStampController.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace NrvBasaBasa {
    NEW_NERVE(BasaBasaNrvWait, BasaBasa, Wait);
    NEW_NERVE(BasaBasaNrvAirWait, BasaBasa, AirWait);
    NEW_NERVE(BasaBasaNrvChaseStart, BasaBasa, ChaseStart);
    NEW_NERVE(BasaBasaNrvChase, BasaBasa, Chase);
    NEW_NERVE(BasaBasaNrvQuickTurnStart, BasaBasa, QuickTurnStart);
    NEW_NERVE(BasaBasaNrvQuickTurn, BasaBasa, QuickTurn);
    NEW_NERVE(BasaBasaNrvPunchDown, BasaBasa, PunchDown);
    NEW_NERVE(BasaBasaNrvTrampleDown, BasaBasa, TrampleDown);
    NEW_NERVE(BasaBasaNrvAttackStart, BasaBasa, AttackStart);
    NEW_NERVE(BasaBasaNrvAttack, BasaBasa, Attack);
    NEW_NERVE(BasaBasaNrvAttackEnd, BasaBasa, AttackEnd);
    NEW_NERVE(BasaBasaNrvAttackEndRecover, BasaBasa, AttackEndRecover);
    NEW_NERVE(BasaBasaNrvHitBack, BasaBasa, HitBack);
    NEW_NERVE(BasaBasaNrvHitBackEnd, BasaBasa, HitBackEnd);
    NEW_NERVE(BasaBasaNrvComeHome, BasaBasa, ComeHome);
    NEW_NERVE(BasaBasaNrvAttachCelling, BasaBasa, AttachCelling);
    NEW_NERVE_ONEND(BasaBasaNrvDPDSwoon, BasaBasa, DPDSwoon, DPDSwoon);
    NEW_NERVE(BasaBasaNrvStun, BasaBasa, Stun);
}; // namespace NrvBasaBasa

BasaBasa::BasaBasa(const char* pName)
    : LiveActor(pName),
      mHangModel(nullptr),
      mScaleController(nullptr),
      mStampController(nullptr),
      mSpinHitController(nullptr),
      _9C(0.0f, 0.0f, 1.0f),
      _A8(2000.0f),
      _AC(0.0f),
      _B4(0),
      _B8(0.0f),
      _BC(0.0f, 0.0f, 0.0f),
      mIsIceModel(false),
      _CC(0.0f, 0.0f, 0.0f),
      _E4(-1.0f),
      _E8(0),
      _EC(0) {
}

void BasaBasa::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* objName;
    MR::getObjectName(&objName, rIter);
    mIsIceModel = MR::isEqualString(objName, "BasaBasaIce");
    initModelManagerWithAnm(objName, nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(2);
    MR::addHitSensorAtJointEnemy(this, "body", "JointRoot", 8, 90.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemyAttack(this, "attack", "JointRoot", 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(110.0f, 0.0f, 0);
    MR::initLightCtrl(this);
    initEffectKeeper(1, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, false);
    MR::initShadowVolumeSphere(this, 100.0f);
    MR::validateClipping(this);
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        _B4 = &MR::getRailPos(this);
    } else {
        _B4 = MR::getPlayerCenterPos();
    }

    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);
    MR::onCalcGravity(this);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::initStarPointerTarget(this, 100.0f, offs);
    mScaleController = new AnimScaleController(nullptr);
    mStampController = new AnimStampController();
    mSpinHitController = new SpinHitController(this, 20, 20, 5, 0.0f, 20.0f, 10.0f, 3, false);
    initHangModel();
    MR::getJMapInfoArg0NoInit(rIter, &_A8);
    MR::getJMapInfoArg1NoInit(rIter, &_E4);
    if (-1.0f == _E4) {
        _E4 = 1000.0f + _A8;
    }

    MR::calcFrontVec(&_9C, this);
    _AC = MR::getRandom(600.0f, 800.0f);
    _B8 = MR::getRandom(-25.0f, 25.0f);
    MR::useStageSwitchSleep(this, rIter);
    makeActorAppeared();
}

void BasaBasa::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        mHangModel->appear();
        _B4 = MR::getPlayerCenterPos();
        MR::validateClipping(this);
        MR::validateHitSensors(this);
        MR::offBind(this);
    }

    if (isNearTarget(_A8)) {
        MR::invalidateClipping(this);
        setNerve(&NrvBasaBasa::BasaBasaNrvChaseStart::sInstance);
    }
}

void BasaBasa::exeAirWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Fly", nullptr);
        if (MR::isExistRail(this)) {
            _B4 = &MR::getRailPos(this);
        } else {
            _B4 = MR::getPlayerCenterPos();
        }

        MR::validateClipping(this);
        MR::validateHitSensors(this);
    }

    tuneHeight();
    if (isNearTarget(_A8)) {
        MR::invalidateClipping(this);
        setNerve(&NrvBasaBasa::BasaBasaNrvChase::sInstance);
    } else if (!tryComeHome()) {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeChaseStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FlyStart", nullptr);
        MR::startSound(this, "SE_EV_BASABASA_FIND", -1, -1);
    }

    if (MR::isLessStep(this, 15)) {
        JMAVECScaleAdd(&mGravity, &mVelocity, &mVelocity, 15.0f);
    }

    if (MR::isStep(this, 15)) {
        MR::onBind(this);
    }

    if (MR::isStep(this, 34)) {
        mHangModel->kill();
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvChase::sInstance);
    }
}

void BasaBasa::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Fly", nullptr);
        MR::onBind(this);
    }

    TVec3f v5;
    v5.sub(*_B4, mPosition);
    TVec3f* gravityPtr = &mGravity;
    f32     dot = gravityPtr->dot(v5);
    JMAVECScaleAdd(gravityPtr, &v5, &v5, -dot);
    MR::normalizeOrZero(&v5);
    if (!MR::isNearZero(v5, 0.001f)) {
        TVec3f v4;
        v4.setPS(v5);
        MR::rotateVecDegree(&v4, mGravity, _B8);
        MR::turnDirectionDegree(this, &_9C, v4, 1.7f);
    }

    JMAVECScaleAdd(&_9C, &mVelocity, &mVelocity, 2.0f);
    tuneHeight();
    if (!trySetNerveDPDSwoon()) {
        if (MR::isNearPlayer(_CC, _E4) && MR::isNearPlayer(this, _AC)) {
            setNerve(&NrvBasaBasa::BasaBasaNrvAttackStart::sInstance);
        } else if (!MR::isFaceToTargetHorizontalDegree(this, *_B4, _9C, 60.0f)) {
            setNerve(&NrvBasaBasa::BasaBasaNrvQuickTurnStart::sInstance);
        } else {
            if (tryComeHome()) {
                return;
            }
        }
    }
}

void BasaBasa::exeQuickTurnStart() {
    JMAVECScaleAdd(&_9C, &mVelocity, &mVelocity, 2.0f);
    if (MR::isStep(this, 45)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvQuickTurn::sInstance);
    } else {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeQuickTurn() {
    JMAVECScaleAdd(&_9C, &mVelocity, &mVelocity, 2.0f);
    MR::turnDirectionToTargetDegree(this, &_9C, *_B4, 2.5500002f);
    if (MR::isFaceToTargetHorizontalDegree(this, *_B4, _9C, 2.5500002f)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvChase::sInstance);
    } else {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeTrampleDown() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(this);
        mVelocity.zero();
        mStampController->startAnim(mGravity);
        MR::invalidateHitSensors(this);
        MR::startSound(this, "SE_EV_BASABASA_DAMAGE", -1, -1);
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
    }

    mStampController->updateNerve();

    if (mStampController->isAnimEnd()) {
        MR::appearCoinPop(this, mPosition, 1);
        kill();
    }
}

void BasaBasa::exePunchDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Blow", nullptr);
        MR::invalidateClipping(this);
        MR::invalidateHitSensors(this);
        MR::startSound(this, "SE_EV_BASABASA_DAMAGE", -1, -1);
        MR::startBlowHitSound(this);
        MR::onBind(this);
    }

    mSpinHitController->execute(this);
}

void BasaBasa::exeAttackStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "AttackStart", nullptr);
    }

    MR::turnDirectionToTargetDegree(this, &_9C, *_B4, 2.5500002f);
    TVec3f v3;
    JMAVECScaleAdd(&_9C, &mGravity, &v3, 2.0f);
    JGeometry::negateInternal(&v3.x, &v3.x);
    MR::normalizeOrZero(&v3);
    if (MR::isNearZero(v3, 0.001f)) {
        TVec3f gravityNeg;
        JGeometry::negateInternal(&mGravity.x, &gravityNeg.x);
        v3.set< f32 >(gravityNeg);
    }

    JMAVECScaleAdd(&v3, &mVelocity, &mVelocity, 0.2f);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvAttack::sInstance);
    } else {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
        MR::startSound(this, "SE_EV_BASABASA_ATTACK", -1, -1);
        TVec3f playerUp;
        MR::getPlayerUpVec(&playerUp);
        JMAVECScaleAdd(&playerUp, MR::getPlayerPos(), &_BC, 100.0f);
    }

    MR::startLevelSound(this, "SE_EM_LV_BASABASA_ATTACK", -1, -1, -1);
    TVec3f v9;
    v9.sub(_BC, mPosition);
    if (mIsIceModel) {
        TVec3f v8;
        MR::calcUpVec(&v8, this);
        TVec3f v7;
        JMAVECScaleAdd(&v8, &v9, &v7, -v8.dot(v9));
        TVec3f v6;
        v6.scale(v8.dot(v9), v8);
        JMAVECScaleAdd(&v6, &v7, &v9, 5.0f);
    }
    TVec3f v5;
    MR::normalize(v9, &v5);
    MR::turnDirectionToTargetDegree(this, &_9C, _BC, 2.5500002f);
    JMAVECScaleAdd(&v5, &mVelocity, &mVelocity, 10.0f);
    if (MR::isNear(this, _BC, 40.0f) || MR::isBindedWall(this) || MR::isStep(this, 180)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvAttackEnd::sInstance);
    } else {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeAttackEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "AttackEnd", nullptr);
        if (MR::getShadowProjectionLength(this, nullptr) < 200.0f) {
            MR::emitEffect(this, "AttackEnd");
        }
    }

    JMAVECScaleAdd(&_9C, &mVelocity, &mVelocity, 0.1f);
    if (MR::isStep(this, 50)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvAttackEndRecover::sInstance);
    } else {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeAttackEndRecover() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
    }

    TVec3f v3;
    JMAVECScaleAdd(&_9C, &mGravity, &v3, -0.5f);
    JGeometry::negateInternal(&v3.x, &v3.x);
    MR::normalize(&v3);
    if (MR::getBckFrame(this) >= 116.0f) {
        JMAVECScaleAdd(&v3, &mVelocity, &mVelocity, 0.30000001f);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvChase::sInstance);
    }

    trySetNerveDPDSwoon();
}

void BasaBasa::exeHitBack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Reaction", nullptr);
        MR::onBind(this);
        MR::turnDirectionToPlayerDegree(this, &_9C, 180.0f);
        MR::startSound(this, "SE_EM_ICEBASA_SPIN_BLOW", -1, -1);
    }

    if (MR::isLessEqualStep(this, 5)) {
        TVec3f v3;
        JMathInlineVEC::PSVECAdd(&_9C, &mGravity, &v3);
        JGeometry::negateInternal(&v3.x, &v3.x);
        MR::normalizeOrZero(&v3);
        if (MR::isNearZero(v3, 0.001f)) {
            TVec3f v2;
            JGeometry::negateInternal(&mGravity.x, &v2.x);
            v3.set< f32 >(v2);
        }
        JMAVECScaleAdd(&v3, &mVelocity, &mVelocity, 15.0f);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvHitBackEnd::sInstance);
    } else {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeHitBackEnd() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Fly", nullptr);
    }

    if (MR::isStep(this, 60)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvChase::sInstance);
    } else {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeComeHome() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Fly", nullptr);
        _B4 = &_CC;
    }

    MR::turnDirectionToTargetDegree(this, &_9C, *_B4, 2.5500002f);
    JMAVECScaleAdd(&_9C, &mVelocity, &mVelocity, 2.0f);
    tuneHeight();
    if (isNearTarget(300.0f)) {
        if (_EC) {
            setNerve(&NrvBasaBasa::BasaBasaNrvAttachCelling::sInstance);
        } else {
            setNerve(&NrvBasaBasa::BasaBasaNrvAirWait::sInstance);
        }
    } else if (MR::isGreaterEqualStep(this, 120) && MR::isNearPlayer(this, _A8) && MR::isNearPlayer(_CC, _E4)) {
        _B4 = MR::getPlayerCenterPos();
        setNerve(&NrvBasaBasa::BasaBasaNrvChase::sInstance);
    } else {
        trySetNerveDPDSwoon();
    }
}

void BasaBasa::exeAttachCelling() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Return", nullptr);
        MR::offBind(this);
        MR::invalidateHitSensors(this);
    }

    MR::turnDirectionDegree(this, &_9C, _D8, 2.5500002f);
    if (MR::isNear(this, _CC, 10.0f)) {
        mVelocity.zero();
        MR::resetPosition(this, _CC);
    } else {
        TVec3f v2;
        v2.sub(_CC, mPosition);
        mVelocity.scale(5.0f, v2);
    }

    if (MR::isStep(this, 42)) {
        mHangModel->appear();
    }

    if (MR::isBckStopped(this)) {
        mVelocity.zero();
        MR::validateHitSensors(this);
        setNerve(&NrvBasaBasa::BasaBasaNrvWait::sInstance);
    }
}

void BasaBasa::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(this);
        MR::emitEffect(this, "Touch");
        mScaleController->startDpdHitVibration();
        MR::startDPDHitSound();
    }

    MR::startDPDFreezeLevelSound(this);

    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", 1, 0)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvAirWait::sInstance);
    }
}

void BasaBasa::endDPDSwoon() {
    MR::deleteEffect(this, "Touch");
    mScaleController->stopAndReset();
}

void BasaBasa::exeStun() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Swoon", nullptr);
        MR::startSound(this, "SE_EV_BASABASA_DAMAGE", -1, -1);
    }

    JMAVECScaleAdd(&mGravity, &mVelocity, &mVelocity, 1.0f);
    if (MR::isBckStopped(this)) {
        setNerve(&NrvBasaBasa::BasaBasaNrvChase::sInstance);
    }
}

void BasaBasa::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
    TVec3f v4;
    JMAVECScaleAdd(&mGravity, &mPosition, &v4, 20.0f);
    TVec3f v3;
    v3.scale(-30.0f, mGravity);
    _EC = MR::getFirstPolyOnLineToMap(&mPosition, nullptr, v4, v3);
    if (_EC) {
        initNerve(&NrvBasaBasa::BasaBasaNrvWait::sInstance);
    } else {
        initNerve(&NrvBasaBasa::BasaBasaNrvAirWait::sInstance);
    }

    _CC.set< f32 >(mPosition);
    MR::calcFrontVec(&_D8, this);
}

void BasaBasa::kill() {
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    mHangModel->kill();
    LiveActor::kill();
}

void BasaBasa::control() {
    MR::startLevelSound(this, "SE_EM_LV_ICEBASA_CHILL", -1, -1, -1);
    mScaleController->updateNerve();
    if (!tryClippingAndResetPos()) {
        controlVelocity();
        updateRailType();
    }
}

void BasaBasa::calcAndSetBaseMtx() {
    TPos3f v11;
    MR::calcMtxFromGravityAndZAxis(&v11, this, mGravity, _9C);
    TPos3f v10;
    v11 = mStampController->_14;
    v11.concat(v10, v11);
    v11.mMtx[0][3] = mPosition.x;
    v11.mMtx[1][3] = mPosition.y;
    v11.mMtx[2][3] = mPosition.z;
    MR::setBaseTRMtx(this, v11);
    MR::setBaseScale(this, mScaleController->_C);
}

void BasaBasa::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    bool v6 = isNerve(&NrvBasaBasa::BasaBasaNrvTrampleDown::sInstance) || isNerve(&NrvBasaBasa::BasaBasaNrvPunchDown::sInstance);

    if (v6) {
        return;
    }

    if (isNerve(&NrvBasaBasa::BasaBasaNrvHitBack::sInstance)) {
        return;
    }

    if (MR::isSensorEnemyAttack(pSender) && MR::isSensorPlayer(pReceiver)) {
        bool v7 = false;
        if (!MR::isPlayerHipDropFalling()) {
            if (mIsIceModel) {
                v7 = MR::sendMsgEnemyAttackFreeze(pReceiver, pSender);
            } else if (!isNerve(&NrvBasaBasa::BasaBasaNrvDPDSwoon::sInstance)) {
                bool v8;
                if (MR::isPlayerSwimming()) {
                    v8 = MR::sendMsgEnemyAttackStrong(pReceiver, pSender);
                } else {
                    v8 = MR::sendMsgEnemyAttack(pReceiver, pSender);
                }

                v7 = v8;
            }
        }

        if (v7) {
            MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
            setNerve(&NrvBasaBasa::BasaBasaNrvHitBack::sInstance);
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else if (MR::isSensorEnemy(pSender)) {
        if (MR::isSensorEnemy(pReceiver)) {
            MR::sendMsgPush(pReceiver, pSender);
            TVec3f v10;
            v10.sub(mPosition, pReceiver->mHost->mPosition);
            MR::normalizeOrZero(&v10);
            if (mVelocity.dot(v10) < 0.0f) {
                TVec3f* velocityPtr = &mVelocity;
                f32     dot = v10.dot(mVelocity);
                JMAVECScaleAdd(&v10, velocityPtr, velocityPtr, -dot);
            }
        }
    }
}

bool BasaBasa::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        if (isNrvEnableStun()) {
            setNerve(&NrvBasaBasa::BasaBasaNrvStun::sInstance);
        }

        return true;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    bool v9 = isNerve(&NrvBasaBasa::BasaBasaNrvTrampleDown::sInstance) || isNerve(&NrvBasaBasa::BasaBasaNrvPunchDown::sInstance);

    if (v9) {
        return false;
    }

    if (mIsIceModel && !MR::isPlayerElementModeIce()) {
        if (MR::isMsgPlayerSpinAttack(msg)) {
            return false;
        } else if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
            MR::sendMsgEnemyAttackFreeze(pSender, pReceiver);
            return true;
        }
    }

    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        MR::tryRumbleDefaultHit(this, 0);
        setNerve(&NrvBasaBasa::BasaBasaNrvTrampleDown::sInstance);
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(&NrvBasaBasa::BasaBasaNrvPunchDown::sInstance);
        return true;
    }

    return false;
}

bool BasaBasa::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBasaBasa::BasaBasaNrvDPDSwoon::sInstance)) {
        return false;
    }

    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    if (MR::isSensorEnemy(pSender) || MR::isSensorMapObj(pSender)) {
        TVec3f v7;
        v7.sub(mPosition, pSender->mHost->mPosition);
        MR::normalizeOrZero(&v7);
        JMAVECScaleAdd(&v7, &mVelocity, &mVelocity, 1.5f);
        return true;
    }

    return false;
}

bool BasaBasa::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    if (MR::isMsgToEnemyAttackBlow(msg)) {
        mSpinHitController->startWithoutStopScene(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(&NrvBasaBasa::BasaBasaNrvPunchDown::sInstance);
        return true;
    }

    if (MR::isMsgToEnemyAttackTrample(msg)) {
        mSpinHitController->startWithoutStopScene(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(&NrvBasaBasa::BasaBasaNrvPunchDown::sInstance);
        return true;
    }

    return false;
}

bool BasaBasa::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!mIsIceModel) {
        return false;
    }

    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    if (MR::isMsgSpinStormRange(msg) && MR::isPlayerElementModeNormal()) {
        if (500.0f < MR::calcDistance(pSender, pReceiver, nullptr)) {
            return false;
        } else {
            setNerve(&NrvBasaBasa::BasaBasaNrvHitBack::sInstance);
            return false;
        }
    }

    return false;
}

void BasaBasa::initHangModel() {
    const char* modelName = mIsIceModel ? "BasaBasaIce" : "BasaBasaHang";
    mHangModel = MR::createModelObjEnemy("ぶら下がりモデル", modelName, MR::getJointMtx(this, "JointRoot"));
    mHangModel->initWithoutIter();
    mHangModel->kill();
}

bool BasaBasa::tryClippingAndResetPos() {
    if (isNerve(&NrvBasaBasa::BasaBasaNrvAttachCelling::sInstance)) {
        return false;
    }

    if (isNerve(&NrvBasaBasa::BasaBasaNrvChaseStart::sInstance)) {
        return false;
    }

    f32 radius = getSensor("body")->mRadius;
    if (!MR::isJudgedToClipFrustum(mPosition, radius) || MR::isNearPlayer(this, 2500.0f)) {
        _E8 = 0;
        return false;
    } else {
        if (_E8 < 180) {
            _E8++;
            return false;
        } else {
            MR::resetPosition(this, _CC);
            _9C.set< f32 >(_D8);
            mVelocity.zero();
            _E8 = 0;
            if (_EC != 0) {
                setNerve(&NrvBasaBasa::BasaBasaNrvWait::sInstance);
            } else {
                setNerve(&NrvBasaBasa::BasaBasaNrvAirWait::sInstance);
            }
            return true;
        }
    }
}

bool BasaBasa::trySetNerveDPDSwoon() {
    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        return false;
    }

    setNerve(&NrvBasaBasa::BasaBasaNrvDPDSwoon::sInstance);
    return true;
}

bool BasaBasa::tryComeHome() {
    if (MR::isNear(this, _CC, _E4) && isNearTarget(_A8)) {
        return false;
    }

    setNerve(&NrvBasaBasa::BasaBasaNrvComeHome::sInstance);
    return true;
}

void BasaBasa::updateRailType() {
    if (MR::isExistRail(this) && _B4 == &MR::getRailPos(this)) {
        MR::moveCoord(this, 1.0f);
        if (MR::isNearPlayer(_CC, _A8)) {
            _B4 = MR::getPlayerPos();
        }
    }
}

void BasaBasa::controlVelocity() {
    bool v1 = false;
    if (isNerve(&NrvBasaBasa::BasaBasaNrvTrampleDown::sInstance) || isNerve(&NrvBasaBasa::BasaBasaNrvPunchDown::sInstance)) {
        v1 = true;
    }

    if (!v1) {
        TVec3f v15;
        MR::calcSideVec(&v15, this);
        f32    v3 = v15.dot(mVelocity);
        TVec3f v14;
        v14.scale(v3, v15);
        v14.scale(0.5f);
        TVec3f* velocity = &mVelocity;
        f32     v4 = v15.dot(*velocity);
        JMAVECScaleAdd(&v15, velocity, velocity, -v4);
        JMathInlineVEC::PSVECAdd(&mVelocity, &v14, &mVelocity);
        f32 v5 = 0.94999999f;
        if (isNerve(&NrvBasaBasa::BasaBasaNrvAttackEnd::sInstance)) {
            v5 = 0.95999998f;
        } else if (isNerve(&NrvBasaBasa::BasaBasaNrvChaseStart::sInstance)) {
            v5 = 0.44999999f;
        }

        mVelocity.scale(v5);
        f32 v6;
        if (isNerve(&NrvBasaBasa::BasaBasaNrvChaseStart::sInstance)) {
            v6 = 15.0f;
        } else if (isNerve(&NrvBasaBasa::BasaBasaNrvHitBack::sInstance)) {
            v6 = 15.0f;
        } else if (isNerve(&NrvBasaBasa::BasaBasaNrvQuickTurn::sInstance)) {
            v6 = 8.0f;
        } else if (isNerve(&NrvBasaBasa::BasaBasaNrvAttack::sInstance)) {
            v6 = 23.0f;
        } else {
            v6 = 8.0f;
        }

        if (PSVECMag(&mVelocity) > v6) {
            TVec3f* velocityPtr = &mVelocity;
            f32     sqr = JMathInlineVEC::PSVECSquareMag(velocityPtr);
            if (sqr <= 0.0000038146973f) {
                velocityPtr->scale(v6 * JGeometry::TUtil< f32 >::inv_sqrt(sqr));
            }
        } else {
            if (MR::isNearZero(mVelocity, 0.001f)) {
                mVelocity.zero();
            }
        }
    }
}

void BasaBasa::tuneHeight() {
    if (MR::isShadowProjected(this, nullptr)) {
        TVec3f v5;
        if (isNearTarget(_AC)) {
            v5.sub(*_B4, mPosition);
            TVec3f* gravityPtr = &mGravity;
            v5.scale(gravityPtr->dot(v5), *gravityPtr);
        } else {
            v5.scale(MR::getShadowProjectionLength(this, nullptr) - (_AC - 350.0f), mGravity);
        }

        if (PSVECMag(&v5) < 2.0f) {
            return;
        } else {
            TVec3f v4;
            v4.set< f32 >(v5);
            MR::normalize(&v4);
            JMAVECScaleAdd(&v4, &mVelocity, &mVelocity, 2.0f);
        }
    }
}

bool BasaBasa::isNearTarget(f32 a1) const {
    if (_B4 == MR::getPlayerCenterPos()) {
        return MR::isNearPlayer(this, a1);
    } else {
        return MR::isNear(this, *_B4, a1);
    }
}

bool BasaBasa::isNrvEnableStun() const {
    bool v1 = isNerve(&NrvBasaBasa::BasaBasaNrvTrampleDown::sInstance) || isNerve(&NrvBasaBasa::BasaBasaNrvPunchDown::sInstance);

    if (v1) {
        return false;
    }

    v1 = !isNerve(&NrvBasaBasa::BasaBasaNrvWait::sInstance) && !isNerve(&NrvBasaBasa::BasaBasaNrvAttachCelling::sInstance);

    return v1;
}

BasaBasa::~BasaBasa() {
}
