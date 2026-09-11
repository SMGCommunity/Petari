#include "Game/Enemy/Unizo.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

namespace {
    const f32 sChaseRadius = 1500.0f;
    const f32 sChaseDegree = 90.0f;
    const f32 sChaseCycle = 20.0f;
    const f32 sRollHeightSea = 30.0f;
    const f32 sRollCycle = 60.0f;
    const f32 sAccelerate = 0.1f;
    const f32 sChaseSpeedMax = 4.0f;
    const f32 sSpinSpeedMax = 6.0f;
    const f32 sTotalSpeedMax = 12.0f;
    const f32 sBrakeRate = 0.9f;
    const f32 sBrakeRateChase = 0.96f;
    const f32 sGravityRateSea = 0.15f;
    const f32 sGravityRateLand = 0.8f;
    const f32 sGravityRateGround0 = 0.2f;
    const f32 sGravityRateGround1 = 0.8f;
    const f32 sDriftSpeedRate = 0.02f;
    const f32 sStopSpeed = 0.5f;
    const f32 sReboundFloor = 0.6f;
    const f32 sReboundEnemy = 0.2f;
    const f32 sReboundPlayerV = 5.0f;
    const f32 sReboundPlayerH = 1.0f;
    const f32 sRadiusScale = 1.0f;
    // sBlinkCount
    // sBlinkRandom
    const f32 sJumpHeight = 70.0f;
    // sStartVelocity
    const s32 sCollidePlayerCount = 120;
    const s32 sCollideEnemyCount = 60;
    const s32 sSpinAnimationCount = 50;
    const s32 sRollSoundTime = 20;
    const f32 sRollSoundSpeed = 2.0f;
    const f32 sLandSoundLandSpeed = 2.0f;
    const f32 sWallHitSoundSpeed = 3.0f;
    const s32 sUnizoCollisionSePlayTime = 30;
    const f32 sSurfaceEffectMinLimit = 40.0f;
    const f32 sSurfaceEffectMaxLimit = 200.0f;
    const f32 sSurfaceEffectMaxRunLimit = 280.0f;
    const f32 sFireDownVertical = 22.0f;
    const f32 sFireDownHorizontal = 6.0f;
    const f32 sFireDownAttenuate = 1.0f;
    const f32 sFireDownGravity = 1.1f;
    const s32 sFireDownStarPiece = 34;
    const f32 sDamageFloorVertical = 30.0f;
    const s32 sDamageFloorFrame = 50;
    const s32 sJumpNow = 1;
    const s32 sStarPieceNum = 6;
    const f32 sRollHeightLand = 0.0f;
    const s32 sStartSearch = 0;
    const f32 sSurfaceEffectMinRunLimit = 0.0f;
}  // namespace

namespace NrvUnizo {
    NEW_NERVE(UnizoNrvWait, Unizo, Wait);
    NEW_NERVE(UnizoNrvJump, Unizo, Jump);
    NEW_NERVE(UnizoNrvChase, Unizo, Chase);
    NEW_NERVE(UnizoNrvAttack, Unizo, Attack);
    NEW_NERVE(UnizoNrvCollidePlayer, Unizo, CollidePlayer);
    NEW_NERVE(UnizoNrvCollideEnemy, Unizo, CollideEnemy);
    NEW_NERVE(UnizoNrvBreak, Unizo, Break);
    NEW_NERVE(UnizoNrvFireDown, Unizo, FireDown);
    NEW_NERVE(UnizoNrvJumpDown, Unizo, JumpDown);
    NEW_NERVE_ONEND(UnizoNrvPointing, Unizo, Pointing, Pointing);
}  // namespace NrvUnizo

Unizo::Unizo(const char* pName)
    : LiveActor(pName), mFrame(0), mBlinkCount(0), mBlinkFrame(0), mSpinAnimTimer(0), mRadiusScale(sRadiusScale), mBreakModel(nullptr),
      mAnimScaleController(nullptr), mBindStarPointerState(nullptr), mJumpCount(0), mGravityRate(sGravityRateSea), mRollHeight(sRollHeightSea),
      mRollSoundTimer(0) {
    mType = TypeSea;
    mBaseMtx.identity();
    mRippleMtx.identity();
    mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
}

