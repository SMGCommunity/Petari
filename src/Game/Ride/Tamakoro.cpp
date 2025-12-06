#include "Game/Ride/Tamakoro.hpp"
#include "Game/GameAudio/AudTamakoroBgmCtrl.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Ride/SphereAccelSensorController.hpp"
#include "Game/Ride/TamakoroTutorial.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"
#include "revolution/wpad.h"

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
    : LiveActor(pName), mAccelSensorCtrl(nullptr), mTutorial(nullptr), mJointCtrl(nullptr), mBaseQuat(0, 0, 0, 1), mRotateQuat(0, 0, 0, 1),
      mRingUp(0, 1, 0), mKickVel(0, 0, 0), mMarioBindRequestPos(0, 0, 0), mMarioRotateUp(0, 1, 0), mMarioPos(0, 0, 0), mMoment(0, 0, 0),
      mMarioRotateFront(0, 0, 1), mDirectionToMario(0, 0, 0), mMoveVec(0, 0, 0), mAirTime(0), mMoveSpeed(0.0f), mMarioRotateYAngle(0.0f),
      mMarioOffset(0.0f), mMarioOffsetVelocity(0.0f), mBgmCtrl(nullptr), mControlDisabled(false), mHasTutorial(false) {
    mAccelSensorCtrl = new SphereAccelSensorController();
    mBgmCtrl = new AudTamakoroBgmCtrl();
}

void Tamakoro::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Tamakoro", 0, false);
    MR::connectToScene(this, 0x29, 7, 0x1a, -1);
    MR::makeQuatFromRotate(&mBaseQuat, this);
    MR::initShadowVolumeSphere(this, 150.0f);
    initEffectKeeper(0, 0, false);
    initSensor();
    initSound(4, false);
    initJointControl();
    MR::onCalcGravity(this);
    initBinder(150.0f, 0.0f, 16);
    initNerve(&NrvTamakoro::TamakoroNrvStandByBind::sInstance);

    MR::declarePowerStar(this);
    bool hasPowerStar = MR::hasPowerStarInCurrentStageWithDeclarer(mName, -1);
    MR::startBva(this, "Tamakoro");
    MR::setBvaFrameAndStop(this, (s32)hasPowerStar);
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
    MR::addHitSensor(this, "Body", ATYPE_SPHERE_PLAYER, 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "Hit", ATYPE_SPHERE_PLAYER_HIT, 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "Bind", ATYPE_SPHERE_PLAYER_BIND, 8, 165.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

void Tamakoro::initJointControl() {
    mJointCtrl = MR::createJointDelegatorWithNullChildFunc(this, &Tamakoro::ringMtxCallBack, "Ring");
}

bool Tamakoro::ringMtxCallBack(TPos3f* pMtx, const JointControllerInfo& pJointCtrlInfo) {
    TVec3f trans;
    pMtx->getTransInline(trans);

    if (MR::isNearZero(mMoveVec, 0.001f) || MR::isSameDirection(mMoveVec, mRingUp, 0.01f)) {
        MR::makeMtxUpNoSupport(pMtx, mRingUp);
    } else {
        MR::makeMtxUpFront(pMtx, mRingUp, mMoveVec);
    }

    pMtx->setTrans(trans);
    return true;
}

void Tamakoro::control() {
    updateMoment();
    MR::rotateQuatMoment(&mBaseQuat, mMoment);
    updateAirTime();
    updateRingUpVec();
    f32 mag = PSVECMag(&mVelocity);
    if (isNerve(&NrvTamakoro::TamakoroNrvJumpHole::sInstance) || isNerve(&NrvTamakoro::TamakoroNrvJumpHoleSetUp::sInstance)) {
        mag = 0.0f;
    }
    mBgmCtrl->control(mag, mControlDisabled, 30);
    mControlDisabled = false;
}

void Tamakoro::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mBaseQuat);
    mJointCtrl->registerCallBack();
}

void Tamakoro::updateBindActorMatrix() {
    // warning, does not match because of the quaternion rotation inline
    // though logic is correct.
    if (isUseMarioOffset()) {
        JMAVECScaleAdd(&mDirectionToMario, &mPosition, &mMarioPos, 150.0f);
        if (mMarioOffset > 0) {
            JMAVECScaleAdd(&mGravity, &mMarioPos, &mMarioPos, -mMarioOffset);
        }
    }

    // quaternion rotation
    TPos3f mtx;
    mtx.setRotateQuaternionInline(mRotateQuat);
    mtx.setTrans(mMarioPos);
    MR::setPlayerBaseMtx(mtx);
}

