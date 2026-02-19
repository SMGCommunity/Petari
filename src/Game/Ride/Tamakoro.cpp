#include "Game/Ride/Tamakoro.hpp"
#include "Game/GameAudio/AudTamakoroBgmCtrl.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Ride/SphereAccelSensorController.hpp"
#include "Game/Ride/TamakoroTutorial.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/mtx.h>
#include <revolution/wpad.h>

namespace {
    static const f32 mBaseRadius = 150.0f;
    static const f32 sBindableUpperDegree = 60.0f;
    static const s32 sBindStartTime = 40;
    static const f32 sBindStartJumpHeight = 150.0f;
    static const f32 sBindStartUpAdjustRate = 0.1f;
    static const f32 sBindStartFrontAdjustRate = 0.1f;
    static const s32 sBindStartLandTime = 14;
    static const f32 sLandStartPositionDegree = 35.0f;
    static const f32 sLandStartVelocityDegree = 40.0f;
    static const f32 sGravityAccel = 2.0f;
    static const f32 sGroundFric = 0.99f;
    static const f32 sIdleBckRate = 1.0f;
    static const f32 sSoftWalkSpeed = 0.25f;
    static const f32 sSoftWalkBckRate = 1.0f;
    static const f32 sWalkSpeed = 6.0f;
    static const f32 sWalkBckRate = 2.0f;
    static const f32 sRunSpeed = 18.0f;
    static const f32 sRunBckRate = 3.0f;
    static const f32 sRunMaxBckRate = 4.0f;
    static const s32 sFallStartTime = 5;
    static const f32 sFallOffsetSpeed = 3.0f;
    static const f32 sJumpPowerV = 40.0f;
    static const f32 sJumpPowerH = 7.0f;
    static const f32 sJumpOffsetSpeed = 4.0f;
    static const f32 sMarioJumpOffsetMax = 150.0f;
    static const f32 sMarioOffsetGravityAccel = 1.0f;
    static const f32 sMarioOffsetFric = 0.99f;
    static const f32 sNeedLandPower = 20.0f;
    static const s32 sLandTime = 2;
    static const f32 sLandFric = 0.88f;
    static const f32 sNeedBumpWallPower = 10.0f;
    static const f32 sCollisionStrongPower = 60.0f;
    static const f32 sCollisionMiddlePower = 25.0f;
    static const f32 sCollisionWeakPower = 5.0f;
    // static const f32 sStampReactionH =
    static const f32 sStampReactionV = 30.0f;
    static const f32 sStampReactionInvalidSpeed = 15.0f;
    static const f32 sExplosionReactionH = 20.0f;
    static const f32 sExplosionReactionV = 40.0f;
    static const f32 sRaidRailFastSpeed = 30.0f;
    static const f32 sRollBckRate = 1.5f;
    // static const f32 sRollMaxBckRate = ;
    static const s32 sDashRailTime = 40;
    static const f32 sDashRailFric = 0.99f;
    static const f32 sDashBallRollSpeed = 60.0f;
    static const f32 sEndBindFrontPower = 5.0f;
    static const f32 sEndBindJumpPower = 35.0f;
    static const s32 sForceBindEndTime = 90;
    static const f32 sMarioUpAdjustRate = 0.1f;
    static const f32 sMarioFrontAdjustRate = 0.25f;
    // static const f32 sStandDirMaxAccelDegree = ;
    static const f32 sStandDirInterRate = 0.02f;
    // static const ___ sMarioAccelDir = ; // ????
    static const f32 sMarioUpperYAccel = 5.0f;
    static const f32 sMarioUpperYFric = 0.9f;
    static const f32 sRingMaxDegree = 45.0f;
    static const f32 sRingMaxAngleSpeed = 4.0f;
    static const f32 sTutorialAccel = 0.5f;
    static const f32 sTutorialFric = 0.95f;
    static const s32 sBgmStateChangeFrames = 30;
}  // namespace

namespace NrvTamakoro {
    NEW_NERVE(TamakoroNrvStandByTutorial, Tamakoro, StandByTutorial);
    NEW_NERVE(TamakoroNrvStandByBind, Tamakoro, StandByBind);
    NEW_NERVE(TamakoroNrvBindStart, Tamakoro, BindStart);
    NEW_NERVE(TamakoroNrvBindStartLand, Tamakoro, BindStartLand);
    NEW_NERVE(TamakoroNrvTutorial, Tamakoro, Tutorial);
    NEW_NERVE(TamakoroNrvWait, Tamakoro, Wait);
    NEW_NERVE(TamakoroNrvFall, Tamakoro, Fall);
    NEW_NERVE(TamakoroNrvJump, Tamakoro, Jump);
    NEW_NERVE(TamakoroNrvBumpWall, Tamakoro, BumpWall);
    NEW_NERVE(TamakoroNrvLand, Tamakoro, Land);
    NEW_NERVE(TamakoroNrvJumpHole, Tamakoro, JumpHole);
    NEW_NERVE(TamakoroNrvJumpHoleSetUp, Tamakoro, JumpHoleSetUp);
    NEW_NERVE_ONEND(TamakoroNrvJumpHoleLaunch, Tamakoro, JumpHoleLaunch, JumpHoleLaunch);
    NEW_NERVE(TamakoroNrvDashRail, Tamakoro, DashRail);
    NEW_NERVE(TamakoroNrvDashRailEnd, Tamakoro, DashRailEnd);
    NEW_NERVE(TamakoroNrvRideRail, Tamakoro, RideRail);
    NEW_NERVE(TamakoroNrvRideRailFastStart, Tamakoro, RideRailFastStart);
    NEW_NERVE(TamakoroNrvRideRailFast, Tamakoro, RideRailFast);
    NEW_NERVE(TamakoroNrvRideRailFastEnd, Tamakoro, RideRailFastEnd);
    NEW_NERVE(TamakoroNrvBindEnd, Tamakoro, BindEnd);
}  // namespace NrvTamakoro