Unizo::~Unizo() {
}

void Unizo::initType(const JMapInfoIter& rIter) {
    const char* pName = mName;
    if (MR::isEqualStringCase(pName, "ウニゾー")) {
        mType = TypeSea;
    } else if (MR::isEqualStringCase(pName, "陸ウニゾー")) {
        mType = TypeLand;
    } else if (MR::isEqualStringCase(pName, "浅瀬ウニゾー")) {
        mType = TypeShoal;
    }
}

void Unizo::init(const JMapInfoIter& rIter) {
    initType(rIter);
    if (mType == TypeSea) {
        initModelManagerWithAnm("Unizo", nullptr, false);
        mBreakModel = new ModelObj("ウニゾー壊れモデル", "UnizoBreak", nullptr, 18, -2, -2, false);
        mBreakModel->initWithoutIter();
        mBreakModel->makeActorDead();
    } else if (mType == TypeLand) {
        mRollHeight = sRollHeightLand;
        mGravityRate = sGravityRateLand;
        initModelManagerWithAnm("UnizoLand", nullptr, false);
        mBreakModel = new ModelObj("陸ウニゾー壊れモデル", "UnizoLandBreak", nullptr, 18, -2, -2, false);
        mBreakModel->initWithoutIter();
        mBreakModel->makeActorDead();
        MR::initFur(this);
    } else if (mType == TypeShoal) {
        initModelManagerWithAnm("UnizoShoal", nullptr, false);
        mBreakModel = new ModelObj("浅瀬ウニゾー壊れモデル", "UnizoShoalBreak", nullptr, 18, -2, -2, false);
        mBreakModel->initWithoutIter();
        mBreakModel->makeActorDead();
    }

    MR::initDefaultPos(this, rIter);
    MR::makeMtxTR(mBaseMtx, this);
    mRadiusScale = sRadiusScale;
    mScale.set< f32 >(sRadiusScale, sRadiusScale, sRadiusScale);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(1);
    MR::addHitSensor(this, "Body", 0x24, 8, 115.2f * mRadiusScale, TVec3f(0.0f, 126.36f * mRadiusScale, 0.0f));
    MR::initStarPointerTarget(this, 115.2f * mRadiusScale, TVec3f(0.0f, 126.36f * mRadiusScale, 0.0f));
    initBinder(126.36f * mRadiusScale, 126.36f * mRadiusScale, 8);
    MR::initShadowVolumeSphere(this, 106.36f * mRadiusScale);
    MR::initShadowVolumeSphere(mBreakModel, 106.36f * mRadiusScale);
    initEffectKeeper(4, nullptr, false);
    if (mType == TypeShoal) {
        MR::setEffectHostMtx(this, "Ripple", mRippleMtx);
    }

    initSound(4, false);
    initNerve(GET_NERVE(Unizo, UnizoNrvWait));
    MR::onCalcGravity(this);
    MR::startBtp(this, "Blink");
    mBlinkFrame = MR::getRandom(100L, 200L);
    MR::addToAttributeGroupSearchTurtle(this);
    MR::declareStarPiece(this, sStarPieceNum);
    mAnimScaleController = new AnimScaleController(nullptr);
    mAnimScaleController->setParamTight();
    mBindStarPointerState = new WalkerStateBindStarPointer(this, mAnimScaleController);
    MR::setGroupClipping(this, rIter, 16);
    makeActorAppeared();
    MR::useStageSwitchSleep(this, rIter);
}

void Unizo::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void Unizo::control() {
    mFrame++;
    if (mSpinAnimTimer > 0) {
        mSpinAnimTimer--;
    }

    if (!MR::isNearZero(mVelocity)) {
        updateRotate();
    }

    MR::turnMtxToYDirDegree(&mBaseMtx, -mGravity, 1.0f);
    mBaseMtx.setTrans(mPosition);
    mAnimScaleController->update();
    updateSurfaceEffect();
    tryPointBind();
}

void Unizo::appear() {
    setNerve(GET_NERVE(Unizo, UnizoNrvWait));
    LiveActor::appear();
    mJumpCount = sJumpNow;
    MR::validateShadowAll(this);
    MR::validateShadowAll(mBreakModel);
}