void Tamakoro::updateRingUpVec() {
    TVec3f up = -mGravity;

    if (!MR::isNearZero(mMoveVec, 0.001f)) {
        TVec3f v1;
        TVec3f up2 = -mGravity;
        JMAVECScaleAdd(&mMoveVec, &up2, &v1, mMoveSpeed);
        MR::normalizeOrZero(&v1);
        MR::turnVecToVecDegree(&up, up, v1, 45.0f, TVec3f(0, 1, 0));
    }

    MR::turnVecToVecDegree(&mRingUp, mRingUp, up, 4.0f, TVec3f(0, 1, 0));
}

void Tamakoro::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender->isType(0x0A) && MR::tryGetItem(pSender, pReceiver)) {
        return;
    }

    if ((MR::isSensorPlayer(pReceiver) || MR::isSensorEnemy(pReceiver)) && isEnablePushPlayer()) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (isEnableEnemyAttack() && MR::isSensorEnemy(pReceiver) && pSender->isType(0x0A)) {
        if (MR::sendMsgPlayerTrample(pReceiver, pSender)) {
            f32 dot = mVelocity.dot(mGravity);
            if (dot < 15.0f) {
                dot = -30.0f - dot;
                mVelocity.add(mGravity.multiplyOperatorInline(dot));
                MR::tryRumblePadWeak(this, 0);
                MR::shakeCameraNormalWeak();
            }
            return;
        }
    }

    if (isEnableBallBind() && pSender == getSensor("Body")) {
        // this is gross, but it matches... even though the commented
        // logic below is what it's really doing, not entirely sure
        // why this matches...

        bool set;
        if (pReceiver->isType(0x7A)) {
            set = pReceiver->receiveMessage(0xAD, pSender);
        } else {
            set = false;
        }
        if (set) {
            setNerve(&NrvTamakoro::TamakoroNrvDashRail::sInstance);
            return;
        }

        if (pReceiver->isType(0x79)) {
            set = pReceiver->receiveMessage(0xAD, pSender);
        } else {
            set = false;
        }
        if (set) {
            setNerve(&NrvTamakoro::TamakoroNrvJumpHole::sInstance);
            return;
        }

        if (pReceiver->isType(0x7B)) {
            set = pReceiver->receiveMessage(0xAD, pSender);
        } else {
            set = false;
        }
        if (set) {
            setNerve(&NrvTamakoro::TamakoroNrvRideRail::sInstance);
            return;
        }

        /*
        if (pSender->isType(0x7A) && pReceiver->receiveMessage(0xAD, pSender)) {
            setNerve(&NrvTamakoro::TamakoroNrvDashRail::sInstance);
            return;
        }

        if (pSender->isType(0x79) && pReceiver->receiveMessage(0xAD, pSender)) {
            setNerve(&NrvTamakoro::TamakoroNrvJumpHole::sInstance);
            return;
        }

        if (pSender->isType(0x7B) && pReceiver->receiveMessage(0xAD, pSender)) {
            setNerve(&NrvTamakoro::TamakoroNrvRideRail::sInstance);
            return;
        }
        */
    }
}

bool Tamakoro::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgStarPieceReflect(msg);
}