Tamakoro::Tamakoro(const char* pName)
    : LiveActor(pName), mAccelSensorCtrl(nullptr), mTutorial(nullptr), mJointCtrl(nullptr), mBallRotateQuat(0, 0, 0, 1), mMarioRotateQuat(0, 0, 0, 1),
      mRingUp(0, 1, 0), mKickVel(0, 0, 0), mMarioBindRequestPos(0, 0, 0), mMarioUp(0, 1, 0), mMarioPos(0, 0, 0), mMoment(0, 0, 0),
      mMarioFront(0, 0, 1), mDirectionToMario(0, 0, 0), mAccelDir(0, 0, 0), mAirTime(0), mAccelRate(0.0f), mMarioRotateYAngle(0.0f),
      mMarioOffset(0.0f), mMarioOffsetVelocity(0.0f), mBgmCtrl(nullptr), mControlDisabled(false), mHasTutorial(false) {
    mAccelSensorCtrl = new SphereAccelSensorController();
    mBgmCtrl = new AudTamakoroBgmCtrl();
}

void Tamakoro::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Tamakoro", nullptr, false);
    MR::connectToScene(this, MR::MovementType_Ride, MR::CalcAnimType_Ride, MR::DrawBufferType_IndirectMapObjStrongLight, -1);
    MR::makeQuatFromRotate(&mBallRotateQuat, this);
    MR::initShadowVolumeSphere(this, mBaseRadius);
    initEffectKeeper(0, 0, false);
    initSensor();
    initSound(4, false);
    initJointControl();
    MR::onCalcGravity(this);
    initBinder(mBaseRadius, 0.0f, 16);
    initNerve(&NrvTamakoro::TamakoroNrvStandByBind::sInstance);

    MR::declarePowerStar(this);
    bool hasPowerStar = MR::hasPowerStarInCurrentStageWithDeclarer(mName, -1);
    MR::startBva(this, "Tamakoro");
    MR::setBvaFrameAndStop(this, static_cast< s32 >(hasPowerStar));
    MR::useStageSwitchWriteA(this, rIter);

    if (mHasTutorial) {
        setNerve(&NrvTamakoro::TamakoroNrvStandByTutorial::sInstance);
        mTutorial = new TamakoroTutorial("タマコロチュートリアル");
        mTutorial->mHost = this;
        MR::initChildObj(mTutorial, rIter, 0);
    }
    makeActorAppeared();
}

void Tamakoro::initSensor() {
    initHitSensor(3);
    MR::addHitSensor(this, "Body", ATYPE_SPHERE_PLAYER, 8, mBaseRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "Hit", ATYPE_SPHERE_PLAYER_HIT, 8, mBaseRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "Bind", ATYPE_SPHERE_PLAYER_BIND, 8, mBaseRadius + 15.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

void Tamakoro::initJointControl() {
    mJointCtrl = MR::createJointDelegatorWithNullChildFunc(this, &Tamakoro::ringMtxCallBack, "Ring");
}

bool Tamakoro::ringMtxCallBack(TPos3f* pMtx, const JointControllerInfo& pJointCtrlInfo) {
    TVec3f trans;
    pMtx->getTransInline(trans);

    if (MR::isNearZero(mAccelDir) || MR::isSameDirection(mAccelDir, mRingUp, 0.01f)) {
        MR::makeMtxUpNoSupport(pMtx, mRingUp);
    } else {
        MR::makeMtxUpFront(pMtx, mRingUp, mAccelDir);
    }

    pMtx->setTrans(trans);
    return true;
}

void Tamakoro::control() {
    updateMoment();
    MR::rotateQuatMoment(&mBallRotateQuat, mMoment);
    updateAirTime();
    updateRingUpVec();
    f32 speed = mVelocity.length();
    if (isNerve(&NrvTamakoro::TamakoroNrvJumpHole::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvJumpHoleSetUp::sInstance)) {
        speed = 0.0f;
    }
    mBgmCtrl->control(speed, mControlDisabled, sBgmStateChangeFrames);
    mControlDisabled = false;
}

void Tamakoro::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBallRotateQuat);
    mJointCtrl->registerCallBack();
}

void Tamakoro::updateBindActorMatrix() {
    if (isUseMarioOffset()) {
        JMAVECScaleAdd(&mDirectionToMario, &mPosition, &mMarioPos, mBaseRadius);
        if (mMarioOffset > 0.0f) {
            JMAVECScaleAdd(&mGravity, &mMarioPos, &mMarioPos, -mMarioOffset);
        }
    }

    // quaternion rotation
    TPos3f mtx;
    mtx.setQT(mMarioRotateQuat, mMarioPos);
    MR::setPlayerBaseMtx(mtx);
}

