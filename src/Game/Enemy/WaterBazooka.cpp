#include "Game/Enemy/WaterBazooka.hpp"
#include "Game/Camera/CameraTargetDemoActor.hpp"
#include "Game/Enemy/ElectricPressureBullet.hpp"
#include "Game/Enemy/MogucchiShooter.hpp"
#include "Game/Enemy/WaterBazookaCapsule.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/WaterPressureBulletHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <math_types.hpp>
#include <revolution/mtx.h>

namespace NrvWaterBazooka {
    NEW_NERVE(WaterBazookaNrvWaitForBattle, WaterBazooka, WaitForBattle);
    NEW_NERVE(WaterBazookaNrvWait, WaterBazooka, Wait);
    NEW_NERVE(WaterBazookaNrvAim, WaterBazooka, Aim);
    NEW_NERVE(WaterBazookaNrvAimEnd, WaterBazooka, AimEnd);
    NEW_NERVE(WaterBazookaNrvShot, WaterBazooka, Shot);
    NEW_NERVE(WaterBazookaNrvShotNoMotion, WaterBazooka, ShotNoMotion);
    NEW_NERVE(WaterBazookaNrvTire, WaterBazooka, Tire);
    NEW_NERVE(WaterBazookaNrvDemoCrackCapsule, WaterBazooka, DemoCrackCapsule);
    NEW_NERVE(WaterBazookaNrvDemoAnger, WaterBazooka, DemoAnger);
    NEW_NERVE(WaterBazookaNrvDemoBreakWait, WaterBazooka, DemoBreakWait);
    NEW_NERVE(WaterBazookaNrvDemoBreakSign, WaterBazooka, DemoBreakSign);
    NEW_NERVE(WaterBazookaNrvDemoBreakExplosion, WaterBazooka, DemoBreakExplosion);
    NEW_NERVE(WaterBazookaNrvDemoBreakEnd, WaterBazooka, DemoBreakEnd);
    NEW_NERVE(WaterBazookaNrvWaitForLaugh, WaterBazooka, WaitForLaugh);
    NEW_NERVE(WaterBazookaNrvPanic, WaterBazooka, Panic);
    NEW_NERVE_ONEND(WaterBazookaNrvStorm, WaterBazooka, Storm, Storm);
}  // namespace NrvWaterBazooka

namespace {
    static const f32 sElectricBulletSpeed = 12.0f;
    // FIXME: sDropPointStringTable should end up in .rodata
    const char* sDropPointStringTable[] = {"落下点1", "落下点2", "落下点3", "落下点4"};

}  // namespace

WaterBazooka::WaterBazooka(const char* pName)
    : LiveActor(pName), mShooter(nullptr), mCapsule(nullptr), mBreakModel(nullptr), mDemoActor(nullptr), mCameraInfo(nullptr), mAlreadyDoneFlag(-1),
      mLife(3), mShotNum(0), mCannonCollisionParts(nullptr), mJointCtrl(nullptr), mIsPinchBGMStarted(false), mHasPowerStar(false), mIsElectric(false),
      mBullets(nullptr), mElectricTime(-1) {
    mBaseMtx.identity();
}