bool Tamakoro::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg) && isEnableEnemyAttack()) {
        MR::tryRumblePadStrong(this, 0);
        MR::setVelocitySeparateHV(this, pSender, pReceiver, 20.0, 40.0);
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

    if (msg == 0x94) {
        setNerve(&NrvTamakoro::TamakoroNrvStandByBind::sInstance);
        return true;
    }

    if (msg == 0xAE && isNerve(&NrvTamakoro::TamakoroNrvJumpHole::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvJumpHoleSetUp::sInstance);
        return true;
    }

    if (msg == 0xAF && isNerve(&NrvTamakoro::TamakoroNrvJumpHoleSetUp::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvJumpHoleLaunch::sInstance);
        return true;
    }

    if (msg == 0xB0 && isNerve(&NrvTamakoro::TamakoroNrvJumpHoleLaunch::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvFall::sInstance);
        return true;
    }

    if (msg == 0xB1 && isNerve(&NrvTamakoro::TamakoroNrvDashRail::sInstance)) {
        setNerve(&NrvTamakoro::TamakoroNrvDashRailEnd::sInstance);
        return true;
    }

    if (msg == 0xB2 && isRideRail()) {
        setNerve(&NrvTamakoro::TamakoroNrvWait::sInstance);
        return true;
    }

    if (msg == 0xB3 && requestEndBind()) {
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
    f32 cosVerticalAngle = vThisToSensor.dot(-mGravity);
    if (cosVerticalAngle < MR::cosDegree(60.0f)) {
        return false;
    }

    MR::invalidateClipping(this);

    MR::makeQuatRotateDegree(&mRotateQuat, *MR::getPlayerRotate());
    mMarioPos.set(*MR::getPlayerPos());
    mRotateQuat.getZDir(mMarioRotateFront);
    TVec3f* grav = &mGravity;
    JMAVECScaleAdd(grav, &mMarioRotateFront, &mMarioRotateFront,
                   -grav->dot(mMarioRotateFront));  // mMarioRotateFront.rejection(mMarioRotateFront, mGravity);
    if (MR::normalizeOrZero(&mMarioRotateFront)) {
        MR::makeAxisVerticalZX(&mMarioRotateFront, mGravity);
    }

    mMarioBindRequestPos.set(mMarioPos);
    mMarioRotateYAngle = 0.0f;
    mMoveSpeed = 0.0f;
    mMarioOffset = 0.0f;
    mMarioOffsetVelocity = 0.0f;

    mMoveVec.zero();
    MR::zeroVelocity(this);
    MR::invalidateHitSensor(this, "Bind");

    TVec3f vel(*MR::getPlayerVelocity());
    MR::normalizeOrZero(&vel);
    f32 cosBetweenVelAndToThis = -vThisToSensor.dot(vel);

    if (cosBetweenVelAndToThis > MR::cosDegree(40.0f) && cosVerticalAngle > MR::cosDegree(35.0f)) {
        // if angle between vel and "to this" < 40 degrees and vertical angle to sensor < 35 degrees
        setNerve(&NrvTamakoro::TamakoroNrvBindStartLand::sInstance);
    } else {
        setNerve(&NrvTamakoro::TamakoroNrvBindStart::sInstance);

        TVec3f v3;  // vec order shenanigans... not used
        TVec3f vPlayerToThis;
        vPlayerToThis = mPosition - mMarioPos;

        TVec3f* grav = &mGravity;
        JMAVECScaleAdd(grav, &vPlayerToThis, &vPlayerToThis, -grav->dot(vPlayerToThis));  // vPlayerToThis.rejection(vPlayerToThis, mGravity);

        if (!MR::normalizeOrZero(&vPlayerToThis)) {
            f32 dot = vPlayerToThis.dot(mMarioRotateFront);
            if (dot > 0.0f) {
                mMarioRotateFront.set(vPlayerToThis);
            } else {
                mMarioRotateFront.set(-vPlayerToThis);
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
        MR::startBckPlayer("SwingRopeSpin", (const char*)0);
        MR::endBindAndPlayerForceJump(this,
                                      mMarioRotateFront.multiplyOperatorInline(-5.0f).addOperatorInLine(mGravity.multiplyOperatorInline(-35.0f)), 0);
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
    if (mAirTime > 5) {
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
    if (MR::calcHitPowerToWall(this) > 10.0f) {
        f32 rebound = -0.6f;
        if (isEnebleHitCollisionMessage()) {
            if (MR::sendMsgToWallSensor(0x39, getSensor("Body"))) {
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
    f32 f1 = MR::calcHitPowerToGround(this);

    if (MR::calcHitPowerToGround(this) > 20.0f) {
        f32 rebound = 0.0f;
        if (isEnebleHitCollisionMessage()) {
            if (MR::sendMsgToWallSensor(0x3A, getSensor("Body"))) {
                rebound = 0.3f;
            }
        }
        mMarioOffsetVelocity = -f1;
        mMarioOffset -= f1;
        MR::reboundVelocityFromCollision(this, rebound, 0.0f, 1.0f);
        setNerve(&NrvTamakoro::TamakoroNrvLand::sInstance);
        return true;
    }

    if (MR::isGreaterStep(this, 2) && MR::isBindedGround(this)) {
        mMarioOffsetVelocity = -f1;
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

    if (wallHitPower > 5.0f) {
        MR::emitEffectHit(this, *MR::getWallHitPos(this), *MR::getWallNormal(this), "WallHit");
    }

    if (groundHitPower > 25.0f) {
        MR::emitEffectHit(this, *MR::getGroundHitPos(this), *MR::getGroundNormal(this), "Land");
    }

    if (wallHitPower > 5.0f) {
        MR::startSound(this, "SE_SM_IRONSPH_HIT", -1, -1);
        MR::startSoundPlayer("SE_PV_GUARD", -1);
    }

    if (hitPower > 60.0f) {
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraStrong();
    } else if (hitPower > 25.0f) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::shakeCameraNormalWeak();
    } else if (hitPower > 5.0f) {
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::shakeCameraWeak();
    }
}

inline void Tamakoro::exeStandByTutorial() {
    TVec3f vel(mVelocity);  // unused! woo!
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::addVelocityToGravity(this, 2.0f);
    f32 attenuation;
    if (MR::isBindedGround(this)) {
        attenuation = 0.95f;
    } else {
        attenuation = 0.995f;
    }
    MR::attenuateVelocity(this, attenuation);
}

inline void Tamakoro::exeStandByBind() {
    TVec3f vel(mVelocity);  // unused! woo!
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::addVelocityToGravity(this, 2.0f);
    f32 attenuation;
    if (MR::isBindedGround(this)) {
        attenuation = 0.95f;
    } else {
        attenuation = 0.995f;
    }
    MR::attenuateVelocity(this, attenuation);
}

void Tamakoro::exeBindStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SlideStomachRecover", (const char*)0);
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        mDirectionToMario.set(-mGravity);
        if (MR::normalizeOrZero(&mMarioRotateFront)) {
            MR::makeAxisVerticalZX(&mMarioRotateFront, mGravity);
        }
        MR::emitEffectHit(this, mMarioPos, mMarioPos - mPosition, "TamakoroLand");
    }

    // smoothly transition arc from bind request position
    // to top of the ball using the nerve rate as time
    f32 time = MR::calcNerveRate(this, 40);
    TVec3f expectMario;
    JMAVECScaleAdd(&mDirectionToMario, &mPosition, &expectMario, 150.0f);
    TVec3f horizontalVec;
    MR::vecBlend(mMarioBindRequestPos, expectMario, &horizontalVec, time);
    time = (time * 2.0f) - 1.0f;
    TVec3f upVec;
    upVec.set(mGravity.multiplyOperatorInline(-(time * time * -150.0f + 150.0f)));

    MR::blendQuatUpFront(&mRotateQuat, -mGravity, mMarioRotateFront, 0.1f, 0.1f);

    mMarioPos.set(horizontalVec.addOperatorInLine(upVec));

    if (MR::isGreaterStep(this, 40)) {
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
        JMAVECScaleAdd(&mDirectionToMario, &mPosition, &mMarioPos, 150.0f);

        mMarioRotateUp = (mDirectionToMario - mGravity).multiplyOperatorInline(0.5f);
        if (MR::normalizeOrZero(&mMarioRotateUp)) {
            mMarioRotateUp.set(-mGravity);
        }

        MR::emitEffectHit(this, mMarioPos, mMarioPos - mPosition, "TamakoroLand");
        MR::startSoundPlayer("SE_PV_CATCH", -1);
        MR::startSound(this, "SE_SM_RIDE_START", -1, -1);

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }
    }

    MR::blendQuatUpFront(&mRotateQuat, mMarioRotateUp, mMarioRotateFront, 0.1f, 0.1f);

    if (MR::isGreaterStep(this, 14)) {
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
        mMoveVec.zero();
        mMoveSpeed = 0.0f;
        mMoment.zero();
        mKickVel.zero();
    }

    mMoveSpeed = mAccelSensorCtrl->calcMoveVector(&mMoveVec, mGravity);

    JMAVECScaleAdd(&mMoveVec, &mKickVel, &mKickVel, mMoveSpeed * 0.5f);

    TVec3f moment;
    MR::calcMomentRollBall(&moment, mKickVel, -mGravity, 150.0f);
    MR::vecBlend(mMoment, moment, &mMoment, 0.25f);
    mKickVel.mult(0.95f);

    updateMoveBckBlend(PSVECMag(&mMoment) * 150.0f);
    updateMarioPose(3.0f);
    updateMarioOffset();
}

void Tamakoro::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("タマコロ移動");
    }
    reactionCollision();
    addVelocityOperate();

    TVec3f vel(mVelocity);  // unused! woo!
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::addVelocityToGravity(this, 2.0f);
    f32 attenuation;
    if (MR::isBindedGround(this)) {
        attenuation = 0.99f;
    } else {
        attenuation = 0.995f;
    }
    MR::attenuateVelocity(this, attenuation);
    updateMoveBckBlend(PSVECMag(&mMoment) * 150.0f);
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
    mMarioOffset += 3.0f;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, 150.0f);

    reactionCollision();

    if (!tryLand()) {
        addVelocityOperate();
        TVec3f vel(mVelocity);  // unused! woo!
        MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
        MR::addVelocityToGravity(this, 2.0f);
        f32 attenuation;
        if (MR::isBindedGround(this)) {
            attenuation = 0.99f;
        } else {
            attenuation = 0.995f;
        }
        MR::attenuateVelocity(this, attenuation);
        updateMarioPose(3.0f);
    }
}

void Tamakoro::exeJump() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallJump", "BallJump");
        MR::startSoundPlayer("SE_PV_JUMP_M", -1);
        MR::startSound(this, "SE_SM_BALLOONSPH_JUMP", -1, -1);
        MR::addVelocityToGravity(this, (-40.0f - mVelocity.dot(mGravity)));
        MR::addVelocityMoveToDirection(this, mMoveVec, mMoveSpeed * 7.0f);
    }

    mMarioOffset += 4.0f;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, 150.0f);

    reactionCollision();

    if (tryBumpWall() || tryLand()) {
        return;
    }

    addVelocityOperate();
    TVec3f vel(mVelocity);  // unused! woo!
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::addVelocityToGravity(this, 2.0f);
    f32 attenuation;
    if (MR::isBindedGround(this)) {
        attenuation = 0.99f;
    } else {
        attenuation = 0.995f;
    }
    MR::attenuateVelocity(this, attenuation);
    updateMarioPose(3.0f);
}