void Unizo::makeActorAppeared() {
    MR::showModel(this);
    mBaseMtx.setTrans(mPosition);
    LiveActor::makeActorAppeared();
    f32 rotX = mRotation.x;
    f32 rotY = mRotation.y;
    f32 rotZ = mRotation.z;
    TQuat4f quatX;
    quatX.setEulerX(rotX);
    TQuat4f quatY;
    quatY.setEulerY(rotY);
    TQuat4f quatZ;
    quatZ.setEulerZ(rotZ);
    mQuat.mult(quatZ, quatX);
    mQuat.mult(mQuat, quatY);
    mQuat.normalize();
    mRotation.zero();
    mModelManager->calcView();
    mModelManager->calcView();
}

void Unizo::kill() {
    makeActorDead();
}

void Unizo::makeActorDead() {
    MR::validateHitSensors(this);
    MR::validateClipping(this);
    MR::validateClipping(mBreakModel);
    mBreakModel->makeActorDead();
    LiveActor::makeActorDead();
}

void Unizo::startClipped() {
    LiveActor::startClipped();
    deleteEffect();
}

void Unizo::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.identity();
    mtx.setQuat(mQuat);
    TVec3f position(mPosition);
    TVec3f quatUp;
    mtx.getYDir(quatUp);
    JMAVECScaleAdd(&quatUp, &position, &position, -(126.36f * mRadiusScale));
    TVec3f baseUp;
    mBaseMtx.getYDir(baseUp);
    JMAVECScaleAdd(&baseUp, &position, &position, 126.36f * mRadiusScale * mAnimScaleController->_C.y);

    f32 rollHeight = mRadiusScale * (mRollHeight * __fabsf(MR::sin(mFrame / sRollCycle)));
    TVec3f horizontalVelocity;
    const TVec3f& velocity = mVelocity;
    const TVec3f& gravity = mGravity;
    JMAVECScaleAdd(&gravity, &velocity, &horizontalVelocity, -gravity.dot(velocity));
    f32 rate = horizontalVelocity.length() / sChaseSpeedMax;
    rollHeight *= rate >= 1.0f ? 1.0f : rate;
    JMAVECScaleAdd(&mGravity, &position, &position, rollHeight);
    MR::scaleMtxToLocalMtx(mtx, mBaseMtx, mAnimScaleController->_C);
    mtx.setTrans(position);
    MR::setBaseTRMtx(this, mtx);
}

void Unizo::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBtp(this, "Normal");
    }

    udpateBlink();
    udpateInfluence();
    if (mJumpCount == 0 && MR::isNearPlayer(this, sChaseRadius) && !MR::isPlayerDamaging() && MR::isBindedGround(this)) {
        doJump();
        setNerve(GET_NERVE(Unizo, UnizoNrvJump));
    }
}

void Unizo::exeJump() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isStep(this, sStartSearch)) {
        MR::startBck(this, "Search", nullptr);
        MR::startBtp(this, "Angry");
        MR::startSound(this, "SE_EM_UNIZO_NEEDLE");
    }

    if (MR::isGreaterEqualStep(this, sChaseCycle)) {
        updateVelocity();
    }
    udpateInfluence();
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Unizo, UnizoNrvChase));
    }
}

void Unizo::exeChase() {
    updateVelocity();
    udpateInfluence();
    if (mType == TypeLand) {
        MR::emitEffect(this, "SearchSmoke");
    }

    if (!MR::isNearPlayer(this, sChaseRadius)) {
        deleteEffect();
        setNerve(GET_NERVE(Unizo, UnizoNrvWait));
    }
}

void Unizo::exeAttack() {
    udpateInfluence();
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
        MR::startSound(this, "SE_EM_UNIZO_NEEDLE");
        MR::startSound(this, "SE_EM_UNIZO_STICK");
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Unizo, UnizoNrvCollidePlayer));
    }
}

void Unizo::exeCollidePlayer() {
    udpateInfluence();
    if (MR::isGreaterStep(this, sCollidePlayerCount)) {
        setNerve(GET_NERVE(Unizo, UnizoNrvChase));
    }
}