void WaterBazooka::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    const char* objectName;
    MR::getObjectName(&objectName, rIter);
    mIsElectric = MR::isEqualString("ElectricBazooka", objectName);
    initModelManagerWithAnm(objectName, nullptr, false);

    MR::connectToSceneMapObjStrongLight(this);

    initHitSensor(2);
    MR::addBodyMessageSensorMapObj(this);
    MR::addMessageSensorEnemy(this, "cannon");

    MR::initCollisionParts(this, "Cannon1", getSensor("cannon"), MR::getJointMtx(this, "Cannon1"));
    mCannonCollisionParts = MR::createCollisionPartsFromLiveActor(this, "AllRoot", getSensor("body"), static_cast< MR::CollisionScaleType >(2));
    MR::validateCollisionParts(mCannonCollisionParts);

    initEffectKeeper(1, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);

    initSound(4, false);
    MR::invalidateClipping(this);

    MR::initActorCamera(this, rIter, &mCameraInfo);

    mJointCtrl = MR::createJointDelegatorWithNullChildFunc(this, &WaterBazooka::calcJointCannon, "Cannon1");
    MR::initJointTransform(this);
    JMath::gekko_ps_copy12(&mBaseMtx, MR::getJointMtx(this, "Cannon1"));

    initShooter();
    initBazookaCapsule();
    initBreakModel();
    initCameraTarget();
    initBullet(rIter);

    MR::getJMapInfoArg0NoInit(rIter, &mHasPowerStar);
    if (mHasPowerStar) {
        MR::declarePowerStar(this);
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mShooter, rIter);
        MR::tryRegisterDemoCast(mCapsule, rIter);
        MR::tryRegisterDemoCast(mDemoActor, rIter);
        MR::tryRegisterDemoCast(mBreakModel, rIter);
    }

    MR::startBrk(this, "Damage");
    MR::setBrkFrameAndStop(this, 0.0f);
    MR::declareStarPiece(this, 24);
    MR::addToAttributeGroupSearchTurtle(this);

    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);

    initNerve(&NrvWaterBazooka::WaterBazookaNrvWait::sInstance);

    if (MR::isEqualStageName("OceanPhantomCaveGalaxy")) {
        u32 a1 = 0;
        mAlreadyDoneFlag = MR::setupAlreadyDoneFlag("ウォータバズーカ撃破", rIter, &a1);

        if (a1 != 0) {
            if (MR::isValidSwitchDead(this)) {
                MR::onSwitchDead(this);
            }
            makeActorDead();
            return;
        }
    }

    makeActorAppeared();
}

bool WaterBazooka::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (pReceiver == getSensor("body")) {
        return false;
    }

    if (MR::isDemoActive()) {
        return false;
    }

    if (isNrvDemo()) {
        return false;
    }

    if (MR::isMsgPlayerHipDropFloor(msg) && pReceiver->mType == ATYPE_WATER_BAZOOKA_CAPSULE) {
        if (isElectricLeak() && (!MR::isPlayerDamaging() || MR::isPlayerParalyzing())) {
            return false;
        }

        damageBazooka(pSender, pReceiver);
        return true;
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        damageBazooka(pSender, pReceiver);
        return true;
    }

    return false;
}

bool WaterBazooka::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("body")) {
        return false;
    }

    if (MR::isDemoActive()) {
        return false;
    }

    if (isNrvDemo()) {
        return false;
    }

    if (pSender->mType == ATYPE_COCO_NUT) {
        damageBazooka(pSender, pReceiver);
        return true;
    }

    return false;
}

bool WaterBazooka::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pSender)) {
        return false;
    }

    if (pReceiver == getSensor("body")) {
        return false;
    }

    if (MR::isDemoActive()) {
        return false;
    }

    if (isNrvDemo()) {
        return false;
    }

    if (MR::isMsgFloorTouch(msg) || MR::isMsgWallTouch(msg)) {
        if (isElectricLeak() && (!MR::isPlayerHipDropFalling() && !MR::isPlayerHipDropLand())) {
            MR::sendMsgEnemyAttackElectric(pSender, pReceiver);
            return true;
        }

        if (isNerve(&NrvWaterBazooka::WaterBazookaNrvStorm::sInstance)) {
            MR::sendMsgEnemyAttackFlip(pSender, pReceiver);
            return true;
        }

        return false;
    }

    return false;
}

bool WaterBazooka::isFirstShoot() const {
    if (MR::isFirstStep(this)) {
        return isNerve(&NrvWaterBazooka::WaterBazookaNrvShot::sInstance);
    }
    return false;
}

bool WaterBazooka::isTired() const {
    return isNerve(&NrvWaterBazooka::WaterBazookaNrvTire::sInstance);
}

bool WaterBazooka::isPanic() const {
    return isNerve(&NrvWaterBazooka::WaterBazookaNrvPanic::sInstance) || isNerve(&NrvWaterBazooka::WaterBazookaNrvStorm::sInstance);
}

bool WaterBazooka::isBazookaPinch() const {
    return mLife <= 1;
}

bool WaterBazooka::isBazookaLifeOut() const {
    return mLife <= 0;
}

void WaterBazooka::exeWaitForBattle() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
    }

    JMath::gekko_ps_copy12(&mBaseMtx, MR::getJointMtx(this, "Cannon1"));

    bool start = false;
    if (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this) || MR::isStageStateScenarioOpeningCamera()) {
        start = true;
    }

    if (start) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWait::sInstance);
    }
}