void Tamakoro::exeBumpWall() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallHit", (const char*)0);
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::shakeCameraNormalWeak();
    }

    reactionCollision();

    addVelocityOperate();
    TVec3f vel(mVelocity);  // unused! woo!
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::addVelocityToGravity(this, 2.0f);
    f32 attenuation;
    if (MR::isBindedGround(this)) {
        attenuation = 0.99f;
    } else {
        attenuation = 0.995f;
    }
    MR::attenuateVelocity(this, attenuation);
    updateMarioPose(3.0f);

    updateMarioOffset();

    if (!tryJump() && MR::isGreaterStep(this, 15)) {
        setNerve(&NrvTamakoro::TamakoroNrvWait::sInstance);
    }
}

void Tamakoro::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("Land", (const char*)0);
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::shakeCameraNormalWeak();
        MR::startSound(this, "SE_SM_IRONSPH_LAND", -1, -1);
    }

    reactionCollision();

    addVelocityOperate();
    TVec3f vel(mVelocity);  // unused! woo!
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::addVelocityToGravity(this, 2.0f);
    f32 attenuation;
    if (MR::isBindedGround(this)) {
        attenuation = 0.88f;  // ??
    } else {
        attenuation = 0.995f;
    }
    MR::attenuateVelocity(this, attenuation);
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
        mMoveVec.zero();
        mMoveSpeed = 0.0f;
        mMarioOffsetVelocity = 0.0f;
    }
    updateSquatBckBlend(PSVECMag(&mMoment) * 150.0f);
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
        MR::startBckPlayer("BallJump", (const char*)0);
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraStrong();
        MR::emitEffect(this, "BigJumpBlur");
    }
    mControlDisabled = true;

    mMarioOffset += 3.0f;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, 150.0f);

    updateMarioPose(3.0f);
}