void Tamakoro::updateRingUpVec() {
    TVec3f up = -mGravity;

    if (!MR::isNearZero(mAccelDir)) {
        TVec3f v1;
        TVec3f up2 = -mGravity;
        JMAVECScaleAdd(&mAccelDir, &up2, &v1, mAccelRate);
        MR::normalizeOrZero(&v1);
        MR::turnVecToVecDegree(&up, up, v1, sRingMaxDegree, TVec3f(0, 1, 0));
    }

    MR::turnVecToVecDegree(&mRingUp, mRingUp, up, sRingMaxAngleSpeed, TVec3f(0, 1, 0));
}

void Tamakoro::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender->isType(ATYPE_SPHERE_PLAYER) && MR::tryGetItem(pSender, pReceiver)) {
        return;
    }

    if ((MR::isSensorPlayer(pReceiver) || MR::isSensorEnemy(pReceiver)) && isEnablePushPlayer()) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (isEnableEnemyAttack() && MR::isSensorEnemy(pReceiver) && pSender->isType(ATYPE_SPHERE_PLAYER)) {
        if (MR::sendMsgPlayerTrample(pReceiver, pSender)) {
            f32 downVel = mVelocity.dot(mGravity);
            if (downVel < sStampReactionInvalidSpeed) {
                downVel = -sStampReactionV - downVel;
                mVelocity.add(mGravity.multiplyOperatorInline(downVel));
                MR::tryRumblePadWeak(this, WPAD_CHAN0);
                MR::shakeCameraNormalWeak();
            }
            return;
        }
    }

    if (isEnableBallBind() && pSender == getSensor("Body")) {
        if (isBindedSphereDash(pSender, pReceiver)) {
            setNerve(&NrvTamakoro::TamakoroNrvDashRail::sInstance);
            return;
        }

        if (isBindedJumpHole(pSender, pReceiver)) {
            setNerve(&NrvTamakoro::TamakoroNrvJumpHole::sInstance);
            return;
        }

        if (isBindedBallRail(pSender, pReceiver)) {
            setNerve(&NrvTamakoro::TamakoroNrvRideRail::sInstance);
            return;
        }
    }
}

bool Tamakoro::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }
    return false;
}

bool Tamakoro::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg) && isEnableEnemyAttack()) {
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::setVelocitySeparateHV(this, pSender, pReceiver, sExplosionReactionH, sExplosionReactionV);
        MR::startSoundPlayer("SE_PV_UPSET", -1);
        return true;
    }
    return false;
}

bool Tamakoro::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg) && requestBind(pSender)) {
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        updateBindActorMatrix();
        return true;
    }

    if (msg == ACTMES_RUSH_FORCE_CANCEL) {
        setNerve(&NrvTamakoro::TamakoroNrvStandByBind::sInstance);
        return true;
    }

    if (msg == ACTMES_SET_UP_JUMP_HOLE && isNerve(&NrvTamakoro::TamakoroNrvJumpHole::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvJumpHoleSetUp::sInstance);
        return true;
    }

    if (msg == ACTMES_SHOOT_JUMP_HOLE && isNerve(&NrvTamakoro::TamakoroNrvJumpHoleSetUp::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvJumpHoleLaunch::sInstance);
        return true;
    }

    if (msg == ACTMES_END_JUMP_HOLE && isNerve(&NrvTamakoro::TamakoroNrvJumpHoleLaunch::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvFall::sInstance);
        return true;
    }

    if (msg == ACTMES_END_RAIL_DASH && isNerve(&NrvTamakoro::TamakoroNrvDashRail::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvDashRailEnd::sInstance);
        return true;
    }

    if (msg == ACTMES_END_BALL_RAIL && isRideRail()) {
        setNerve(&NrvTamakoro::TamakoroNrvWait::sInstance);
        return true;
    }

    if (msg == ACTMES_END_BALL_BIND && requestEndBind()) {
        return true;
    }

    return false;
}

bool Tamakoro::requestBind(HitSensor* pSensor) {
    if (!isNerve(&NrvTamakoro::TamakoroNrvStandByBind::sInstance)) {
        return false;
    }

    // check if sensor within vertical angle range
    TVec3f vThisToSensor = MR::getSensorPos(pSensor) - mPosition;
    MR::normalizeOrZero(&vThisToSensor);
    f32 cosPositionDegree = vThisToSensor.dot(-mGravity);
    if (cosPositionDegree < MR::cosDegree(sBindableUpperDegree)) {
        return false;
    }

    MR::invalidateClipping(this);

    MR::makeQuatRotateDegree(&mMarioRotateQuat, *MR::getPlayerRotate());
    mMarioPos.set(*MR::getPlayerPos());
    mMarioRotateQuat.getZDir(mMarioFront);
    mMarioFront.rejection(mGravity);
    if (MR::normalizeOrZero(&mMarioFront)) {
        MR::makeAxisVerticalZX(&mMarioFront, mGravity);
    }

    mMarioBindRequestPos.set(mMarioPos);
    mMarioRotateYAngle = 0.0f;
    mAccelRate = 0.0f;
    mMarioOffset = 0.0f;
    mMarioOffsetVelocity = 0.0f;

    mAccelDir.zero();
    MR::zeroVelocity(this);
    MR::invalidateHitSensor(this, "Bind");

    TVec3f vel(*MR::getPlayerVelocity());
    MR::normalizeOrZero(&vel);
    f32 cosVelocityDegree = -vThisToSensor.dot(vel);

    if (cosVelocityDegree > MR::cosDegree(sLandStartVelocityDegree) && cosPositionDegree > MR::cosDegree(sLandStartPositionDegree)) {
        // if angle between vel and "to this" < sLandStartVelocityDegree (40.0f) degrees and vertical angle to sensor < sLandStartPositionDegree
        // (35.0f) degrees
        setNerve(&NrvTamakoro::TamakoroNrvBindStartLand::sInstance);
    } else {
        setNerve(&NrvTamakoro::TamakoroNrvBindStart::sInstance);

        TVec3f v3;  // vec order shenanigans... not used
        TVec3f vPlayerToThis;
        vPlayerToThis = mPosition - mMarioPos;
        vPlayerToThis.rejection(mGravity);

        if (!MR::normalizeOrZero(&vPlayerToThis)) {
            if (vPlayerToThis.dot(mMarioFront) > 0.0f) {
                mMarioFront.set(vPlayerToThis);
            } else {
                mMarioFront.set(-vPlayerToThis);
            }
        }
    }

    if (!isNeedTutorial()) {
        MR::stopStageBGM(40);
    }
    return true;
}