void WaterBazooka::exeWait() {
    if (MR::isFirstStep(this) && isBazookaPinch() && !MR::isBrkPlaying(this, "PowerUp")) {
        MR::startBrk(this, "PowerUp");
    }

    if (mShooter->isLaughed()) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWaitForLaugh::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, 0) && !MR::isPlayerHidden()) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvAim::sInstance);
        return;
    }

    if (tryWaitForBattle()) {
    }
}

void WaterBazooka::exeAim() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "ShotStart", nullptr);
    }

    MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_TURN", -1, -1, -1);

    if (mIsElectric && MR::isGreaterEqualStep(this, 10)) {
        MR::startLevelSound(this, "SE_EM_LV_ELECBAZ_PRE_SHOOT", -1, -1, -1);
    }

    if (aimAtMario()) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvAimEnd::sInstance);
        return;
    }

    if (MR::isGreaterEqualStep(this, 60)) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvAimEnd::sInstance);
    }
}

void WaterBazooka::exeAimEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwingStop", nullptr);
        MR::startSound(this, "SE_EM_WATERBAZ_TURN_END", -1, -1);
    }

    if (mIsElectric) {
        MR::startLevelSound(this, "SE_EM_LV_ELECBAZ_PRE_SHOOT", -1, -1, -1);
    }

    if (MR::isBckStopped(this)) {
        if (isBazookaPinch() && !mIsElectric) {
            setNerve(&NrvWaterBazooka::WaterBazookaNrvShotNoMotion::sInstance);
        } else {
            setNerve(&NrvWaterBazooka::WaterBazookaNrvShot::sInstance);
        }
    }
}

void WaterBazooka::exeShot() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shot", nullptr);
        mShotNum++;
    }

    if (mIsElectric) {
        MR::startLevelSound(this, "SE_EM_LV_ELECBAZ_PRE_SHOOT", -1, -1, -1);
    }

    if (MR::isStep(this, 30)) {
        MR::startSound(this, "SE_EM_WATERBAZ_SHOT", -1, -1);
        tryShotBullet();
    }

    if (MR::isBckStopped(this)) {
        if (mShotNum >= 3) {
            mShotNum = 0;
            setNerve(&NrvWaterBazooka::WaterBazookaNrvTire::sInstance);
            return;
        }

        if (isBazookaPinch() && !mIsElectric) {
            setNerve(&NrvWaterBazooka::WaterBazookaNrvShotNoMotion::sInstance);
        } else {
            setNerve(&NrvWaterBazooka::WaterBazookaNrvWaitForLaugh::sInstance);
        }
    }
}

void WaterBazooka::exeShotNoMotion() {
    if (MR::isFirstStep(this)) {
        mShotNum++;
        MR::startBck(this, "ShortShot", nullptr);
        MR::startSound(this, "SE_EM_WATERBAZ_SHOT", -1, -1);
        tryShotBullet();
    }

    aimAtMario();

    if (mShooter->isLaughed()) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWaitForLaugh::sInstance);
        return;
    }

    if (MR::isStep(this, 30)) {
        if (mShotNum >= 5) {
            mShotNum = 0;
            setNerve(&NrvWaterBazooka::WaterBazookaNrvTire::sInstance);
            return;
        }

        setNerve(&NrvWaterBazooka::WaterBazookaNrvShotNoMotion::sInstance);
    }
}

void WaterBazooka::exeTire() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Tire", nullptr);
    }

    if (mShooter->isLaughed()) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWaitForLaugh::sInstance);
        return;
    }

    if (MR::isStep(this, 120)) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWait::sInstance);
    }
}

void WaterBazooka::exeDemoCrackCapsule() {
    if (MR::isFirstStep(this)) {
        tryJumpBackPlayerFromBazooka();
        MR::startBck(this, "1stDamage", nullptr);
        mShooter->hitShock();
    }

    if (MR::isStep(this, 27)) {
        MR::startSound(this, "SE_EV_MOGUBAZ_ANGRY1", -1, -1);
    }

    if (!MR::isDemoActiveRegistered(this)) {
        TVec3f starPieceSpawnPos;
        TVec3f up;
        MR::calcUpVec(&up, this);
        JMAVECScaleAdd(&up, &mPosition, &starPieceSpawnPos, 600.0f);
        MR::appearStarPiece(this, starPieceSpawnPos, 8, 25.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        MR::validateCollisionParts(mCapsule);
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWait::sInstance);
    }
}

