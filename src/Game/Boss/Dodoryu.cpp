#include "Game/Boss/Dodoryu.hpp"
#include "Game/Boss/DodoryuDemo.hpp"
#include "Game/Boss/DodoryuHill.hpp"
#include "Game/Boss/DodoryuMove.hpp"
#include "Game/Boss/DodoryuStateBase.hpp"
#include "Game/Boss/DodoryuStateLv1.hpp"
#include "Game/Boss/DodoryuStateLv2.hpp"
#include "Game/Boss/DodoryuStateWait.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/JointRumbler.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include <cstdio>

namespace {
    static const Vec sHeadOffset = {50.0f, 30.0f, 0.0f};
    static const Vec sBodyOffset = {0.0f, 0.0f, 30.0f};
    static const Vec sLegOffset = {40.0f, 0.0f, 0.0f};
    static const Vec sArmOffset = {0.0f, 0.0f, 0.0f};
    static const Vec sTailOffset = {50.0f, 0.0f, 0.0f};
    static const Vec sStarPieceSensorOffset = {-150.0f, 0.0f, 0.0f};
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
    static const s32 sJumpOutStopSceneFrame = 8;
    // static const s32 sRumbleInterval = _;
    static const char* sSpinOutCamera = "ドドリュウ吹っ飛び";
    // static const f32 sSpinOutCameraInterpolation = _;
    // static const f32 sSpinOutCameraBlendRate = _;
    static const f32 sRumblePadNearDist = 500.0f;
    static const f32 sRumblePadFarDist = 1000.0f;
    // static const char* sPrintStateLabel = _;
    static const f32 sRabbitDistMax = 1500.0f;
    static const f32 sRabbitDistMin = 650.0f;
    static const f32 sRabbitRestDist = 2000.0f;
    static const f32 sRabbitFastSpeed = 8.0f;
    static const f32 sRabbitSlowSpeed = 4.0f;
    // static const f32 sTalkDistanceLong = _;
    // static const f32 sTalkDistanceShort = _;
    // static const TVec3f sRotateVel = _;
    static DodoryuChaseParam sDodoryuChaseDataLv2 = {
        60,
        16.0f,
        0.1f,
        1.5f,
        0.0f,
        600.0f,
        30,
        9.0f,
        0.1f,
        1.0f,
        0.0f,
        45,
        55,
        800.0f,
        180.0f,
        5.0f,
        0.2f,
        2.0f,
        0.0f,
        120,
        150,
        5,
        250,
        1500.0f,
        50,
        70,
        8.0f,
        50.0f,
        1.5f,
        60,
        120,
        60,
        90,
        1200.0f,
        1500.0f,
        5.0f,
        80,
        -1,
        51,
        false,
        10.0f,
        1.0f,
        3.0f,
        0.92f,
        360,
        0.01f,
        7.0f,
        50,
        60,
        10,
        20,
        {"Burrow", nullptr, "Normal", "Normal", nullptr},
        {"UnderGroundWalk", nullptr, "Normal", "Normal", nullptr},
        {"UnderGroundWalk", nullptr, nullptr, "Normal", nullptr},
        {"EffectWalkAppear", nullptr, "Normal", nullptr, "EffectWalkAppear"},
        {"EffectWalk", nullptr, nullptr, nullptr, "EffectWalk"},
        {"EffectWalkHide", nullptr, nullptr, nullptr, "EffectWalkHide"},
        {"Appear", nullptr, nullptr, "Careless", "Appear"},
        {"Walk", nullptr, nullptr, "Normal", "Walk"},
        {"Hide", nullptr, nullptr, nullptr, "Hide"},
        {"SwoonWalkStart", nullptr, nullptr, "Cry", nullptr},
        {"SwoonWalk", nullptr, nullptr, nullptr, nullptr},
        {"SwoonWalkEnd", nullptr, nullptr, nullptr, nullptr},
        {"Dive", nullptr, nullptr, "Normal", nullptr},
        {"AngryPunchDamage", nullptr, nullptr, nullptr, nullptr},
        {"AttackHit", nullptr, nullptr, nullptr, "AttackHit"},
        {"Hide", nullptr, nullptr, nullptr, "Hide"},
        {"Jump", nullptr, nullptr, "Careless", "Jump"},
        {},
        {},
        {},
    };
    static DodoryuChaseParam sDodoryuChaseDataLv3 = {
        60,
        20.0f,
        0.2f,
        1.0f,
        0.0f,
        500.0f,
        30,
        9.0f,
        0.1f,
        1.0f,
        0.0f,
        55,
        65,
        750.0f,
        180.0f,
        6.0f,
        0.2f,
        2.0f,
        0.99f,
        60,
        75,
        5,
        140,
        1500.0f,
        40,
        60,
        9.0f,
        70.0f,
        2.0f,
        60,
        120,
        60,
        90,
        1500.0f,
        1600.0f,
        5.0f,
        -1,
        45,
        -1,
        true,
        18.0f,
        0.5f,
        0.1f,
        0.96f,
        240,
        0.1f,
        7.0f,
        50,
        60,
        10,
        20,
        {"Burrow", nullptr, "Angry", "Angry", nullptr},
        {"UnderGroundWalk", nullptr, "Angry", "Angry", nullptr},
        {"UnderGroundWalk", nullptr, "Angry", "Angry", nullptr},
        {"AngryEffectWalkAppear", nullptr, nullptr, nullptr, "EffectWalkAppear"},
        {"AngryEffectWalk", nullptr, nullptr, nullptr, "EffectWalk"},
        {"AngryEffectWalkHide", nullptr, nullptr, nullptr, "EffectWalkHide"},
        {"AngryAppear", nullptr, nullptr, nullptr, "Appear"},
        {"AngryWalk", nullptr, nullptr, nullptr, "Walk"},
        {"AngryHide", nullptr, nullptr, nullptr, "Hide"},
        {"SwoonWalkStart", nullptr, "CoolDown", "Cry", nullptr},
        {"SwoonWalk", nullptr, "Normal", nullptr, nullptr},
        {"SwoonWalkEnd", nullptr, nullptr, nullptr, nullptr},
        {"AngryDive", nullptr, "AngryStart", "Angry", nullptr},
        {"AngryPunchDamage", nullptr, nullptr, nullptr, nullptr},
        {"AngryAttackHit", nullptr, nullptr, nullptr, "AttackHit"},
        {"AngryHide", nullptr, nullptr, nullptr, "Hide"},
        {"AngryJump", nullptr, nullptr, "Cry", "Jump"},
        {"AngryChaseStart", nullptr, nullptr, "Cry", "AngryChaseStart"},
        {"AngryChase", nullptr, nullptr, "Angry", "AngryChase"},
        {"AngryChaseEnd", nullptr, nullptr, nullptr, "AngryChaseEnd"},
    };
};  // namespace