void Unizo::exeCollideEnemy() {
    if (MR::isFirstStep(this)) {
        mBlinkCount = MR::isBtpStopped(this) ? 2 : 0;
    }

    udpateInfluence();
    if (MR::isBtpStopped(this) && mBlinkCount > 0) {
        mBlinkCount--;
        MR::startBtp(this, "Blink");
    }

    if (MR::isGreaterStep(this, sCollideEnemyCount)) {
        MR::startBtp(this, "Angry");
        setNerve(GET_NERVE(Unizo, UnizoNrvChase));
    }
}

void Unizo::exeBreak() {
    if (MR::isDead(mBreakModel) || (MR::isBckStopped(mBreakModel) && MR::isBrkStopped(mBreakModel))) {
        kill();
    }
}

void Unizo::exeFireDown() {
    MR::addVelocityToGravity(this, sFireDownGravity);
    if (MR::isBindedGround(this)) {
        MR::attenuateVelocity(this, sFireDownAttenuate);
    }
    updateRotate();
    calcAndSetBaseMtx();
    mBreakModel->mPosition.set(mPosition);

    if (MR::isStep(this, sFireDownStarPiece)) {
        if (MR::appearStarPiece(this, mBreakModel->mPosition, sStarPieceNum, 10.0f, 40.0f, true)) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        }
    }

    if (MR::isDead(mBreakModel) || (MR::isBckStopped(mBreakModel) && MR::isBrkStopped(mBreakModel))) {
        kill();
    }
    if (MR::isGreaterEqualStep(this, sDamageFloorFrame)) {
        kill();
    }
}

void Unizo::exeJumpDown() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_UNIZO_BREAK");
    }

    MR::addVelocityToGravity(this, sFireDownGravity);
    if (MR::isBindedGround(this)) {
        MR::attenuateVelocity(this, sFireDownAttenuate);
    }
    updateRotate();
    calcAndSetBaseMtx();
    mBreakModel->mPosition.set(mPosition);

    if (MR::isDead(mBreakModel) || (MR::isBckStopped(mBreakModel) && MR::isBrkStopped(mBreakModel))) {
        kill();
    }
    if (MR::isGreaterEqualStep(this, sDamageFloorFrame)) {
        kill();
    }
}

void Unizo::exePointing() {
    MR::updateActorStateAndNextNerve(this, mBindStarPointerState, GET_NERVE(Unizo, UnizoNrvWait));
}

void Unizo::endPointing() {
    mBindStarPointerState->kill();
}

void Unizo::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isBreakNow()) {
        return;
    }

    if (MR::isSensorPlayerOrRide(pReceiver)) {
        if (!isNerve(GET_NERVE(Unizo, UnizoNrvAttack)) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            if (!isBreakNow()) {
                doAttack(pReceiver);
            }
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    } else if (MR::isSensorEnemy(pReceiver)) {
        if (MR::sendMsgEnemyAttack(pReceiver, pSender) && !isNerve(GET_NERVE(Unizo, UnizoNrvCollideEnemy))) {
            setNerve(GET_NERVE(Unizo, UnizoNrvCollideEnemy));
        }
    }
}

bool Unizo::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        mAnimScaleController->startHitReaction();
        return true;
    }
    if (isBreakNow()) {
        return false;
    }

    if (MR::isMsgFireBallAttack(msg)) {
        if (mType == TypeSea) {
            doBreak();
        } else {
            TVec3f direction = pReceiver->mPosition - *MR::getPlayerPos();
            MR::normalizeOrZero(&direction);
            doFireDown(direction);
        }
        return true;
    }
    if (MR::isMsgJetTurtleAttack(msg)) {
        doBreak();
        return true;
    }
    return false;
}

bool Unizo::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorType(pSender, 0x24)) {
        TVec3f direction = pReceiver->mPosition - pSender->mPosition;
        f32 distance = direction.length();
        MR::normalizeOrZero(&direction);
        MR::addVelocityMoveToDirection(this, direction, sReboundEnemy * distance);
        startWallHitSound();
        if (!MR::isBckPlaying(this, "Shock")) {
            MR::startBck(this, "Shock", nullptr);
        }
    } else {
        doBreak();
    }
    return true;
}

bool Unizo::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgSpinStormRange(msg)) {
        doSpin();
        return true;
    }
    return false;
}