void WaterBazooka::exeDemoAnger() {
    if (MR::isFirstStep(this)) {
        tryJumpBackPlayerFromBazooka();
        MR::startBck(this, "2ndDamage", nullptr);
        mShooter->anger();
        startBrk("PowerUp");
    }

    if (MR::isStep(this, 20)) {
        MR::startSound(this, "SE_EV_MOGUBAZ_ANGRY2", -1, -1);
    }

    if (mIsElectric) {
        if (MR::isFirstStep(this)) {
            MR::emitEffect(this, "AngrySign");
        } else if (MR::isStep(this, 60)) {
            MR::deleteEffect(this, "AngrySign");
        } else if (MR::isStep(this, 90)) {
            MR::emitEffect(this, "Angry");
        }

        if (MR::isLessStep(this, 60)) {
            MR::startLevelSound(this, "SE_EM_LV_ELECBAZ_PRE_ELEC_LEAK", -1, -1, -1);
        } else if (MR::isGreaterEqualStep(this, 90)) {
            MR::startLevelSound(this, "SE_EM_LV_ELECBAZ_ELEC_LEAK", -1, -1, -1);
        }
    }

    if (!MR::isDemoActiveRegistered(this)) {
        if (mIsElectric) {
            MR::deleteEffect(this, "Angry");
            mElectricTime = 0;
        }

        TVec3f starPieceSpawnPos;
        TVec3f up;
        MR::calcUpVec(&up, this);
        JMAVECScaleAdd(&up, &mPosition, &starPieceSpawnPos, 600.0f);
        MR::appearStarPiece(this, starPieceSpawnPos, 16, 25.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        MR::validateCollisionParts(mCapsule);
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWait::sInstance);
    }
}

void WaterBazooka::exeDemoBreakWait() {
    if (MR::isFirstStep(this)) {
        MR::offUpdateCollisionParts(this);
        if (MR::isPlayerInBind()) {
            MR::endBindAndPlayerJumpWithRollLanding(this, *MR::getPlayerVelocity(), 0);
        }
    }

    if (MR::isStep(this, 5)) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakSign::sInstance);
    }
}

void WaterBazooka::exeDemoBreakSign() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down", nullptr);
        tryJumpBackPlayerFromBazooka();
        if (mIsElectric) {
            MR::deleteEffect(this, "Angry");
            MR::deleteEffect(this, "AngrySign");
        }
    }

    MR::tryRumblePadWeak(this, WPAD_CHAN0);
    MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_PRE_EXPLODE", -1, -1, -1);
    if (MR::isGreaterEqualStep(this, 120)) {
        MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_BREAK_FLASH", -1, -1, -1);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakExplosion::sInstance);
    }
}

void WaterBazooka::exeDemoBreakExplosion() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::invalidateCollisionParts(this);
        MR::invalidateCollisionParts(mCannonCollisionParts);
        MR::invalidateHitSensors(this);
        MR::deleteEffect(this, "DamageSmokeS1");
        MR::deleteEffect(this, "DamageSmokeS2");
        MR::deleteEffect(this, "DamageSmokeL1");
        MR::deleteEffect(this, "DamageSmokeL2");
        mShooter->_99 = true;
        mShooter->explosion();
        mCapsule->kill();
        mBreakModel->makeActorAppeared();
        MR::invalidateClipping(mBreakModel);
        MR::startBck(mBreakModel, "Break", nullptr);
        MR::startBrk(mBreakModel, "Break");
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
        MR::startSound(this, "SE_EM_WATERBAZ_EXPLODE", -1, -1);
        MR::startSound(this, "SE_EV_MOGUBAZ_DEAD", -1, -1);
    }

    if (MR::isStep(this, 53)) {
        if (mHasPowerStar) {
            TVec3f spawnPos;
            MR::copyJointPos(this, "Cockpit", &spawnPos);
            MR::startAfterBossBGM();
            MR::requestAppearPowerStar(this, spawnPos);
        } else {
            MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
        }
    }

    if (MR::isBckStopped(mBreakModel)) {
        mBreakModel->kill();
        setNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakEnd::sInstance);
    }
}

void WaterBazooka::exeDemoBreakEnd() {
    if (MR::isStep(this, 60)) {
        if (MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }
        kill();
    }
}