bool Tamakoro::requestEndBind() {
    if (!isNerve(&NrvTamakoro::TamakoroNrvBindEnd::sInstance) && !isNerve(&NrvTamakoro::TamakoroNrvStandByBind::sInstance)) {
        MR::startBckPlayer("SwingRopeSpin", static_cast< const char* >(nullptr));
        MR::endBindAndPlayerForceJump(
            this, mMarioFront.multiplyOperatorInline(-sEndBindFrontPower).addOperatorInLine(mGravity.multiplyOperatorInline(-sEndBindJumpPower)), 0);
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
        MR::zeroVelocity(this);
        mAccelSensorCtrl->notifyDeactivate();
        setNerve(&NrvTamakoro::TamakoroNrvBindEnd::sInstance);
        return true;
    }
    return false;
}

void Tamakoro::startRide() {
    if (isNerve(&NrvTamakoro::TamakoroNrvStandByTutorial::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvStandByBind::sInstance);
    }
}

bool Tamakoro::requestTutorialEnd() {
    if (isNerve(&NrvTamakoro::TamakoroNrvTutorial::sInstance)) {
        MR::zeroVelocity(this);

        if (MR::isEqualStageName("HellProminenceGalaxy")) {
            MR::startStageBGM("BGM_TAMAKORO_2", false);
        } else {
            MR::startStageBGM("BGM_TAMAKORO", false);
        }

        mBgmCtrl->init();
        setNerve(&NrvTamakoro::TamakoroNrvWait::sInstance);
        return true;
    }
    return false;
}

bool Tamakoro::tryFall() {
    if (mAirTime > sFallStartTime) {
        setNerve(&NrvTamakoro::TamakoroNrvFall::sInstance);
        return true;
    }
    return false;
}

bool Tamakoro::tryJump() {
    if (mAccelSensorCtrl->calcJumpPower() && MR::isBindedGround(this)) {
        setNerve(&NrvTamakoro::TamakoroNrvJump::sInstance);
        return true;
    }
    return false;
}

bool Tamakoro::tryBumpWall() {
    if (MR::calcHitPowerToWall(this) > sNeedBumpWallPower) {
        f32 rebound = -0.6f;
        if (isEnebleHitCollisionMessage()) {
            if (MR::sendMsgToWallSensor(ACTMES_BALL_DASH_WALL, getSensor("Body"))) {
                rebound = 0.3f;
            }
        }
        MR::reboundVelocityFromCollision(this, rebound, 0.0f, 1.0f);
        mMarioOffsetVelocity = 2.0f;
        setNerve(&NrvTamakoro::TamakoroNrvBumpWall::sInstance);
        return true;
    }
    return false;
}

bool Tamakoro::tryLand() {
    f32 power = MR::calcHitPowerToGround(this);

    if (MR::calcHitPowerToGround(this) > sNeedLandPower) {
        f32 rebound = 0.0f;
        if (isEnebleHitCollisionMessage()) {
            if (MR::sendMsgToWallSensor(ACTMES_BALL_DASH_GROUND, getSensor("Body"))) {
                rebound = 0.3f;
            }
        }
        mMarioOffsetVelocity = -power;
        mMarioOffset -= power;
        MR::reboundVelocityFromCollision(this, rebound, 0.0f, 1.0f);
        setNerve(&NrvTamakoro::TamakoroNrvLand::sInstance);
        return true;
    }

    if (MR::isGreaterStep(this, sLandTime) && MR::isBindedGround(this)) {
        mMarioOffsetVelocity = -power;
        setNerve(&NrvTamakoro::TamakoroNrvWait::sInstance);
        return true;
    }

    return false;
}

void Tamakoro::reactionCollision() {
    f32 hitPower;

    f32 wallHitPower = MR::calcHitPowerToWall(this);
    f32 groundHitPower = MR::calcHitPowerToGround(this);

    if (wallHitPower >= groundHitPower) {
        hitPower = wallHitPower;
    } else {
        hitPower = groundHitPower;
    }

    if (wallHitPower > sCollisionWeakPower) {
        MR::emitEffectHit(this, *MR::getWallHitPos(this), *MR::getWallNormal(this), "WallHit");
    }

    if (groundHitPower > sCollisionMiddlePower) {
        MR::emitEffectHit(this, *MR::getGroundHitPos(this), *MR::getGroundNormal(this), "Land");
    }

    if (wallHitPower > sCollisionWeakPower) {
        MR::startSound(this, "SE_SM_IRONSPH_HIT", -1, -1);
        MR::startSoundPlayer("SE_PV_GUARD", -1);
    }

    if (hitPower > sCollisionStrongPower) {
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraStrong();
    } else if (hitPower > sCollisionMiddlePower) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::shakeCameraNormalWeak();
    } else if (hitPower > sCollisionWeakPower) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::shakeCameraWeak();
    }
}