void Unizo::updateRotate() {
    MR::rotateQuatRollBall(&mQuat, mVelocity, -mGravity, 126.36f * mRadiusScale);
}

void Unizo::updateVelocity() {
    TVec3f direction = *MR::getPlayerPos() - mPosition;
    f32 chaseRate = MR::sin(mFrame / sChaseCycle);
    MR::vecKillElement(direction, mGravity, &direction);
    MR::normalizeOrZero(&direction);
    f32 speedRate = mVelocity.length() / sChaseSpeedMax;
    chaseRate *= speedRate >= 1.0f ? 1.0f : speedRate;
    TPos3f rotateMtx;
    f32 angle = (PI / 180.0f) * (sChaseDegree * chaseRate / 2.0f);
    rotateMtx.makeRotate(mGravity, angle);
    rotateMtx.mult(direction, direction);

    if (direction.dot(mVelocity) > 0.0f) {
        if (mVelocity.length() < sChaseSpeedMax) {
            MR::addVelocityMoveToDirection(this, direction, sAccelerate);
        }
    } else {
        MR::addVelocityMoveToDirection(this, direction, sAccelerate);
    }
}

void Unizo::udpateInfluence() {
    bool rebound;
    if (mJumpCount != 0) {
        MR::addVelocityToGravity(this, mGravityRate);
        rebound = MR::reboundVelocityFromCollision(this, sReboundFloor, 0.0f, 1.0f);
    } else {
        rebound = MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
        if (MR::isBindedGround(this)) {
            MR::addVelocityToGravity(this, sGravityRateGround0);
        } else {
            MR::addVelocityToGravity(this, sGravityRateGround1);
        }
    }

    MR::restrictVelocity(this, sTotalSpeedMax);
    if (isBreakGround()) {
        doJumpDown();
        return;
    }

    if (rebound) {
        f32 verticalSpeed = -mGravity.dot(mVelocity);
        if (verticalSpeed >= sLandSoundLandSpeed) {
            if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                MR::startLevelSound(this, "SE_EM_LV_UNIZO_LAND_WATER", 100.0f * verticalSpeed, 30);
            } else {
                MR::startLevelSound(this, "SE_EM_LV_UNIZO_LAND", 100.0f * verticalSpeed, 30);
            }
        }
        if (MR::isBindedWall(this)) {
            startWallHitSound();
        }
    }

    if (MR::isBindedGround(this)) {
        if (isNerve(GET_NERVE(Unizo, UnizoNrvCollidePlayer)) || isNerve(GET_NERVE(Unizo, UnizoNrvCollideEnemy)) ||
            isNerve(GET_NERVE(Unizo, UnizoNrvWait))) {
            MR::attenuateVelocity(this, sBrakeRate);
            if (mVelocity.length() < sStopSpeed) {
                mVelocity.set(TVec3f(0.0f, 0.0f, 0.0f));
            }
        } else {
            MR::attenuateVelocity(this, sBrakeRateChase);
        }
        if (mJumpCount != 0) {
            mJumpCount--;
        }
    }

    if (MR::isBindedGround(this)) {
        mRollSoundTimer = sRollSoundTime;
    }
    if (mRollSoundTimer > 0) {
        f32 speed = mVelocity.length();
        if (speed >= sRollSoundSpeed) {
            if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                MR::startLevelSound(this, "SE_EM_LV_UNIZO_ROLL_WATER", 100.0f * speed);
            } else {
                MR::startLevelSound(this, "SE_EM_LV_UNIZO_ROLL_GROUND", 100.0f * speed);
            }
        }
        mRollSoundTimer--;
    }
}

void Unizo::udpateBlink() {
    if (mBlinkFrame == getNerveStep() % 200) {
        MR::startBtp(this, "Blink");
    }
}