namespace {
    NEW_NERVE(DodoryuBankNrvBankAppear, DodoryuBank, Appear);
};  // namespace

namespace {
    NEW_NERVE(DodoryuRabbitNrvRabbitEscapeWaiting, DodoryuRabbit, EscapeWaiting);
    NEW_NERVE(DodoryuRabbitNrvRabbitEscape, DodoryuRabbit, Escape);
    NEW_NERVE(DodoryuRabbitNrvRabbitEscapeSlow, DodoryuRabbit, EscapeSlow);
    NEW_NERVE(DodoryuRabbitNrvRabbitRest, DodoryuRabbit, Rest);
    NEW_NERVE(DodoryuRabbitNrvRabbitJump, DodoryuRabbit, Jump);
    NEW_NERVE(DodoryuRabbitNrvRabbitWait, DodoryuRabbit, Wait);
    NEW_NERVE(DodoryuRabbitNrvRabbitReturn, DodoryuRabbit, Return);
    NEW_NERVE(DodoryuRabbitNrvRabbitPleasure, DodoryuRabbit, Pleasure);
};  // namespace

Dodoryu::Dodoryu(const char* pName)
    : LiveActor(pName), _BC(0.0f, 0.0f, 0.0f), _C8(), _CC(), mState(), mMoveStateHolder(), mHill(), mLeadHill(), mBank(), mRabbit(),
      _128(0.0f, 1.0f, 0.0f), _134(1.0f, 0.0f, 0.0f), mClosedAreaObj(), _144(), _148(new CameraTargetMtx("カメラターゲットダミー")),
      _14C(new CameraTargetMtx("カメラターゲットダミー")), _150(), _154(), mAnimScaleCtrl(new AnimScaleController(nullptr)) {
    mBaseMtx.identity();
}