void Tamakoro::exeStandByTutorial() {
    updateVelocityNormal(0.95f);
}

void Tamakoro::exeStandByBind() {
    updateVelocityNormal(0.95f);
}

void Tamakoro::exeBindStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SlideStomachRecover", static_cast< const char* >(nullptr));
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        mDirectionToMario.set(-mGravity);
        if (MR::normalizeOrZero(&mMarioFront)) {
            MR::makeAxisVerticalZX(&mMarioFront, mGravity);
        }
        MR::emitEffectHit(this, mMarioPos, mMarioPos - mPosition, "TamakoroLand");
    }

    // smoothly transition arc from bind request position
    // to top of the ball using the nerve rate as time
    f32 time = MR::calcNerveRate(this, sBindStartTime);
    TVec3f expectMario;
    JMAVECScaleAdd(&mDirectionToMario, &mPosition, &expectMario, mBaseRadius);
    TVec3f horizontalVec;
    MR::vecBlend(mMarioBindRequestPos, expectMario, &horizontalVec, time);
    time = (time * 2.0f) - 1.0f;
    TVec3f upVec;
    upVec.set(mGravity.multiplyOperatorInline(-(time * time * -sBindStartJumpHeight + mBaseRadius)));

    MR::blendQuatUpFront(&mMarioRotateQuat, -mGravity, mMarioFront, sBindStartUpAdjustRate, sBindStartFrontAdjustRate);

    mMarioPos.set(horizontalVec.addOperatorInLine(upVec));

    if (MR::isGreaterStep(this, sBindStartTime)) {
        setNerve(&NrvTamakoro::TamakoroNrvBindStartLand::sInstance);
    }
}

void Tamakoro::exeBindStartLand() {
    if (MR::isFirstStep(this)) {
        if (!isNeedTutorial()) {
            if (MR::isEqualStageName("HellProminenceGalaxy")) {
                MR::startStageBGM("BGM_TAMAKORO_2", false);
            } else {
                MR::startStageBGM("BGM_TAMAKORO", false);
            }

            mBgmCtrl->init();
        }
        MR::startBckPlayer("Land", "BallLand");
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);

        mDirectionToMario.set(mMarioPos - mPosition);
        if (MR::normalizeOrZero(&mDirectionToMario)) {
            mDirectionToMario.set(-mGravity);
        }
        JMAVECScaleAdd(&mDirectionToMario, &mPosition, &mMarioPos, mBaseRadius);

        mMarioUp = (mDirectionToMario - mGravity).multiplyOperatorInline(0.5f);
        if (MR::normalizeOrZero(&mMarioUp)) {
            mMarioUp.set(-mGravity);
        }

        MR::emitEffectHit(this, mMarioPos, mMarioPos - mPosition, "TamakoroLand");
        MR::startSoundPlayer("SE_PV_CATCH", -1);
        MR::startSound(this, "SE_SM_RIDE_START", -1, -1);

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
    }

    MR::blendQuatUpFront(&mMarioRotateQuat, mMarioUp, mMarioFront, sBindStartUpAdjustRate, sBindStartFrontAdjustRate);

    if (MR::isGreaterStep(this, sBindStartLandTime)) {
        if (isNeedTutorial()) {
            mTutorial->requestStart();
            setNerve(&NrvTamakoro::TamakoroNrvTutorial::sInstance);
        } else {
            mTutorial->requestEnd();
            setNerve(&NrvTamakoro::TamakoroNrvWait::sInstance);
        }
    }
}

void Tamakoro::exeTutorial() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("タマコロ移動");
        MR::zeroVelocity(this);
        mAccelDir.zero();
        mAccelRate = 0.0f;
        mMoment.zero();
        mKickVel.zero();
    }

    mAccelRate = mAccelSensorCtrl->calcMoveVector(&mAccelDir, mGravity);

    JMAVECScaleAdd(&mAccelDir, &mKickVel, &mKickVel, mAccelRate * sTutorialAccel);

    TVec3f moment;
    MR::calcMomentRollBall(&moment, mKickVel, -mGravity, mBaseRadius);
    MR::vecBlend(mMoment, moment, &mMoment, 0.25f);
    mKickVel.mult(sTutorialFric);

    updateMoveBckBlend(mMoment.length() * mBaseRadius);
    updateMarioPose(3.0f);
    updateMarioOffset();
}

void Tamakoro::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("タマコロ移動");
    }
    reactionCollision();
    addVelocityOperate();
    updateVelocityNormal(sGroundFric);
    updateMoveBckBlend(mMoment.length() * mBaseRadius);
    updateMarioPose(3.0f);
    updateMarioOffset();

    if (tryJump() || tryFall()) {
        return;
    }
}

void Tamakoro::exeFall() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("Fall", "BallFall");
    }
    mMarioOffset += sFallOffsetSpeed;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, sMarioJumpOffsetMax);

    reactionCollision();

    if (!tryLand()) {
        addVelocityOperate();
        updateVelocityNormal(sGroundFric);
        updateMarioPose(3.0f);
    }
}

