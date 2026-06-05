#include "Game/Boss/Dodoryu.hpp"
#include "Game/Boss/DodoryuDemo.hpp"
#include "Game/Boss/DodoryuHill.hpp"
#include "Game/Boss/DodoryuMove.hpp"
// #include "Game/Boss/DodoryuStateLv2.hpp"
#include "Game/Boss/DodoryuStateBase.hpp"
#include "Game/Boss/DodoryuStateWait.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include <cstdio>

namespace {
    static const Vec sHeadOffset = {50.0f, 30.0f, 0.0f};
    static const Vec sBodyOffset = {0.0f, 0.0f, 30.0f};
    static const Vec sLegOffset = {40.0f, 0.0f, 0.0f};
    static const Vec sArmOffset = {0.0f, 0.0f, 0.0f};
    static const Vec sTailOffset = {50.0f, 0.0f, 0.0f};
    static const Vec sStarPieceSensorOffset = {-150.0f, 0.0f, 0.0f};
    // static _ sDodoryuChaseDataLv2 = _;
    // static _ sDodoryuChaseDataLv3 = _;
    static const char* sHeadSensorName = "head";
    static const char* sHeadJointName = "Head";
    static const f32 sHeadRadius = 210.0f;
    static const char* sBodySensorName = "body";
    static const f32 sBodyRadius = 250.0f;
    static const char* sLegLSensorName = "leg_left";
    static const char* sLegRSensorName = "leg_right";
    static const f32 sLegRadius = 110.0f;
    static const char* sArmLSensorName = "arm_left";
    static const char* sArmRSensorName = "arm_right";
    static const f32 sArmRadius = 50.0f;
    static const char* sTailSensorName = "tail";
    static const f32 sTailRadius = 120.0f;
    static const char* sMapObjSensorName = "eye";
    static const f32 sMapObjSensorRadius = 100.0f;
    static const char* sStarPieceSensorName = "star";
    static const f32 sStarPieceSensorRadius = 400.0f;
    // static const f32 sScale = _;
    // static const _ sEarthCheckLength = _;
    // static const _ sEarthCheckExtra = _;
    // static const TVec3f sCamRegOffset = _;
    // static const s32 sJumpOutStopSceneFrame = _;
    // static const s32 sRumbleInterval = _;
    static const char* sSpinOutCamera = "ドドリュウ吹っ飛び";
    // static const f32 sSpinOutCameraInterpolation = _;
    // static const f32 sSpinOutCameraBlendRate = _;
    // static const f32 sRumblePadNearDist = _;
    // static const f32 sRumblePadFarDist = _;
    // static const _ sPrintStateLabel = _;
    // static const f32 sRabbitDistMax = _;
    // static const f32 sRabbitDistMin = _;
    // static const f32 sRabbitRestDist = _;
    // static const f32 sRabbitFastSpeed = _;
    // static const f32 sRabbitSlowSpeed = _;
    // static const f32 sTalkDistanceLong = _;
    // static const f32 sTalkDistanceShort = _;
    // static const TVec3f sRotateVel = _;
};  // namespace

Dodoryu::Dodoryu(const char* pName)
    : LiveActor(pName), _BC(0.0f, 0.0f, 0.0f), _C8(), _CC(), mState(), mMoveStateHolder(), mHill(), mLeadHill(), mBank(), mRabbit(),
      _128(0.0f, 1.0f, 0.0f), _134(1.0f, 0.0f, 0.0f), _140(), _144(), _148(new CameraTargetMtx("カメラターゲットダミー")),
      _14C(new CameraTargetMtx("カメラターゲットダミー")), _150(), _154(), mAnimScaleCtrl(new AnimScaleController(nullptr)) {
    mBaseMtx.identity();
}

void Dodoryu::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Dodoryu", nullptr, false);

    mMoveStateHolder = new DodoryuSub::MoveStateHolder(this);

    initState();
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::initFur(this);
    initHitSensor();
    initSound(8, false);
    initRailRider(rIter);
    MR::moveCoordToNearestPos(this, mPosition);
    initSwitch(rIter);
    initEffectKeeper(0, nullptr, false);
    MR::initShadowFromCSV(this, "Shadow");
    MR::declareCameraRegisterVec(this, 0, &_BC);
    MR::declareGlobalEventCameraFixedThere(::sSpinOutCamera, true, 0.0f);
    createMogucchiHill();
    // mLeadHill = new DodoryuLeadHill(this);
    createDodoryuBank();
    // mRabbit = new DodoryuRabbit(this, rIter);
    MR::declarePowerStar(this);
    MR::createDepthOfFieldBlur();
    MR::declareStarPiece(this, 24);
    MR::createCenterScreenBlur();
    _148->mMatrix.setInline(MR::getZonePlacementMtx(rIter));
    MR::startBckWithInterpole(this, "UnderGroundWalk", 0);
    MR::startBva(this, "Normal");
    MR::startBrk(this, "Normal");
    mState[_CC]->start();

    if (MR::isValidSwitchB(this)) {
        MR::onSwitchB(this);
    }

    makeActorAppeared();
}