void Tamakoro::endJumpHoleLaunch() {
    MR::onBind(this);
    MR::deleteEffect(this, "BigJumpBlur");
    MR::emitEffect(this, "BigJumpLand");
}

void Tamakoro::exeDashRail() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallRoll", (const char*)0);
        MR::startSoundPlayer("SE_PV_JUMP_JOY", -1);
        MR::startSound(this, "SE_SM_IRONSPH_RAILDASH", -1, -1);
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        mMoveVec.zero();
        mMoveSpeed = 0.0f;
    }
    mControlDisabled = true;
    updateMarioPose(10.0f);
    updateMarioOffset();
    MR::setBckRatePlayer(PSVECMag(&mVelocity) / 60.0f);
}

void Tamakoro::exeDashRailEnd() {
    if (MR::isFirstStep(this)) {
        mMarioOffsetVelocity = 0.0f;
    }

    if (MR::isLessStep(this, 40)) {
        MR::setBckRatePlayer(PSVECMag(&mVelocity) / 60.0f);
    }

    if (MR::isStep(this, 40)) {
        MR::startBckPlayer("Fall", "BallFall");
    }

    mMarioOffset += 3.0f;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, 150.0f);

    if (tryBumpWall() || tryLand()) {
        return;
    }

    TVec3f vel(mVelocity);  // unused! woo!
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::addVelocityToGravity(this, 2.0f);
    f32 attenuation;
    if (MR::isBindedGround(this)) {
        attenuation = 0.99f;
    } else {
        attenuation = 0.995f;
    }
    MR::attenuateVelocity(this, attenuation);
    updateMarioPose(10.0f);
}