void Tamakoro::exeJump() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallJump", "BallJump");
        MR::startSoundPlayer("SE_PV_JUMP_M", -1);
        MR::startSound(this, "SE_SM_BALLOONSPH_JUMP", -1, -1);
        MR::addVelocityToGravity(this, (-sJumpPowerV - mVelocity.dot(mGravity)));
        MR::addVelocityMoveToDirection(this, mAccelDir, mAccelRate * sJumpPowerH);
    }

    mMarioOffset += sJumpOffsetSpeed;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, sMarioJumpOffsetMax);

    reactionCollision();

    if (tryBumpWall() || tryLand()) {
        return;
    }

    addVelocityOperate();
    updateVelocityNormal(sGroundFric);
    updateMarioPose(3.0f);
}

void Tamakoro::exeBumpWall() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallHit", static_cast< const char* >(nullptr));
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::shakeCameraNormalWeak();
    }

    reactionCollision();

    addVelocityOperate();
    updateVelocityNormal(sGroundFric);
    updateMarioPose(3.0f);
    updateMarioOffset();

    if (!tryJump() && MR::isGreaterStep(this, 15)) {
        setNerve(&NrvTamakoro::TamakoroNrvWait::sInstance);
    }
}

void Tamakoro::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("Land", static_cast< const char* >(nullptr));
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::shakeCameraNormalWeak();
        MR::startSound(this, "SE_SM_IRONSPH_LAND", -1, -1);
    }

    reactionCollision();

    addVelocityOperate();
    updateVelocityNormal(sLandFric);
    updateMarioPose(3.0f);
    updateMarioOffset();

    if (!tryJump() && MR::isGreaterStep(this, 15)) {
        setNerve(&NrvTamakoro::TamakoroNrvWait::sInstance);
    }
}

void Tamakoro::exeJumpHole() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::offBind(this);
        MR::startBckPlayerJ("タマコロしゃがみ");
        mAccelDir.zero();
        mAccelRate = 0.0f;
        mMarioOffsetVelocity = 0.0f;
    }
    updateSquatBckBlend(mMoment.length() * mBaseRadius);
    updateMarioPose(3.0f);
    updateMarioOffset();
}

void Tamakoro::exeJumpHoleSetUp() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
    }
    updateSquatBckBlend(0.0f);
    updateMarioPose(3.0f);
    updateMarioOffset();
}

void Tamakoro::exeJumpHoleLaunch() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallJump", static_cast< const char* >(nullptr));
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraStrong();
        MR::emitEffect(this, "BigJumpBlur");
    }
    mControlDisabled = true;

    mMarioOffset += 3.0f;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, sMarioJumpOffsetMax);

    updateMarioPose(3.0f);
}

void Tamakoro::endJumpHoleLaunch() {
    MR::onBind(this);
    MR::deleteEffect(this, "BigJumpBlur");
    MR::emitEffect(this, "BigJumpLand");
}

void Tamakoro::exeDashRail() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallRoll", static_cast< const char* >(nullptr));
        MR::startSoundPlayer("SE_PV_JUMP_JOY", -1);
        MR::startSound(this, "SE_SM_IRONSPH_RAILDASH", -1, -1);
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        mAccelDir.zero();
        mAccelRate = 0.0f;
    }
    mControlDisabled = true;
    updateMarioPose(10.0f);
    updateMarioOffset();
    MR::setBckRatePlayer(mVelocity.length() / sDashBallRollSpeed);
}

void Tamakoro::exeDashRailEnd() {
    if (MR::isFirstStep(this)) {
        mMarioOffsetVelocity = 0.0f;
    }

    if (MR::isLessStep(this, sDashRailTime)) {
        MR::setBckRatePlayer(mVelocity.length() / sDashBallRollSpeed);
    }

    if (MR::isStep(this, sDashRailTime)) {
        MR::startBckPlayer("Fall", "BallFall");
    }

    mMarioOffset += 3.0f;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, sMarioJumpOffsetMax);

    if (tryBumpWall() || tryLand()) {
        return;
    }

    updateVelocityNormal(sDashRailFric);
    updateMarioPose(10.0f);
}

void Tamakoro::exeRideRail() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("タマコロ移動");
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }

    f32 speed = updateRideRail();
    updateMoveBckBlend(speed);
    if (speed > sRaidRailFastSpeed) {
        setNerve(&NrvTamakoro::TamakoroNrvRideRailFastStart::sInstance);
    }
}

void Tamakoro::exeRideRailFastStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallRollStart", static_cast< const char* >(nullptr));
        MR::setBckRatePlayer(sRollBckRate);
    }
    updateRideRail();
    mControlDisabled = true;
    if (MR::isBckStoppedPlayer()) {
        setNerve(&NrvTamakoro::TamakoroNrvRideRailFast::sInstance);
    }
}

void Tamakoro::exeRideRailFast() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallRoll", static_cast< const char* >(nullptr));
        MR::setBckRatePlayer(sRollBckRate);
    }
    mControlDisabled = true;
    f32 speed = updateRideRail();

    MR::setBckRatePlayer(mVelocity.length() / sDashBallRollSpeed);

    if (speed < sRaidRailFastSpeed) {
        setNerve(&NrvTamakoro::TamakoroNrvRideRailFastEnd::sInstance);
    }
}