void Unizo::updateSurfaceEffect() {
    if (mType == TypeShoal) {
        WaterInfo waterInfo;
        if (!MR::getWaterAreaObj(&waterInfo, mPosition)) {
            MR::deleteEffect(this, "Ripple");
            return;
        }
        MR::getWaterAreaInfo(&waterInfo, mPosition, mGravity, false);
        if (!waterInfo.isInWater()) {
            MR::deleteEffect(this, "Ripple");
            return;
        }

        f32 speed = mVelocity.length();
        f32 depth = waterInfo.mCamWaterDepth;
        f32 minDepth;
        if (speed > 5.0f) {
            minDepth = sSurfaceEffectMinRunLimit;
        } else {
            minDepth = sSurfaceEffectMinLimit;
        }
        f32 maxDepth;
        if (speed > 5.0f) {
            maxDepth = sSurfaceEffectMaxRunLimit;
        } else {
            maxDepth = sSurfaceEffectMaxLimit;
        }
        if (depth < minDepth || depth > maxDepth) {
            MR::deleteEffect(this, "Ripple");
            return;
        }

        mRippleMtx.set(mBaseMtx);
        MR::setMtxTrans(mRippleMtx, waterInfo.mSurfacePos.x, waterInfo.mSurfacePos.y, waterInfo.mSurfacePos.z);
        MR::emitEffect(this, "Ripple");
    }
}

void Unizo::deleteEffect() {
    if (mType == TypeLand) {
        MR::deleteEffect(this, "SearchSmoke");
    } else if (mType == TypeShoal) {
        MR::deleteEffect(this, "Ripple");
    }
}

void Unizo::doJump() {
    f32 gravity = mGravityRate * mGravity.length();
    mJumpCount = sJumpNow;
    f32 speed = MR::sqrt(2.0f * (mRadiusScale * (sJumpHeight * gravity)));
    mVelocity.add(mGravity * speed * -1.0f);
}

void Unizo::doAttack(HitSensor* pSensor) {
    TVec3f direction = mPosition - pSensor->mHost->mPosition;
    TVec3f up = -mGravity;
    MR::normalizeOrZero(&direction);
    MR::normalizeOrZero(&up);
    mJumpCount = sJumpNow;
    mVelocity.set(up * sReboundPlayerV + direction * sReboundPlayerH);
    setNerve(GET_NERVE(Unizo, UnizoNrvAttack));
}

void Unizo::doJumpDown() {
    appearBreakModel();
    if (mType == TypeShoal) {
        MR::startBck(mBreakModel, "Firedown", nullptr);
        MR::startBrk(mBreakModel, "Break");
    } else if (mType == TypeLand) {
        MR::startBck(mBreakModel, "Firedown", nullptr);
        MR::startBrk(mBreakModel, "FireDown");
    }
    mVelocity.set(mGravity * -sDamageFloorVertical);
    setNerve(GET_NERVE(Unizo, UnizoNrvJumpDown));
}

void Unizo::doFireDown(const TVec3f& rDirection) {
    appearBreakModel();
    if (mType == TypeShoal) {
        if (MR::appearStarPiece(this, mPosition, sStarPieceNum, 10.0f, 40.0f, true)) {
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        }
        MR::startBck(mBreakModel, "Firedown", nullptr);
        MR::startBrk(mBreakModel, "Break");
        MR::startSound(this, "SE_EM_UNIZO_BREAK");
        MR::shakeCameraNormal();
        mBreakModel->appear();
        setNerve(GET_NERVE(Unizo, UnizoNrvBreak));
    } else {
        if (mType == TypeLand) {
            MR::startBck(mBreakModel, "Firedown", nullptr);
            MR::startBrk(mBreakModel, "FireDown");
        }
        MR::zeroVelocity(this);
        MR::setVelocitySeparateHV(this, rDirection, sFireDownHorizontal, sFireDownVertical);
        setNerve(GET_NERVE(Unizo, UnizoNrvFireDown));
    }
}

void Unizo::doBreak() {
    bool appeared;
    if (mType == TypeSea) {
        appeared = MR::appearStarPiece(this, mPosition, sStarPieceNum, 10.0f, 40.0f, false);
    } else {
        appeared = MR::appearStarPiece(this, mPosition, sStarPieceNum, 10.0f, 40.0f, true);
    }
    if (appeared) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    }
    MR::startSound(this, "SE_EM_UNIZO_BREAK");
    appearBreakModel();
    if (mType == TypeSea) {
        MR::shakeCameraNormal();
        MR::startBck(mBreakModel, "Break", nullptr);
        MR::startBrk(mBreakModel, "Break");
        MR::emitEffect(this, "Death");
    } else if (mType == TypeShoal) {
        MR::shakeCameraNormal();
        MR::startBck(mBreakModel, "Break", nullptr);
        MR::startBrk(mBreakModel, "Break");
    } else if (mType == TypeLand) {
        MR::startBck(mBreakModel, "FireDown", nullptr);
        MR::startBrk(mBreakModel, "FireDown");
    }
    mBreakModel->appear();
    setNerve(GET_NERVE(Unizo, UnizoNrvBreak));
}