void WaterBazooka::exeWaitForLaugh() {
    if (mShooter->isLaughed()) {
        mShotNum = 0;
    } else {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWait::sInstance);
    }
}

void WaterBazooka::exePanic() {
    // FIXME: r30, r31 regswap
    // https://decomp.me/scratch/9cP5w
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
        mShotNum = 0;
    }

    aimAtMario();

    bool playerOn = false;
    if (MR::isOnPlayer(getSensor("cannon")) || mCapsule->isPlayerOnCapsule()) {
        playerOn = true;
    }

    if (!playerOn && MR::isOnGroundPlayer()) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWait::sInstance);
        return;
    }

    if (MR::isStep(this, 180)) {
        mShooter->stormStart();
        startBrk("Spin");
        MR::startSound(this, "SE_EV_MOGUBAZ_PRE_STORM", -1, -1);
    }

    if (MR::isGreaterEqualStep(this, 180)) {
        MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_PRE_STORM", -1, -1, -1);
    }

    if (MR::isStep(this, 300)) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvStorm::sInstance);
    }
}

void WaterBazooka::exeStorm() {
    if (MR::isFirstStep(this)) {
        mShooter->storm();
        MR::emitEffect(this, "Spin");
    }

    TVec3f pos;
    TVec3f side;
    mBaseMtx.getTrans(pos);
    mBaseMtx.getXDir(side);

    f32 deg = MR::calcNerveEaseOutValue(this, 150, 25.0f, 0.0f);
    MR::rotateVecDegree(&side, mGravity, deg);
    TVec3f up;
    JGeometry::negateInternal(&mGravity.x, &up.x);
    MR::makeMtxSideUpPos(&mBaseMtx, side, up, pos);

    MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_STORM", (deg * 100.0f) / 25.0f, -1, -1);

    if (MR::isStep(this, 90)) {
        MR::deleteEffect(this, "Spin");
    }

    if (MR::isStep(this, 150)) {
        setNerve(&NrvWaterBazooka::WaterBazookaNrvWait::sInstance);
    }
}

void WaterBazooka::endStorm() {
    if (MR::isEffectValid(this, "Spin")) {
        MR::deleteEffect(this, "Spin");
    }
}

void WaterBazooka::makeActorDead() {
    mShooter->makeActorDead();
    mCapsule->makeActorDead();
    mBreakModel->makeActorDead();
    mDemoActor->makeActorDead();
    mCannonCollisionParts->_CC = false;
    LiveActor::makeActorDead();
}

void WaterBazooka::kill() {
    mDemoActor->kill();
    if (mAlreadyDoneFlag >= 0) {
        MR::updateAlreadyDoneFlag(mAlreadyDoneFlag, 1);
    }
    LiveActor::kill();
}

void WaterBazooka::control() {
    setCameraTargetMtx();
    switchShowOrHide();

    if (!MR::isStageStateScenarioOpeningCamera()) {
        if (MR::isEqualStageName("OceanPhantomCaveGalaxy")) {
            if (MR::isExecScenarioStarter()) {
                MR::playLevelMarioPinchBGM(mIsPinchBGMStarted);
                mIsPinchBGMStarted = true;
            } else {
                if (MR::isNearPlayer(this, 5000.0f)) {
                    MR::playLevelMarioPinchBGM(mIsPinchBGMStarted);
                    mIsPinchBGMStarted = true;
                }
            }
        }
    }

    updateElectricLeak();
    if (getSmokeLevel() != -1) {
        MR::startLevelSound(this, "SE_EM_LV_WATERBAZ_SMOKE", -1, -1, -1);
    }
    tryPanic();
}

void WaterBazooka::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    mJointCtrl->registerCallBack();
}

void WaterBazooka::startDemoCrackCapsule() {
    MR::requestStartDemoRegisteredMarioPuppetable(this, nullptr, nullptr, "カプセル破壊");
    mCapsule->crackCapsule();
    MR::invalidateCollisionParts(mCapsule);
    setNerve(&NrvWaterBazooka::WaterBazookaNrvDemoCrackCapsule::sInstance);
}