void Tamakoro::exeRideRail() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("タマコロ移動");
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
    }

    f32 mag = updateRideRail();
    updateMoveBckBlend(mag);
    if (mag > 30.0f) {
        setNerve(&NrvTamakoro::TamakoroNrvRideRailFastStart::sInstance);
    }
}

void Tamakoro::exeRideRailFastStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallRollStart", (const char*)0);
        MR::setBckRatePlayer(1.5);
    }
    updateRideRail();
    mControlDisabled = true;
    if (MR::isBckStoppedPlayer()) {
        setNerve(&NrvTamakoro::TamakoroNrvRideRailFast::sInstance);
    }
}

void Tamakoro::exeRideRailFast() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallRoll", (const char*)0);
        MR::setBckRatePlayer(1.5);
    }
    mControlDisabled = true;
    f32 mag = updateRideRail();

    MR::setBckRatePlayer(PSVECMag(&mVelocity) / 60.0f);

    if (mag < 30.0f) {
        setNerve(&NrvTamakoro::TamakoroNrvRideRailFastEnd::sInstance);
    }
}

void Tamakoro::exeRideRailFastEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BallRollEnd", (const char*)0);
        MR::setBckRatePlayer(1.5);
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
        mMoveVec.zero();
        mMoveSpeed = 0.0f;
        if (MR::isValidSwitchA(this)) {
            MR::offSwitchA(this);
        }
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_SM_RIDE_END", -1, -1);
        MR::startSoundPlayer("SE_PV_JUMP_S", -1);
        MR::stopStageBGM(10);
    }

    if ((MR::isGreaterStep(this, 12) && MR::isOnGroundPlayer()) || MR::isGreaterStep(this, 90)) {
        MR::startLastStageBGM();
        MR::requestAppearPowerStar(this, mPosition);
        kill();
    }
}

void Tamakoro::addVelocityOperate() {
    mMoveSpeed = mAccelSensorCtrl->calcMoveVector(&mMoveVec, mGravity);

    TVec3f* vel = &mVelocity;  // this needs to be like this to match...
    f32 scale;
    if (MR::isBindedGround(this)) {
        scale = 0.4f;
    } else {
        scale = 0.2f;
    }

    JMAVECScaleAdd(&mMoveVec, vel, &mVelocity, mMoveSpeed * scale);
}