void Dodoryu::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    mScale.set(1.0f, 1.0f, 1.0f);

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
    mLeadHill = new DodoryuLeadHill(this);
    createDodoryuBank();
    mRabbit = new DodoryuRabbit(this, rIter);
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
    mState[_CC]->movement();
    updateRumblePad();
    updateCameraTarget();
    mAnimScaleCtrl->updateNerve();
}

void Dodoryu::startClipped() {
    LiveActor::startClipped();
    MR::validateClipping(mRabbit);
    mState[_CC]->catchStartClipped();
}

void Dodoryu::endClipped() {
    LiveActor::endClipped();
    MR::invalidateClipping(mRabbit);
    mState[_CC]->catchEndClipped();
}

void Dodoryu::snapToGround() {
    MR::calcGravityVector(this, &mGravity, nullptr, 0);

    Triangle triangle;
    TVec3f v4;
    TVec3f v3 = mGravity.scaleInline(200.0f);
    TVec3f v2 = mGravity.scaleInline(1200.0f);
    TVec3f v1 = mPosition - v2;

    if (MR::getFirstPolyOnLineToMap(&v4, &triangle, v3, v1)) {
        _128 = *triangle.getNormal(0);

        turnUpVecTo(-mGravity);
        mPosition.set(v4);
        mBaseMtx.setTrans(v4);
    }
}

bool Dodoryu::snapToWall() {
    TVec3f cylinderPos;
    MR::calcCylinderPos(&cylinderPos, mClosedAreaObj);

    TVec3f cylinderUpVec;
    MR::calcCylinderUpVec(&cylinderUpVec, mClosedAreaObj);

    f32 cylinderRadius = MR::getCylinderRadius(mClosedAreaObj);

    TVec3f v1 = mPosition - cylinderPos;
    v1.scaleAdd(v1, cylinderUpVec, cylinderUpVec.dot(v1));

    if (MR::isNearZero(v1)) {
        return false;
    }

    MR::normalize(&v1);
    _134.set(v1);

    TVec3f v2 = cylinderPos + v1.scaleInline(cylinderRadius);
    mPosition.set(v2);
    mBaseMtx.setTrans(v2);

    return true;
}

void Dodoryu::setMtx(const TPos3f& rMtx) {
    mBaseMtx.setInline(&rMtx);
    mBaseMtx.getTrans(mPosition);

    if (_C8) {
        _BC = *MR::getPlayerPos();
    } else {
        _BC = mPosition - mGravity.scaleInline(150.0f);
    }
}

void Dodoryu::notifyOnSwitchA() {
    MR::invalidateClipping(this);
    MR::invalidateClipping(mHill);
    MR::invalidateClipping(mLeadHill);
    MR::invalidateClipping(mBank);
    MR::invalidateClipping(mRabbit);
    _144 = 1;
    mHill->setAppearNum(mHill->_94);
}

void Dodoryu::nextState() {
    mState[_CC]->end();

    _CC++;

    if (_CC >= mState.size()) {
        MR::startAfterBossBGM();
        MR::requestAppearPowerStar(this, mPosition);
        mRabbit->setNerve(&::DodoryuRabbitNrvRabbitReturn::sInstance);

        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }

        MR::setImageEffectControlAuto();
        kill();
    } else {
        MR::turnOnDepthOfField(true);
        MR::setDepthOfFieldBlurIntensity(0.75f);
        mState[_CC]->start();
    }
}