void WaterBazooka::startDemoAnger() {
    if (MR::isDemoPartExist(this, "乗組員怒り")) {
        MR::requestStartDemoRegisteredMarioPuppetable(this, nullptr, nullptr, "乗組員怒り");
    }
    mCapsule->crackCapsule();
    MR::invalidateCollisionParts(mCapsule);
    setNerve(&NrvWaterBazooka::WaterBazookaNrvDemoAnger::sInstance);
}

void WaterBazooka::startDemoBreakCapsule() {
    MR::requestStartDemoRegisteredMarioPuppetable(this, nullptr, nullptr, "完全破壊");
    mShooter->panicDeath();
    mCapsule->breakCapsule();
    MR::invalidateCollisionParts(mCapsule);
    setNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakWait::sInstance);
}

bool WaterBazooka::aimAtMario() {
    // FIXME: instruction order swap in MR::turnVecToVecCos register loads
    // https://decomp.me/scratch/juRcM
    if (MR::isStageStateScenarioOpeningCamera()) {
        return true;
    }

    TVec3f side;
    TVec3f cannonPos;
    mBaseMtx.getXDir(side);
    mBaseMtx.getTrans(cannonPos);

    TVec3f aimPos;
    aimPos.scaleAdd(mGravity.negateOperatorInternal(), *MR::getPlayerPos(), 100.0f);

    TVec3f aim;
    aim.sub(aimPos, cannonPos);
    MR::normalize(&aim);
    MR::turnVecToVecCos(&side, side, aim, JMath::sSinCosTable.cosLap(1.2f), mGravity, 0.02f);

    TVec3f v1;
    MR::turnVecToPlane(&v1, side, mGravity.negateOperatorInternal());
    MR::clampVecAngleDeg(&side, v1, 15.0f);

    if (side.dot(mGravity.negateOperatorInternal()) > 0.0f) {
        MR::turnVecToPlane(&side, side, mGravity.negateOperatorInternal());
    }

    MR::makeMtxSideUpPos(&mBaseMtx, side, mGravity.negateOperatorInternal(), cannonPos);

    TVec3f side2;
    mBaseMtx.getXDir(side2);
    f32 angle = JMath::sAtanTable.atan2_(side2.cross(aim).length(), side2.dot(aim));
    return __fabsf(angle) * _180_PI <= 2.0f;
}

void WaterBazooka::switchShowOrHide() {
    if (isNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakExplosion::sInstance) ||
        isNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakEnd::sInstance)) {
        MR::hideModel(this);
        return;
    }

    if (MR::isJudgedToClipFrustum(mPosition, 800.0f)) {
        if (!MR::isHiddenModel(this)) {
            MR::hideModel(this);
        }
    } else {
        if (MR::isHiddenModel(this)) {
            MR::showModel(this);
        }
    }
}

void WaterBazooka::updateElectricLeak() {
    if (mIsElectric && mElectricTime >= 0 && !isNrvDemo() && isBazookaPinch()) {
        if (isElectricLeakSign() && !MR::isEffectValid(this, "AngrySign")) {
            MR::emitEffect(this, "AngrySign");
            MR::deleteEffect(this, "Angry");
        } else if (isElectricLeak() && !MR::isEffectValid(this, "Angry")) {
            MR::emitEffect(this, "Angry");
            MR::deleteEffect(this, "AngrySign");
        }

        if (isElectricLeakSign()) {
            MR::startLevelSound(this, "SE_EM_LV_ELECBAZ_PRE_ELEC_LEAK", -1, -1, -1);
        } else if (isElectricLeak()) {
            MR::startLevelSound(this, "SE_EM_LV_ELECBAZ_ELEC_LEAK", -1, -1, -1);
        }

        if (mElectricTime < 600) {
            mElectricTime++;
        } else {
            mElectricTime = 0;
            MR::deleteEffect(this, "Angry");
        }
    }
}

void WaterBazooka::damageBazooka(HitSensor* pSender, HitSensor* pReceiver) {
    startBrk("Damage");
    MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);

    if (mLife == 3) {
        startDemoCrackCapsule();
        MR::emitEffect(this, "DamageSmokeS1");
    } else if (mLife == 2) {
        startDemoAnger();
        MR::emitEffect(this, "DamageSmokeS2");
        MR::emitEffect(this, "DamageSmokeL1");
    } else if (mLife == 1) {
        startDemoBreakCapsule();
        MR::emitEffect(this, "DamageSmokeL2");
    }

    mLife--;
}