void Tamakoro::exeRideRailFastEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallRollEnd", static_cast< const char* >(nullptr));
        MR::setBckRatePlayer(sRollBckRate);
    }
    updateRideRail();
    if (MR::isBckStoppedPlayer()) {
        setNerve(&NrvTamakoro::TamakoroNrvRideRail::sInstance);
    }
}

void Tamakoro::exeBindEnd() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraStrong();
        mAccelDir.zero();
        mAccelRate = 0.0f;
        if (MR::isValidSwitchA(this)) {
            MR::offSwitchA(this);
        }
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_SM_RIDE_END", -1, -1);
        MR::startSoundPlayer("SE_PV_JUMP_S", -1);
        MR::stopStageBGM(10);
    }

    if ((MR::isGreaterStep(this, 12) && MR::isOnGroundPlayer()) || MR::isGreaterStep(this, sForceBindEndTime)) {
        MR::startLastStageBGM();
        MR::requestAppearPowerStar(this, mPosition);
        kill();
    }
}

void Tamakoro::addVelocityOperate() {
    mAccelRate = mAccelSensorCtrl->calcMoveVector(&mAccelDir, mGravity);

    TVec3f* vel = &mVelocity;  // this needs to be like this to match...
    JMAVECScaleAdd(&mAccelDir, vel, &mVelocity, mAccelRate * (MR::isBindedGround(this) ? 0.4f : 0.2f));
}

f32 Tamakoro::updateRideRail() {
    mAccelDir.set(mVelocity);

    if (MR::normalizeOrZero(&mAccelDir)) {
        mAccelRate = 0.5f;
    } else {
        mAccelRate = 0.0f;
    }

    f32 speed = mMoment.length() * mBaseRadius;

    updateMarioPose(15.0f);
    updateMarioOffset();

    return speed;
}

void Tamakoro::updateVelocityNormal(f32 friction) {
    TVec3f vel(mVelocity);  // unused! woo!
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::addVelocityToGravity(this, sGravityAccel);
    MR::attenuateVelocity(this, MR::isBindedGround(this) ? friction : 0.995f);
}

void Tamakoro::updateMoment() {
    if (isGroundMomentBall()) {
        TVec3f moment;
        const TVec3f* up;
        // TVec3f tmpVec;
        if (MR::isBindedGround(this)) {
            up = MR::getGroundNormal(this);
        } else {
            TVec3f tmpVec(-mGravity);
            up = &tmpVec;
        }

        MR::calcMomentRollBall(&moment, mVelocity, *up, mBaseRadius);
        MR::vecBlend(mMoment, moment, &mMoment, 0.25f);
        startRotateLevelSound();
    } else {
        mMoment.mult(0.98f);
    }
}

void Tamakoro::updateAirTime() {
    if (MR::isBindedGround(this)) {
        mAirTime = 0;
    } else {
        mAirTime++;
    }
}

void Tamakoro::updateMarioPose(f32 degree) {
    // TODO : rejection
    TVec3f velH;

    // these *need* to be pointers to match...
    TVec3f* vel = &mVelocity;
    TVec3f* grav = &mGravity;
    f32 dot = grav->dot(*vel);
    JMAVECScaleAdd(grav, vel, &velH, -dot);  // velH.rejection(vel, grav);

    if (MR::isNearZero(velH)) {
        if (mAccelRate > 0.001f) {
            MR::turnDirectionDegree(this, &mMarioFront, mAccelDir, degree * mAccelRate);
        }
    } else {
        MR::turnDirectionDegree(this, &mMarioFront, velH, degree);
    }

    TVec3f zDir;
    TVec3f xDir;
    mMarioRotateQuat.getXDir(xDir);
    mMarioRotateQuat.getZDir(zDir);

    if (mAccelRate > 0.0f) {
        f32 dot = MR::normalize(1.0f - mAccelDir.dot(zDir), 0.1f, 1.0f);
        f32 sign;
        if (mAccelDir.dot(xDir) >= 0.0f) {
            sign = 1.0f;
        } else {
            sign = -1.0f;
        }
        dot = sign * dot;
        mMarioRotateYAngle += dot * sMarioUpperYAccel;
    }
    mMarioRotateYAngle *= sMarioUpperYFric;

    TVec3f up;
    up.set(-mGravity);
    mMarioUp.set(-mGravity);

    TVec3f rotAxis = mAccelDir.cross(mGravity);

    if (!MR::normalizeOrZero(&rotAxis)) {
        MR::rotateVecDegree(&up, rotAxis, mAccelRate * 35.0f);
        MR::rotateVecDegree(&mMarioUp, rotAxis, mAccelRate * 20.0f);
    }
    MR::vecBlendSphere(mDirectionToMario, up, &mDirectionToMario, sStandDirInterRate);

    if (MR::normalizeOrZero(&mDirectionToMario)) {
        mDirectionToMario.set(-mGravity);
    }
    MR::blendQuatUpFront(&mMarioRotateQuat, mMarioUp, mMarioFront, sMarioUpAdjustRate, sMarioFrontAdjustRate);

    f32 rotY = mMarioRotateYAngle;
    MR::setPlayerUpperRotateY(PI_180 * rotY);
}

void Tamakoro::updateMarioOffset() {
    f32 velocity = mMarioOffsetVelocity - sMarioOffsetGravityAccel;
    velocity *= sMarioOffsetFric;

    mMarioOffsetVelocity = velocity;
    mMarioOffset += mMarioOffsetVelocity;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, sMarioJumpOffsetMax);
}