void Dodoryu::initAfterPlacement() {
    snapToGround();
}

void Dodoryu::control() {
    mMoveStateHolder->execute();
    checkHipDrop();
    mState[_CC]->control();
    updateRumblePad();
    updateCameraTarget();
    mAnimScaleCtrl->updateNerve();
}

void Dodoryu::startClipped() {
    LiveActor::startClipped();
    // MR::validateClipping(mRabbit);
    mState[_CC]->catchStartClipped();
}

void Dodoryu::endClipped() {
    LiveActor::endClipped();
    // MR::invalidateClipping(mRabbit);
    mState[_CC]->catchEndClipped();
}

// Dodoryu::snapToGround
// Dodoryu::snapToWall
// Dodoryu::setMtx

void Dodoryu::notifyOnSwitchA() {
    MR::invalidateClipping(this);
    MR::invalidateClipping(mHill);
    // MR::invalidateClipping(mLeadHill);
    // MR::invalidateClipping(mBank);
    // MR::invalidateClipping(mRabbit);
    _144 = 1;
    mHill->setAppearNum(mHill->_94);
}

// Dodoryu::nextState

void Dodoryu::pauseOff() {
    MR::requestMovementOn(this);
    mHill->pauseOff();
    // MR::requestMovementOn(mLeadHill);
    // MR::requestMovementOn(mBank);
    // MR::requestMovementOn(mRabbit);
    MR::requestEffectStopSceneEnd();
}

void Dodoryu::startPlayerCenterCamera() {
    if (MR::isValidSwitchB(this)) {
        MR::onSwitchB(this);
    }
}

void Dodoryu::endPlayerCenterCamera() {
    if (MR::isValidSwitchB(this)) {
        MR::offSwitchB(this);
    }
}

void Dodoryu::getPlayerResetMtx(TPos3f* pMtx, u32 param2) const {
    char buf[64];
    snprintf(buf, sizeof(buf), "マリオ再セット%d", param2 + 1);

    MR::tryFindNamePos(buf, (MtxPtr)pMtx);
}

// Dodoryu::checkWallWithVelocity

void Dodoryu::reactJumpOutCommon() const {
    MR::stopSceneForDefaultHit(8);
}

void Dodoryu::startSpinOutCamera() {
    MR::startGlobalEventCamera(::sSpinOutCamera, CameraTargetArg(this), 120);
}

void Dodoryu::endSpinOutCamera() {
    MR::endGlobalEventCamera(::sSpinOutCamera, 0, true);
}

// Dodoryu::keepOffFromClosedArea

void Dodoryu::tryRumblePad() {
    f32 playerDistance = MR::calcDistanceToPlayer(this);

    if (playerDistance < 500.0f) {
        MR::tryRumblePadWeak(this, 0);
    } else if (playerDistance < 1000.0f) {
        MR::tryRumblePadVeryWeak(this, 0);
    }
}

void Dodoryu::invalidateEyeSensor() {
    MR::invalidateHitSensor(this, ::sMapObjSensorName);
}

void Dodoryu::validateEyeSensor() {
    MR::validateHitSensor(this, ::sMapObjSensorName);
}

void Dodoryu::invalidateStarPieceSensor() {
    MR::invalidateHitSensor(this, ::sStarPieceSensorName);
}

void Dodoryu::validateStarPieceSensor() {
    MR::validateHitSensor(this, ::sStarPieceSensorName);
}

// Dodoryu::isHeadNeedle

void Dodoryu::shiftMoveStateRail(f32 param1) {
    mMoveStateHolder->shiftRail(param1);
}

void Dodoryu::shiftMoveStateChase(f32 param1, f32 param2, f32 param3, f32 param4) {
    mMoveStateHolder->shiftChase(param1, param2, param3, param4);
}

void Dodoryu::shiftMoveStateEscape(f32 param1, s32 param2, s32 param3, f32 param4, f32 param5) {
    mMoveStateHolder->shiftEscape(param1, param2, param3, param4, param5);
}

void Dodoryu::shiftMoveStateJumpOut(f32 param1, f32 param2) {
    mMoveStateHolder->shiftJumpOut(param1, param2);
}

void Dodoryu::shiftMoveStateNull() {
    mMoveStateHolder->shiftNull();
}

void Dodoryu::setMoveRailSpeed(f32 railSpeed) {
    mMoveStateHolder->setRailSpeed(railSpeed);
}

void Dodoryu::startHill() {
    mHill->startNaturally();
}

void Dodoryu::stopHill() {
    mHill->endNaturally();
}

void Dodoryu::stopHillForce() {
    mHill->end();
}

void Dodoryu::killAllHillPieces() {
    mHill->killAllPieces();
}

void Dodoryu::setHillAppearNumHalf() {
    mHill->setAppearNum(mHill->_94 / 2);
}