bool WaterBazooka::calcJointCannon(TPos3f* pMtx, const JointControllerInfo& rJointInfo) {
    JMath::gekko_ps_copy12(pMtx, &mBaseMtx);
    return true;
}

s32 WaterBazooka::getSmokeLevel() const {
    if (mLife == 4) {
        return 0;
    }

    if (mLife == 3) {
        return 1;
    }

    if (mLife == 2) {
        return 2;
    }

    if (mLife == 1) {
        return 3;
    }

    return -1;
}

bool WaterBazooka::tryWaitForBattle() {
    if (MR::isStageStateScenarioOpeningCamera()) {
        return false;
    }

    if (!MR::isValidSwitchA(this)) {
        return false;
    }

    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        return false;
    }

    setNerve(&NrvWaterBazooka::WaterBazookaNrvWaitForBattle::sInstance);
    return true;
}

bool WaterBazooka::tryPanic() {
    if (isNrvDemo()) {
        return false;
    }

    bool b1 = false;
    if (isNerve(&NrvWaterBazooka::WaterBazookaNrvPanic::sInstance) || isNerve(&NrvWaterBazooka::WaterBazookaNrvStorm::sInstance)) {
        b1 = true;
    }

    if (b1) {
        return false;
    }

    bool playerOn = false;
    if (MR::isOnPlayer(getSensor("cannon")) || mCapsule->isPlayerOnCapsule()) {
        playerOn = true;
    }

    if (!playerOn) {
        return false;
    }

    setNerve(&NrvWaterBazooka::WaterBazookaNrvPanic::sInstance);
    return true;
}

bool WaterBazooka::isNrvDemo() const {
    return isNerve(&NrvWaterBazooka::WaterBazookaNrvDemoCrackCapsule::sInstance) || isNerve(&NrvWaterBazooka::WaterBazookaNrvDemoAnger::sInstance) ||
           isNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakWait::sInstance) || isNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakSign::sInstance) ||
           isNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakExplosion::sInstance) ||
           isNerve(&NrvWaterBazooka::WaterBazookaNrvDemoBreakEnd::sInstance);
}

bool WaterBazooka::isElectricLeakSign() const {
    if (!mIsElectric) {
        return false;
    }

    return mElectricTime >= 300 && mElectricTime < 420;
}

bool WaterBazooka::isElectricLeak() const {
    if (!mIsElectric) {
        return false;
    }

    return mElectricTime >= 420 && mElectricTime < 600;
}

void WaterBazooka::initShooter() {
    mShooter = new MogucchiShooter(this, "ウォーターバズーカ乗組員モグッチ");
    mShooter->initFixedPosition("Cockpit");
    mShooter->initWithoutIter();
}

void WaterBazooka::initBazookaCapsule() {
    mCapsule = new WaterBazookaCapsule(this, "ウォーターバズーカのカプセル");
    mCapsule->initFixedPosition("Top");
    mCapsule->initWithoutIter();
}

void WaterBazooka::initBreakModel() {
    if (mIsElectric) {
        mBreakModel = MR::createModelObjEnemy("エレクトリックバズーカ壊れモデル", "ElectricBazookaBreak", getBaseMtx());
    } else {
        mBreakModel = MR::createModelObjEnemy("ウォーターバズーカ壊れモデル", "WaterBazookaBreak", getBaseMtx());
    }
    mBreakModel->initWithoutIter();
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
}

void WaterBazooka::initCameraTarget() {
    mDemoActor = new CameraTargetDemoActor(MR::getJointMtx(this, "Cockpit"), "バズーカカメラターゲット");
    mDemoActor->initWithoutIter();
    setCameraTargetMtx();
}

void WaterBazooka::initBullet(const JMapInfoIter& rIter) {
    if (mIsElectric) {
        mBullets = new ElectricPressureBullet*[5];

        for (s32 idx = 0; idx < 5; idx++) {
            mBullets[idx] = new ElectricPressureBullet("エレクトリックバズーカの弾");
            mBullets[idx]->initWithoutIter();
            MR::tryRegisterDemoCast(mBullets[idx], rIter);
        }
    } else {
        MR::createSceneObj(SceneObj_WaterPressureBulletHolder);
    }
}