f32 Tamakoro::updateRideRail() {
    mMoveVec.set(mVelocity);

    if (MR::normalizeOrZero(&mMoveVec)) {
        mMoveSpeed = 0.5f;
    } else {
        mMoveSpeed = 0.0f;
    }

    f32 mag = PSVECMag(&mMoment) * 150.0f;

    updateMarioPose(15.0f);
    updateMarioOffset();

    return mag;
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

        MR::calcMomentRollBall(&moment, mVelocity, *up, 150.0f);
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

void Tamakoro::updateMarioPose(f32 pDegree) {
    TVec3f v1;

    // these *need* to be pointers to match...
    TVec3f* vel = &mVelocity;
    TVec3f* grav = &mGravity;
    f32 dot = grav->dot(*vel);
    JMAVECScaleAdd(grav, vel, &v1, -dot);  // v1.rejection(vel, grav);

    if (MR::isNearZero(v1, 0.001f)) {
        if (mMoveSpeed > 0.001f) {
            MR::turnDirectionDegree(this, &mMarioRotateFront, mMoveVec, pDegree * mMoveSpeed);
        }
    } else {
        MR::turnDirectionDegree(this, &mMarioRotateFront, v1, pDegree);
    }

    TVec3f zDir;
    TVec3f xDir;
    mRotateQuat.getXDir(xDir);
    mRotateQuat.getZDir(zDir);

    if (mMoveSpeed > 0.0f) {
        f32 dot = MR::normalize(1.0f - mMoveVec.dot(zDir), 0.1f, 1.0f);
        f32 sign;
        if (mMoveVec.dot(xDir) >= 0.0f) {
            sign = 1.0f;
        } else {
            sign = -1.0f;
        }
        dot = sign * dot;
        mMarioRotateYAngle += dot * 5.0f;
    }
    mMarioRotateYAngle *= 0.9f;

    TVec3f up;
    up.set(-mGravity);
    mMarioRotateUp.set(-mGravity);

    TVec3f v6;
    PSVECCrossProduct(&mMoveVec, &mGravity, &v6);

    if (!MR::normalizeOrZero(&v6)) {
        MR::rotateVecDegree(&up, v6, mMoveSpeed * 35.0f);
        MR::rotateVecDegree(&mMarioRotateUp, v6, mMoveSpeed * 20.0f);
    }
    MR::vecBlendSphere(mDirectionToMario, up, &mDirectionToMario, 0.02f);

    if (MR::normalizeOrZero(&mDirectionToMario)) {
        mDirectionToMario.set(-mGravity);
    }
    MR::blendQuatUpFront(&mRotateQuat, mMarioRotateUp, mMarioRotateFront, 0.1f, 0.25f);

    f32 rotY = mMarioRotateYAngle;
    MR::setPlayerUpperRotateY(0.017453292f * rotY);  // sin(1.0f)
}

void Tamakoro::updateMarioOffset() {
    f32 decay = (mMarioOffsetVelocity - 1.0f);
    decay *= 0.99f;

    mMarioOffsetVelocity = decay;
    mMarioOffset += decay;
    mMarioOffset = MR::clamp(mMarioOffset, 0.0f, 150.0f);
}

void Tamakoro::updateMoveBckBlend(f32 pSpeed) {
    f32 w0 = 0.0f;
    f32 w1 = 0.0f;
    f32 w2 = 0.0f;
    f32 w3 = 0.0f;
    f32 rate = 0.0f;
    f32 limit1 = 0.25f;
    f32 limit2 = 6.0f;
    f32 limit3 = 18.0f;

    if (pSpeed < limit1) {
        w1 = MR::normalize(pSpeed, 0.0f, 0.25f);
        w0 = 1.0f - w1;
        rate = MR::getLinerValue(w1, 1.0f, 1.0f, 1.0f);
    } else if (pSpeed < limit2) {
        w2 = MR::normalize(pSpeed, 0.25, 6.0f);
        w1 = 1.0f - w2;
        rate = MR::getLinerValue(w2, 1.0f, 2.0f, 1.0f);
    } else if (pSpeed < limit3) {
        w3 = MR::normalize(pSpeed, 6.0f, 18.0f);
        w2 = 1.0f - w3;
        rate = MR::getLinerValue(w3, 2.0f, 3.0f, 1.0f);
    } else {
        w3 = 1.0f;
        rate = (pSpeed / 18.0f) * 3.0f;
        if (rate > 4.0f) {
            rate = 4.0f;
        }
    }

    MR::setBckBlendWeight(w0, w1, w2, w3);
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
    f32 mag = PSVECMag(&mMoment);

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