void Dodoryu::setHillAppearNumMax() {
    mHill->setAppearNum(mHill->_94 - 10);
}
/*
void Dodoryu::startLeadHillBck(const char* pBckName) {
    if (MR::isDead(mLeadHill)) {
        mLeadHill->appear();
    }

    MR::startBck(mLeadHill, pBckName, nullptr);
}

void Dodoryu::killLeadHill() {
    mLeadHill->kill();
}

void Dodoryu::appearBank() {
    if (MR::isDead(mBank)) {
    }
}

void Dodoryu::leaveRabbit() {
    if (MR::isDead(mRabbit)) {
        return;
    }

    setNerve(&::DodoryuRabbitNrvRabbitJump::sInstance);
}

void Dodoryu::resetRabbit() {
    mRabbit->reset(true);
}
*/
// Dodoryu::calcRabbitDir
// Dodoryu::displayRabbitMessage
// Dodoryu::calcAndSetBaseMtx

void Dodoryu::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pSender, ::sStarPieceSensorName)) {
        return;
    }

    if (MR::isSensorMapObj(pReceiver)) {
        if (MR::isSensor(pSender, ::sMapObjSensorName)) {
            MR::sendMsgEnemyAttack(pReceiver, pSender);
        }
    } else {
        mState[_CC]->catchAttackSensor(pSender, pReceiver);
    }
}

bool Dodoryu::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensor(pReceiver, ::sStarPieceSensorName)) {
        if (MR::isMsgStarPieceAttack(msg)) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_HIT_STOP");
            mAnimScaleCtrl->startHitReaction();
            mMoveStateHolder->brake();

            return true;
        }

        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return isHeadNeedle(pReceiver, pSender);
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        return true;
    }

    return mState[_CC]->catchPlayerAttack(msg, pSender, pReceiver);
}

void Dodoryu::initHitSensor() {
    LiveActor::initHitSensor(9);

    MR::addHitSensorAtJointEnemy(this, ::sHeadSensorName, ::sHeadJointName, 8, ::sHeadRadius, ::sHeadOffset);
    MR::addHitSensorAtJointEnemy(this, ::sBodySensorName, "Hip", 8, ::sBodyRadius, ::sBodyOffset);
    MR::addHitSensorAtJointEnemy(this, ::sLegLSensorName, "LegL2", 8, ::sLegRadius, ::sLegOffset);
    MR::addHitSensorAtJointEnemy(this, ::sLegRSensorName, "LegR2", 8, ::sLegRadius, ::sLegOffset);
    MR::addHitSensorAtJointEnemy(this, ::sArmLSensorName, "HandL", 8, ::sArmRadius, ::sArmOffset);
    MR::addHitSensorAtJointEnemy(this, ::sArmRSensorName, "HandR", 8, ::sArmRadius, ::sArmOffset);
    MR::addHitSensorAtJointEnemy(this, ::sTailSensorName, "Tail", 8, ::sTailRadius, ::sTailOffset);
    MR::addHitSensorEye(this, ::sMapObjSensorName, 16, ::sMapObjSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointMapObjSimple(this, ::sStarPieceSensorName, "Hip", 16, ::sStarPieceSensorRadius, ::sStarPieceSensorOffset);
}

void Dodoryu::initSwitch(const JMapInfoIter& rIter) {
    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &Dodoryu::notifyOnSwitchA));
    }

    MR::useStageSwitchWriteB(this, rIter);
}

void Dodoryu::createMogucchiHill() {
    mHill = new DodoryuHill(this);
    mHill->setAppearNum(5);
}
/*
void Dodoryu::createDodoryuBank() {
    mBank = new DodoryuBank();
    mBank->initWithoutIter();
}
*/
// Dodoryu::turnUpVecTo

void Dodoryu::checkHipDrop() {
    if (MR::isPlayerHipDropLand()) {
        mState[_CC]->catchHipDrop();
    }
}

void Dodoryu::initState() {
    mState.push_back(new DodoryuStateWait(this, "DodoryuStateWait"));
    mState.push_back(new DodoryuDemoOpening(this, "DodoryuDemoOpening"));
    // mState.push_back(new DodoryuStateLv1(this, "DodoryuStateLv1"));
    mState.push_back(new DodoryuDemoAppear(this, "DodoryuDemoAppear"));
    // mState.push_back(new DodoryuStateLv2(this, &::sDodoryuChaseDataLv2, "DodoryuStateLv2"));
    mState.push_back(new DodoryuDemoAngry(this, "DodoryuDemoAngry"));
    // mState.push_back(new DodoryuStateLv2(this, &::sDodoryuChaseDataLv3, "DodoryuStateLv3"));
    mState.push_back(new DodoryuDemoDown(this, "DodoryuDemoDown"));
    _CC = 0;
}

void Dodoryu::updateRumblePad() {
    if (!_150) {
        return;
    }

    _154++;

    MR::tryRumblePadWeak(this, 0);
}

// Dodoryu::updateCameraTarget