bool WaterBazooka::tryShotBullet() {
    LiveActor* bullet;
    if (mIsElectric) {
        bullet = selectBulletElectric();
    } else {
        bullet =
            reinterpret_cast< WaterPressureBulletHolder* >(MR::getSceneObjHolder()->getObj(SceneObj_WaterPressureBulletHolder))->callEmptyBullet();
    }

    if (bullet == nullptr) {
        return false;
    }

    TPos3f posMtx;
    calcGunPointFromCannon(&posMtx);
    if (mIsElectric) {
        reinterpret_cast< ElectricPressureBullet* >(bullet)->shotElectricBullet(this, posMtx, sElectricBulletSpeed);
    } else {
        reinterpret_cast< WaterPressureBullet* >(bullet)->shotWaterBullet(this, posMtx, 40.0f, true, true, false, &mCameraInfo);
    }

    return true;
}

ElectricPressureBullet* WaterBazooka::selectBulletElectric() {
    for (s32 idx = 0; idx < 5; idx++) {
        if (MR::isDead(mBullets[idx])) {
            return mBullets[idx];
        }
    }
    return nullptr;
}

bool WaterBazooka::tryJumpBackPlayerFromBazooka() const {
    bool playerOn = false;
    if (MR::isOnPlayer(getSensor("cannon")) || mCapsule->isPlayerOnCapsule()) {
        playerOn = true;
    }

    if (!playerOn && !mCapsule->isPlayerOnCapsule()) {
        return false;
    }

    TVec3f dropPoint;
    calcNearDropPoint(&dropPoint);
    TVec3f jumpDir;
    jumpDir.sub(dropPoint, *MR::getPlayerPos());
    const TVec3f& grav = mGravity;
    TVec3f rej;
    rej.rejection(jumpDir, grav);
    TVec3f rej2(rej.negateOperatorInternal());

    MR::offBind(MR::getPlayerDemoActor());
    MR::unlockPlayerAnimation();
    MR::forceFlyPlayer(dropPoint, rej2, MR::calcDistanceToPlayer(dropPoint) / 20.0f);
    return true;
}

void WaterBazooka::calcNearDropPoint(TVec3f* pPos) const {
    // FIXME: rejection register
    // https://decomp.me/scratch/bkPaT

    f32 nearestDistance = FLOAT_MAX;
    TVec3f nearestPoint;
    for (u32 idx = 0; idx < 4; idx++) {
        TVec3f pos;
        if (MR::tryFindNamePos(sDropPointStringTable[idx], &pos, nullptr) && MR::isNearPlayer(pos, nearestDistance)) {
            nearestPoint.set(pos);
            nearestDistance = MR::calcDistanceToPlayer(nearestPoint);
        }
    }

    if (nearestDistance != FLOAT_MAX) {
        pPos->set(nearestPoint);
        return;
    }

    TVec3f toPlayer;
    toPlayer.sub(*MR::getPlayerPos(), mPosition);
    const TVec3f& grav = mGravity;
    toPlayer.rejection(toPlayer, grav);  // possibly uninline
    MR::normalizeOrZero(&toPlayer);

    if (MR::isNearZero(toPlayer)) {
        MR::getPlayerFrontVec(&toPlayer);
        toPlayer.negateInternal();
    }

    pPos->scaleAdd(toPlayer, mPosition, 800.0f);
}

void WaterBazooka::calcGunPointFromCannon(TPos3f* pMtx) {
    TPos3f mtx;
    JMath::gekko_ps_copy12(&mtx, MR::getJointMtx(this, "Cannon1"));
    TVec3f side;
    mtx.getXDir(side);
    TVec3f pos;
    mtx.getTrans(pos);
    JMAVECScaleAdd(&side, &pos, &pos, 550.0f);
    JMath::gekko_ps_copy12(pMtx, &mtx);
    TVec3f up;
    mtx.getYDirInline(up);
    TVec3f front;
    mtx.getZDir(front);
    MR::makeMtxFrontUpPos(pMtx, side, up, pos);
}

void WaterBazooka::setCameraTargetMtx() {
    TPos3f mtx;
    JMath::gekko_ps_copy12(&mtx, mShooter->getBaseMtx());

    mtx.setPos(mPosition);
    TVec3f up;
    MR::calcUpVec(&up, this);
    TVec3f front;
    mtx.getZDir(front);
    MR::makeMtxUpFrontPos(&mtx, up, front, mPosition);
    mDemoActor->setTargetMtx(mtx);
}