void Unizo::doSpin() {
    f32 speed = mVelocity.length();
    TVec3f pullVelocity;
    MR::calcPlayerSpinPullVelocity(&pullVelocity, mPosition);
    MR::vecKillElement(pullVelocity, mGravity, &pullVelocity);
    mVelocity.add(pullVelocity * sDriftSpeedRate);
    if (speed > sSpinSpeedMax) {
        mVelocity.setLength(speed);
    }
    if (mSpinAnimTimer == 0 && !MR::isBckPlaying(this, "Shock")) {
        mSpinAnimTimer = sSpinAnimationCount;
        MR::startBck(this, "Shock", nullptr);
    }
}

bool Unizo::isBreakGround() {
    for (u32 i = 0; i < mBinder->mPlaneNum; i++) {
        const Triangle* pPlane = &mBinder->getPlane(i)->mParentTriangle;
        if (MR::isGroundCodeDamage(pPlane) || MR::isGroundCodeDamageFire(pPlane)) {
            return true;
        }
    }
    return false;
}

bool Unizo::isBreakNow() const {
    if (isNerve(GET_NERVE(Unizo, UnizoNrvBreak)) || isNerve(GET_NERVE(Unizo, UnizoNrvFireDown)) || isNerve(GET_NERVE(Unizo, UnizoNrvJumpDown))) {
        return true;
    }
    return false;
}

bool Unizo::tryPointBind() {
    if (isEnablePointBind() && mBindStarPointerState->tryStartPointBind()) {
        setNerve(GET_NERVE(Unizo, UnizoNrvPointing));
        return true;
    }
    return false;
}

bool Unizo::isEnablePointBind() const {
    if (isNerve(GET_NERVE(Unizo, UnizoNrvWait)) || isNerve(GET_NERVE(Unizo, UnizoNrvJump)) || isNerve(GET_NERVE(Unizo, UnizoNrvChase)) ||
        isNerve(GET_NERVE(Unizo, UnizoNrvAttack)) || isNerve(GET_NERVE(Unizo, UnizoNrvCollidePlayer)) ||
        isNerve(GET_NERVE(Unizo, UnizoNrvCollideEnemy))) {
        return true;
    }
    return false;
}

void Unizo::appearBreakModel() {
    TPos3f mtx;
    TVec3f rotation;
    MR::makeMtxFrontUp(&mtx, -MR::getCamZdir(), MR::getCamYdir());
    mtx.getEuler(rotation);
    f32 x = rotation.x;
    f32 y = rotation.y;
    f32 z = rotation.z;
    rotation.set(_180_PI * x, _180_PI * y, _180_PI * z);
    mBreakModel->mPosition.set(mPosition);
    mBreakModel->mScale.set(mScale);
    mBreakModel->mRotation.set(rotation);
    mBreakModel->appear();
    MR::hideModel(this);
    MR::invalidateHitSensors(this);
    MR::invalidateClipping(mBreakModel);
    MR::invalidateClipping(this);
    MR::invalidateShadowAll(this);
    MR::invalidateShadowAll(mBreakModel);
}

void Unizo::startWallHitSound() {
    f32 speed = mVelocity.length();
    if (speed >= sWallHitSoundSpeed) {
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startLevelSound(this, "SE_EM_LV_UNIZO_COLLISION_WATER", 100.0f * speed, sUnizoCollisionSePlayTime);
        } else {
            MR::startLevelSound(this, "SE_EM_LV_UNIZO_COLLISION", 100.0f * speed, sUnizoCollisionSePlayTime);
        }
    }
}

MtxPtr Unizo::getBaseMtx() const {
    return const_cast< TPos3f& >(mBaseMtx).toMtxPtr();
}