void Dodoryu::pauseOff() {
    MR::requestMovementOn(this);
    mHill->pauseOff();
    MR::requestMovementOn(mLeadHill);
    MR::requestMovementOn(mBank);
    MR::requestMovementOn(mRabbit);
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

bool Dodoryu::checkWallWithVelocity() {
    mClosedAreaObj = nullptr;

    AreaObj* closedAreaObj = MR::getAreaObj("DodoryuClosedCylinder", mPosition);

    if (closedAreaObj == nullptr) {
        return false;
    }

    if (MR::isNearZero(mVelocity)) {
        return false;
    }

    TVec3f cylinderPos;
    MR::calcCylinderPos(&cylinderPos, closedAreaObj);

    TVec3f cylinderUpVec;
    MR::calcCylinderUpVec(&cylinderUpVec, closedAreaObj);

    TVec3f deltaPos = mPosition - cylinderPos;
    deltaPos.scaleAdd(cylinderUpVec, deltaPos, -cylinderUpVec.dot(deltaPos));

    if (MR::isNearZero(deltaPos)) {
        return false;
    }

    if (deltaPos.dot(mVelocity) >= 0.0f) {
        return false;
    }

    MR::normalize(&deltaPos);

    _134 = deltaPos;
    mClosedAreaObj = closedAreaObj;

    return true;
}

void Dodoryu::reactJumpOutCommon() const {
    MR::stopSceneForDefaultHit(::sJumpOutStopSceneFrame);
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

    if (playerDistance < ::sRumblePadNearDist) {
        MR::tryRumblePadWeak(this, 0);
    } else if (playerDistance < ::sRumblePadFarDist) {
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

bool Dodoryu::isHeadNeedle(HitSensor* pReceiver, HitSensor* pSender) const {
    HitSensor* pSensor = getSensor(::sHeadSensorName);

    if (pReceiver != pSensor) {
        return false;
    }

    TVec3f deltaPos = pSender->mPosition - pReceiver->mPosition;

    TVec3f xDir;
    TRot3f jointMtx;
    jointMtx.setInline(MR::getJointMtx(this, ::sHeadJointName));
    jointMtx.getXDir(xDir);

    return xDir.dot(deltaPos) >= 105.0f;
}

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
    DodoryuRabbit* rabbit = mRabbit;

    if (MR::isDead(rabbit)) {
        return;
    }

    rabbit->setNerve(&::DodoryuRabbitNrvRabbitJump::sInstance);
}

void Dodoryu::resetRabbit() {
    mRabbit->reset(true);
}

void Dodoryu::calcRabbitDir(TVec3f* pDir) const {
    pDir->set(mRabbit->mPosition - mPosition);
    pDir->scaleAdd(*pDir, mGravity, -mGravity.dot(*pDir));
    MR::normalizeOrZero(pDir);
}

void Dodoryu::displayRabbitMessage() {
    mRabbit->mIsDisplayMessage = true;
}

void Dodoryu::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBaseMtx);
    MR::setBaseScale(this, mAnimScaleCtrl->_C.mult(mScale));
}

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

void Dodoryu::createDodoryuBank() {
    mBank = new DodoryuBank();
    mBank->initWithoutIter();
}

// Dodoryu::turnUpVecTo

void Dodoryu::checkHipDrop() {
    if (MR::isPlayerHipDropLand()) {
        mState[_CC]->catchHipDrop();
    }
}