void Tamakoro::updateMoveBckBlend(f32 pSpeed) {
    f32 idleBckWeight = 0.0f;
    f32 softWalkBckWeight = 0.0f;
    f32 walkBckWeight = 0.0f;
    f32 runBckWeight = 0.0f;
    f32 rate = 0.0f;
    f32 softWalkSpeed = sSoftWalkSpeed;
    f32 walkSpeed = sWalkSpeed;
    f32 runSpeed = sRunSpeed;

    if (pSpeed < softWalkSpeed) {
        softWalkBckWeight = MR::normalize(pSpeed, 0.0f, softWalkSpeed);
        idleBckWeight = 1.0f - softWalkBckWeight;
        rate = MR::getLinerValue(softWalkBckWeight, sIdleBckRate, sSoftWalkBckRate, 1.0f);
    } else if (pSpeed < walkSpeed) {
        walkBckWeight = MR::normalize(pSpeed, softWalkSpeed, walkSpeed);
        softWalkBckWeight = 1.0f - walkBckWeight;
        rate = MR::getLinerValue(walkBckWeight, sSoftWalkBckRate, sWalkBckRate, 1.0f);
    } else if (pSpeed < runSpeed) {
        runBckWeight = MR::normalize(pSpeed, walkSpeed, runSpeed);
        walkBckWeight = 1.0f - runBckWeight;
        rate = MR::getLinerValue(runBckWeight, sWalkBckRate, sRunBckRate, 1.0f);
    } else {
        runBckWeight = 1.0f;
        rate = (pSpeed / runSpeed) * sRunBckRate;
        if (rate > sRunMaxBckRate) {
            rate = sRunMaxBckRate;
        }
    }

    MR::setBckBlendWeight(idleBckWeight, softWalkBckWeight, walkBckWeight, runBckWeight);
    MR::setBckRatePlayer(rate);
}

void Tamakoro::updateSquatBckBlend(f32 pSpeed) {
    f32 limit2 = 6.0f;
    f32 w1 = MR::normalize(pSpeed, 0.0f, limit2);
    f32 w0 = 1.0f - w1;
    f32 rate = MR::getLinerValue(w1, 1.0f, 2.0f, 1.0f);

    MR::setBckBlendWeight(w0, w1);
    MR::setBckRatePlayer(rate);
}

bool Tamakoro::isEnablePushPlayer() const {
    if (isNerve(&NrvTamakoro::TamakoroNrvStandByTutorial::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvStandByBind::sInstance)) {
        return true;
    }
    return false;
}

bool Tamakoro::isEnableEnemyAttack() const {
    if (isNerve(&NrvTamakoro::TamakoroNrvWait::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvFall::sInstance) ||
        isNerve(&NrvTamakoro::TamakoroNrvJump::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvBumpWall::sInstance) ||
        isNerve(&NrvTamakoro::TamakoroNrvDashRailEnd::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvLand::sInstance)) {
        return true;
    }
    return false;
}

bool Tamakoro::isEnableBallBind() const {
    if (isNerve(&NrvTamakoro::TamakoroNrvWait::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvFall::sInstance) ||
        isNerve(&NrvTamakoro::TamakoroNrvJump::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvBumpWall::sInstance) ||
        isNerve(&NrvTamakoro::TamakoroNrvDashRailEnd::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvLand::sInstance)) {
        return true;
    }
    return false;
}

bool Tamakoro::isEnebleHitCollisionMessage() const {
    return isNerve(&NrvTamakoro::TamakoroNrvJump::sInstance);
}

bool Tamakoro::isUseMarioOffset() const {
    if (isNerve(&NrvTamakoro::TamakoroNrvBindStart::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvBindStartLand::sInstance)) {
        return false;
    }
    return true;
}

bool Tamakoro::isGroundMomentBall() const {
    if (MR::isBindedGround(this) || isRideRail() || isNerve(&NrvTamakoro::TamakoroNrvDashRail::sInstance) ||
        isNerve(&NrvTamakoro::TamakoroNrvJumpHole::sInstance)) {
        return true;
    }
    return false;
}

bool Tamakoro::isRideRail() const {
    if (isNerve(&NrvTamakoro::TamakoroNrvRideRail::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvRideRailFastStart::sInstance) ||
        isNerve(&NrvTamakoro::TamakoroNrvRideRailFast::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvRideRailFastEnd::sInstance)) {
        return true;
    }
    return false;
}

bool Tamakoro::isNeedTutorial() const {
    if (mTutorial == nullptr) {
        return false;
    }
    return mTutorial->isNeedTutorial();
}

void Tamakoro::startRotateLevelSound() {
    f32 mag = mMoment.length();

    if (mag <= 0.001f) {
        return;
    }

    s32 lvl = ((mag * 10000.0f) / TWO_PI);
    if (isNerve(&NrvTamakoro::TamakoroNrvRideRail::sInstance)) {
        MR::startLevelSound(this, "SE_SM_LV_IRONSPH_ROLL_RAIL", lvl, -1, -1);
    } else {
        MR::startLevelSound(this, "SE_SM_LV_IRONSPH_ROLL1", lvl, -1, -1);
    }
}

namespace MR {
    NameObj* createTamakoroWithTutorial(const char* pName) {
        Tamakoro* tamakoro = new Tamakoro(pName);
        tamakoro->setTutorial();
        return tamakoro;
    }
};  // namespace MR