void Dodoryu::initState() {
    mState.push_back(new DodoryuStateWait(this, "DodoryuStateWait"));
    mState.push_back(new DodoryuDemoOpening(this, "DodoryuDemoOpening"));
    mState.push_back(new DodoryuStateLv1(this, "DodoryuStateLv1"));
    mState.push_back(new DodoryuDemoAppear(this, "DodoryuDemoAppear"));
    mState.push_back(new DodoryuStateLv2(this, &::sDodoryuChaseDataLv2, "DodoryuStateLv2"));
    mState.push_back(new DodoryuDemoAngry(this, "DodoryuDemoAngry"));
    mState.push_back(new DodoryuStateLv2(this, &::sDodoryuChaseDataLv3, "DodoryuStateLv3"));
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

void Dodoryu::updateCameraTarget() {
    TPos3f m1;
    m1.setInline(MR::getJointMtx(this, "Hip"));

    TVec3f v1;
    m1.getTrans(v1);

    TVec3f v4 = v1.scaleInline(0.7f) + MR::getPlayerPos()->scaleInline(0.3f);

    v1 = v4;
    m1.setTrans(v1);
    _14C->mMatrix.setInline(m1);
}

DodoryuBank::DodoryuBank() : ModelObj("ドドリュウ盛土", "DodoryuBank", _90, MR::DrawBufferType_MapObjStrongLight, -2, -2, false) {
    _90.identity();
}

void DodoryuBank::init(const JMapInfoIter& rIter) {
    initNerve(&::DodoryuBankNrvBankAppear::sInstance);
    makeActorDead();
}

void DodoryuBank::exeAppear() {
    if (MR::isBckStopped(this)) {
        kill();
        MR::validateClipping(this);
    }
}

DodoryuRabbit::DodoryuRabbit(Dodoryu* pHost, const JMapInfoIter& rIter)
    : ModelObj("ドドリュウに追われるウサギ", "DodoryuRabbit", _94, MR::DrawBufferType_NPC, -2, -2, false), mHost(pHost), _C4(), mTalkCtrl(), _CC(300),
      _D0(), _D4(), _D8(), mIsDisplayMessage() {
    _94.identity();

    initWithoutIter();

    _E0.identity();
    _E0.setTrans(mPosition);

    mTalkCtrl = MR::createTalkCtrl(this, rIter, "DodoryuRabbit", TVec3f(100.0f, 150.0f, 100.0f), _E0);
    MR::setDistanceToTalk(mTalkCtrl, 1500.0f);
}

void DodoryuRabbit::init(const JMapInfoIter& rIter) {
    initNerve(&::DodoryuRabbitNrvRabbitEscape::sInstance);
    initHitSensor(4);
    MR::addHitSensorAtJointEnemy(this, "body", "Spine", 8, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initShadowFromCSV(this, "Shadow");

    _D4 = new JointRumbler(this, "Spine", 4.0f, 0.2f, 60, true, 0);
    _D8 = new JointRumbler(this, "Spine", 4.0f, 0.2f, 30, true, 0);

    MR::initLightCtrl(this);
    reset(false);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void DodoryuRabbit::control() {
    bool isEscape = isNerve(&::DodoryuRabbitNrvRabbitEscapeWaiting::sInstance) || isNerve(&::DodoryuRabbitNrvRabbitEscape::sInstance) ||
                    isNerve(&::DodoryuRabbitNrvRabbitEscapeSlow::sInstance);

    if (isEscape) {
        _CC--;

        if (_CC <= 0) {
            _CC = 300;
            _D0 = (_D0 + 1) % 2;

            MR::resetAndForwardNode(mTalkCtrl, _D0);
        }
    }

    if (_D4->isRumbling()) {
        _D4->update();
    }

    if (_D8->isRumbling()) {
        _D8->update();
    }

    if (isNerve(&::DodoryuRabbitNrvRabbitPleasure::sInstance) && getNerveStep() > 30) {
        return;
    }

    _E0.setInline(MR::getJointMtx(this, "Spine"));

    if (MR::isShadowProjected(this, nullptr)) {
        TVec3f shadowPos;
        MR::getShadowProjectionPos(this, nullptr, &shadowPos);
        _E0.setTrans(shadowPos);
    }
}

void DodoryuRabbit::exeEscapeWaiting() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Run", nullptr);
        MR::startBtp(this, "Blink");
    }

    tryTalk();
}

void DodoryuRabbit::exeEscape() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Run", nullptr);
        MR::startBtp(this, "Blink");
    }

    if (MR::checkPassBckFrame(this, 3.0f)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }

    updatePos(::sRabbitFastSpeed);
    tryTalk();

    if (calcCoordDiff() > ::sRabbitDistMax) {
        setNerve(&::DodoryuRabbitNrvRabbitEscapeSlow::sInstance);
    }
}

void DodoryuRabbit::exeEscapeSlow() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Walk", nullptr);
        MR::startBtp(this, "blink");
    }

    if (MR::checkPassBckFrame(this, 3.0f)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }

    updatePos(::sRabbitSlowSpeed);
    tryTalk();

    f32 coordDiff = calcCoordDiff();

    if (coordDiff < ::sRabbitDistMin) {
        setNerve(&::DodoryuRabbitNrvRabbitEscape::sInstance);
    } else if (coordDiff > ::sRabbitRestDist) {
        setNerve(&::DodoryuRabbitNrvRabbitRest::sInstance);
    }
}

void DodoryuRabbit::exeRest() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Tired", nullptr);
        MR::startBtp(this, "Tired");
    }

    tryTalk();

    if (calcCoordDiff() < ::sRabbitDistMin) {
        setNerve(&::DodoryuRabbitNrvRabbitEscape::sInstance);
    }
}

void DodoryuRabbit::exeJump() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Jump", nullptr);
        MR::startBtp(this, "Blink");
        _94.setInline(mHost->_148->mMatrix);
    }

    MR::setNerveAtBckStopped(this, &::DodoryuRabbitNrvRabbitWait::sInstance);
}

void DodoryuRabbit::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::startBtp(this, "Blink");
    }
}

void DodoryuRabbit::exeReturn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "StarJump", nullptr);
        MR::startBtp(this, "blink");
    }

    MR::setNerveAtBckStopped(this, &::DodoryuRabbitNrvRabbitPleasure::sInstance);
}

void DodoryuRabbit::exePleasure() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "StarWait", nullptr);
        MR::startBtp(this, "blink");
        MR::resetAndForwardNode(mTalkCtrl, 2);
        MR::setDistanceToTalk(mTalkCtrl, 350.0f);
    }

    tryTalk();
}

void DodoryuRabbit::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayerOrRide(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    } else if (MR::isSensorMapObj(pReceiver)) {
        MR::sendMsgEnemyAttack(pReceiver, pSender);
    }
}

bool DodoryuRabbit::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg)) {
        if (_D8->isRumbling()) {
            _D8->reset();
        }

        _D8->start();

        return true;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        if (_D4->isRumbling()) {
            _D4->reset();
        }

        _D4->start();

        return false;
    }

    return false;
}

void DodoryuRabbit::reset(bool param1) {
    _C4 = MR::getRailCoord(mHost) + 1075.0f;

    setNerve(&::DodoryuRabbitNrvRabbitEscape::sInstance);

    if (param1) {
        updatePos(0.0f);
    }
}

// DodoryuRabbit::updatePos

f32 DodoryuRabbit::calcCoordDiff() const {
    f32 coord = MR::getRailCoord(mHost);

    if (_C4 < coord) {
        return (_C4 + MR::getRailTotalLength(mHost)) - coord;
    }

    return _C4 - coord;
}

bool DodoryuRabbit::tryTalk() {
    if (mIsDisplayMessage) {
        return MR::tryTalkNearPlayer(mTalkCtrl);
    }

    return false;
}

DodoryuLeadHill::DodoryuLeadHill(Dodoryu* pHost) : LiveActor("ドドリュウ塚先頭"), mHostBaseMtx(getBaseMtx()), _90() {
    for (int i = 0; i < ARRAY_SIZE(_94); i++) {
        _94[i] = nullptr;
    }

    initWithoutIter();
}

void DodoryuLeadHill::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("DodoryuLeadHill", nullptr, false);
    MR::connectToSceneMapObjDecorationStrongLight(this);
    MR::initLightCtrl(this);
    initSound(8, false);
    initEffectKeeper(8, nullptr, false);
    mPosition.setTrans(mHostBaseMtx);
    initJoint();
    makeActorAppeared();
}

void DodoryuLeadHill::control() {
    _90 += 0.07f;
}

// DodoryuLeadHill::calcJoint

void DodoryuLeadHill::calcAndSetBaseMtx() {
    for (int i = 0; i < ARRAY_SIZE(_94); i++) {
        _94[i]->registerCallBack();
    }

    if (mHostBaseMtx != nullptr) {
        mPosition.setTrans(mHostBaseMtx);
        MR::setBaseTRMtx(this, mHostBaseMtx);
    } else {
        LiveActor::calcAndSetBaseMtx();
    }
}

void DodoryuLeadHill::initJoint() {
    for (int i = 0; i < ARRAY_SIZE(_94); i++) {
        char buf[64];
        snprintf(buf, sizeof(buf), "DodoryuLeadHill%d", i + 1);

        JointControlDelegator< DodoryuLeadHill >* pJointDelegator =
            new JointControlDelegator< DodoryuLeadHill >(this, &DodoryuLeadHill::calcJoint, nullptr);

        MR::setJointControllerParam(pJointDelegator, this, buf);

        _94[i] = pJointDelegator;
    }
}
