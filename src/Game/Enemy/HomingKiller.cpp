#include "Game/Enemy/HomingKiller.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/PartsModel.hpp"

namespace {
    static const Vec cBodyHitSensorOffset = {0.0f, 0.0f, -10.0f};

    const u32 cSensorTableTarget[] = {
        ATYPE_KILLER_TARGET_ENEMY, ATYPE_KILLER_TARGET_MAPOBJ, ATYPE_BREAKABLE_CAGE, ATYPE_ROCK, ATYPE_WANWAN, ATYPE_PUNCH_BOX};
    const u32 cSensorTableAttackIfBinded[] = {ATYPE_KILLER_TARGET_ENEMY,   ATYPE_KILLER_TARGET_MAPOBJ,        ATYPE_BREAKABLE_CAGE, ATYPE_PUNCH_BOX,
                                              ATYPE_TRIPODBOSS_GUARD_WALL, ATYPE_TRIPODBOSS_KILLER_GENERATER, ATYPE_MAP_OBJ};
    const u32 cSensorTableTryExplosion[] = {ATYPE_KILLER_TARGET_ENEMY, ATYPE_KILLER_TARGET_MAPOBJ, ATYPE_ROCK, ATYPE_WANWAN, ATYPE_KARIKARI};

    static const f32 cBodyHitSensorRadius = 70.0f;
    static const f32 cEyeHitSensorRadius = 750.0f;
    static const f32 cBinderRadius = 70.0f;
    static const f32 cExplosionDistance = 250.0f;
    static const f32 cShadowRadiusXY = 60.0f;
    static const f32 cShadowRadiusZ = 100.0f;
    static const f32 cStarWandRadius3d = 150.0f;
    static const f32 cSandColumnEmitDot = -0.75f;
    static const s32 cAppearIntervalFrame = 180;
    static const s32 cForceKillFrame = 600;
    static const f32 cCameraShakeDistance = 1000.0f;
    static const f32 cChaseStartAngle = 360.0f;
    static const f32 cChaseSpeed = 12.0f;
    static const f32 cChaseTargetHeight = 200.0f;
    static const f32 cChaseEndDistance = 3000.0f;
    static const f32 cChaseInvalidFrame = 35;  // BUG: typo, should be an s32 :)
    static const f32 cChaseRotateZSpeed = 1.0f;
    static const f32 cChaseRotateZMax = 60.0f;
    static const f32 cChaseRotateZDot = 0.95f;
    static const f32 cChaseHeightMin = 150.0f;
    static const f32 cUpVecRotateSpeed = 2.0f;
    static const f32 cMoveRotateCosineMax = 0.02f;
    static const s32 cFreezeFrame = 20;
    static const f32 cFreezeRumbleSpeed = 75.0f;
    static const f32 cFreezeRumbleWidth = 5.0f;
    static const f32 cFreezeRotateCosineMax = 0.02f;
    static const s32 cAppearMoveFrame = 20;
    static const s32 cAppearRumbleFrame = 22;
    static const s32 cAppearStopFrame = 30;
    static const f32 cAppearMoveDistance = 200.0f;
    static const f32 cAppearRumbleSpeed = 100.0f;
    static const f32 cAppearRumbleWidth = 18.0f;
    static const f32 cChaseSpeedTorpedo = 5.0f;
    static const f32 cChaseTargetHeightTorpedo = 75.0f;
    static const f32 cTorpedoLightClippingRadius = 1000.0f;
    static const f32 cNearClipDistanceMagnum = 400.0f;
    static const f32 cMagnamFlySeDistMin = 500.0f;
    static const f32 cMagnamFlySeDistMax = 1400.0f;
    static const f32 cMagnamFlySePitchMin = 0.9f;
    static const f32 cMagnamFlySePitchMax = 1.8f;
}  // namespace

namespace NrvHomingKiller {
    NEW_NERVE(HomingKillerNrvAppear, HomingKiller, Appear);
    NEW_NERVE(HomingKillerNrvMoveStart, HomingKiller, MoveStart);
    NEW_NERVE(HomingKillerNrvMove, HomingKiller, Move);
    NEW_NERVE(HomingKillerNrvChaseStart, HomingKiller, ChaseStart);
    NEW_NERVE(HomingKillerNrvChase, HomingKiller, Chase);
    NEW_NERVE(HomingKillerNrvFreeze, HomingKiller, Freeze);
    NEW_NERVE(HomingKillerNrvBreak, HomingKiller, Break);
    NEW_NERVE(HomingKillerNrvGoToTarget, HomingKiller, GoToTarget);

    NEW_NERVE(HomingKillerLauncherNrvAppearKiller, HomingKillerLauncher, AppearKiller);
    NEW_NERVE(HomingKillerLauncherNrvDeadKiller, HomingKillerLauncher, DeadKiller);
}  // namespace NrvHomingKiller

// TODO: this replace with something global
f32 toRadian(f32 degree) {
    return PI_180 * degree;
}

namespace {
    bool isSensorType(const HitSensor* pSensor, const u32* pTypes, u32 numTypes) {
        u32 sensorType = pSensor->mType;
        for (u32 idx = 0; idx < numTypes; idx++) {
            if (sensorType == pTypes[idx]) {
                return true;
            }
        }
        return false;
    }
}  // namespace

HomingKiller::HomingKiller(const char* pName)
    : LiveActor(pName), mType(Type_HomingKiller), mChaseStartDist(2000.0f), mChaseEndDist(cChaseEndDistance), mChaseRotateSpeed(0.9997f),
      mChaseStartAngle(cChaseStartAngle), mFront(0.0f, 0.0f, 1.0f), mUp(0.0f, 1.0f, 0.0f), mBasePos(gZeroVec), mBaseFront(0.0f, 0.0f, 1.0f),
      mBaseUp(0.0f, 1.0f, 0.0f), mFreezeTime(0), mFreezePos(gZeroVec), mUnfreezeNerve(nullptr), mChaseInvalidTime(0), mMoveTime(0),
      mTargetSensor(nullptr), mDisableChase(false), mIsLinearShot(false), mUseFullSightAngle(false), mPropeller(nullptr), mTorpedoLight(nullptr) {
    mBaseMtx.identity();
    mEffectMtx.identity();
}

void HomingKiller::init(const JMapInfoIter& rIter) {
    const char* objectName = "HomingKiller";
    if (MR::isValidInfo(rIter)) {
        MR::getObjectName(&objectName, rIter);
        if (MR::isEqualObjectName(rIter, "Torpedo")) {
            mType = Type_Torpedo;
            setName("トーピード");
        } else if (MR::isEqualObjectName(rIter, "MagnumKiller")) {
            mType = Type_MagnumKiller;
            setName("マグナムキラー");
        }
    }

    if (MR::isValidInfo(rIter)) {
        initMapToolInfo(rIter);
    }

    initModelManagerWithAnm(objectName, nullptr, false);

    if (mType == Type_Torpedo) {
        mPropeller = MR::createPartsModelEnemyAndFix(this, "トーピードプロペラ", "TorpedoPropeller", nullptr, TVec3f(0.0f, 0.0f, 0.0f),
                                                     TVec3f(0.0f, 0.0f, 0.0f), nullptr);
        mTorpedoLight = MR::createModelObjMapObj("トーピードライト", "TorpedoLight", getBaseMtx());
        MR::invalidateClipping(mTorpedoLight);
        mTorpedoLight->makeActorDead();
    }

    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);

    initHitSensor(2);
    f32 hitRadius = getScale();
    f32 offsetScale = getScale();

    MR::addHitSensor(this, "body", ATYPE_HOMING_KILLER, 8, hitRadius * cBodyHitSensorRadius, TVec3f(cBodyHitSensorOffset).scaleInline(offsetScale));
    MR::addHitSensorEye(this, "eye", 16, cEyeHitSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));

    f32 binderRadius = getScale();
    initBinder(binderRadius * cBinderRadius, 0.0f, 0);

    MR::onCalcGravity(this);
    if (MR::isValidInfo(rIter)) {
        MR::calcGravity(this);
    }

    initEffectKeeper(0, nullptr, false);

    if (mType == Type_HomingKiller) {
        MR::setEffectHostMtx(this, "SandColumn", mEffectMtx);
    }

    f32 dpdRadius = getScale();
    MR::initStarPointerTarget(this, dpdRadius * cStarWandRadius3d, TVec3f(0.0f, 0.0f, 0.0f));

    if (MR::isValidInfo(rIter)) {
        calcInitPosture();
    }

    initSound(16, false);

    if (mType == Type_MagnumKiller) {
        MR::initShadowFromCSV(this, "Shadow");
    } else {
        f32 scale = getScale();
        MR::initShadowVolumeOval(this, TVec3f(cShadowRadiusXY, cShadowRadiusXY, cShadowRadiusZ).scaleInline2(scale));
        MR::setShadowDropLength(this, nullptr, 2000.0f);
    }

    MR::addToAttributeGroupSearchTurtle(this);
    MR::invalidateClipping(this);
    initNerve(&NrvHomingKiller::HomingKillerNrvAppear::sInstance);
    makeActorDead();
}

void HomingKiller::appear() {
    mPosition.set(mBasePos);
    mRotation.zero();
    mFront.set(mBaseFront);
    mUp.set(mBaseUp);
    mChaseInvalidTime = 0;
    mMoveTime = 0;
    updateBaseMtxNoRotateZ();
    MR::showModel(this);
    MR::offBind(this);
    MR::validateHitSensors(this);
    MR::invalidateShadow(this, nullptr);
    LiveActor::appear();
    setNerve(&NrvHomingKiller::HomingKillerNrvAppear::sInstance);
}

void HomingKiller::appear(const TVec3f& rPos, const TVec3f& rFront) {
    mBasePos.set(rPos);
    mBaseFront.set(rFront);
    MR::calcGravityVector(this, rPos, &mBaseUp, nullptr, 0);
    mBaseUp.negate(mGravity);
    appear();
}

bool HomingKiller::isMoveStart() const {
    return isNerve(&NrvHomingKiller::HomingKillerNrvMoveStart::sInstance) && mMoveTime == 0;
}

void HomingKiller::setChaseStartEndDistance(f32 startDist, f32 endDist) {
    mChaseStartDist = startDist;
    mChaseEndDist = endDist;
}

bool HomingKiller::isInactive() const {
    return isNerve(&NrvHomingKiller::HomingKillerNrvAppear::sInstance) || isNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance);
}

bool HomingKiller::isMoveNormal() const {
    return isNerve(&NrvHomingKiller::HomingKillerNrvMoveStart::sInstance) || isNerve(&NrvHomingKiller::HomingKillerNrvMove::sInstance);
}

bool HomingKiller::isGravityIgnored() const {
    return mIsLinearShot || mUseFullSightAngle;
}

void HomingKiller::control() {
    if (!isInactive()) {
        if (!isValidShowModel()) {
            MR::hideModelAndOnCalcAnimIfShown(this);
            if (mType != Type_Torpedo) {
                MR::deleteEffect(this, "Smoke");
                if (mType == Type_HomingKiller) {
                    MR::deleteEffect(this, "SmokeChase");
                }
            } else {
                MR::deleteEffect(this, "Bubble");
                MR::deleteEffect(this, "BubbleChase");
            }
        } else {
            MR::showModelIfHidden(this);
            if (mType != Type_Torpedo) {
                MR::emitEffect(this, isMoveNormal() ? "Smoke" : "SmokeChase");
            } else {
                MR::emitEffect(this, isMoveNormal() ? "Bubble" : "BubbleChase");
            }
        }

        if (mType == Type_Torpedo) {
            if (MR::isJudgedToClipFrustum(mTorpedoLight->mPosition, cTorpedoLightClippingRadius)) {
                MR::hideModelAndOnCalcAnimIfShown(mTorpedoLight);
            } else {
                MR::showModelIfHidden(mTorpedoLight);
            }
        }
    }

    if (!isNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance)) {
        if (!isGravityIgnored() || isChasing()) {
            TVec3f up;
            up.negate(mGravity);
            if (isNerve(&NrvHomingKiller::HomingKillerNrvChaseStart::sInstance) || isNerve(&NrvHomingKiller::HomingKillerNrvChase::sInstance) ||
                isNerve(&NrvHomingKiller::HomingKillerNrvFreeze::sInstance) || isNerve(&NrvHomingKiller::HomingKillerNrvGoToTarget::sInstance)) {
                MR::turnVecToVecCos(&mUp, mUp.copy(), up, MR::cosDegree(cUpVecRotateSpeed), mFront, cMoveRotateCosineMax);
            } else {
                mUp.set(up);
            }
        }
        updateBaseMtxNoRotateZ();
    }
}

void HomingKiller::calcAndSetBaseMtx() {
    // FIXME: Incorrect register scheduling in inline
    // https://decomp.me/scratch/5jw7T

    TPos3f mtx2;
    TPos3f mtx;
    mtx.setRotateInlineZeroTrans(TVec3f(0.0f, 0.0f, 1.0f), toRadian(mRotation.z));

    mtx2.concat(mBaseMtx, mtx);
    MR::setBaseTRMtx(this, mtx2);
}

void HomingKiller::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance)) {
        return;
    }

    if (pSender == getSensor("eye")) {
        if (!isNerve(&NrvHomingKiller::HomingKillerNrvAppear::sInstance)) {
            if (isNerve(&NrvHomingKiller::HomingKillerNrvChase::sInstance) &&
                isSensorType(pReceiver, cSensorTableTarget, ARRAY_SIZE(cSensorTableTarget))) {
                mTargetSensor = pReceiver;
                setNerve(&NrvHomingKiller::HomingKillerNrvGoToTarget::sInstance);
            }
        }
        return;
    }

    if (isNerve(&NrvHomingKiller::HomingKillerNrvAppear::sInstance)) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (MR::isSensorPlayerOrRide(pReceiver)) {
        if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
            setNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance);
        }
        return;
    }

    if (!pReceiver->isType(ATYPE_KARIKARI) && tryToExplosion(pSender, pReceiver) && mType != Type_MagnumKiller) {
        setNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance);
    }
}

bool HomingKiller::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isInactive()) {
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (MR::isMsgFireBallAttack(msg)) {
        return true;
    }

    if (MR::isMsgJetTurtleAttack(msg) || MR::isMsgInvincibleAttack(msg)) {
        setNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance);
        return true;
    }

    return false;
}

bool HomingKiller::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isInactive()) {
        return false;
    }

    if (mType != Type_MagnumKiller) {
        setNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance);
    }

    return true;
}

bool HomingKiller::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::tryForceKillIfMsgStartPowerStarGet(this, msg);
}

void HomingKiller::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mChaseStartDist);

    s32 turnRadius = 3;
    MR::getJMapInfoArg1NoInit(rIter, &turnRadius);
    mChaseRotateSpeed = 1.0f - turnRadius * 0.0001f;

    MR::getJMapInfoArg2NoInit(rIter, &mIsLinearShot);
    if (mType == Type_MagnumKiller) {
        mIsLinearShot = true;
    }

    MR::getJMapInfoArg3NoInit(rIter, &mUseFullSightAngle);
    if (!mUseFullSightAngle) {
        mChaseStartAngle = cChaseStartAngle / 2;
    }
}

void HomingKiller::calcInitPosture() {
    TPos3f mtx;
    MR::makeMtxRotate(mtx, mRotation);

    mBasePos.set(mPosition);
    mtx.getZDirInline(mBaseFront);

    if (!isGravityIgnored()) {
        MR::vecKillElement(mBaseFront, mGravity, &mBaseFront);
        MR::normalize(&mBaseFront);
    }

    if (!isGravityIgnored()) {
        mBaseUp.negate(mGravity);
    } else {
        mtx.getYDir(mBaseUp);
    }
}

void HomingKiller::updateVelocity() {
    mVelocity.set(mFront);
    mVelocity.scale(mType == Type_Torpedo ? cChaseSpeedTorpedo : cChaseSpeed);
}

void HomingKiller::updateRotateZ(const TVec3f& rDir) {
    TVec3f grav = mGravity;
    TVec3f front;
    TVec3f side;

    bool turn = false;
    if (!MR::isSameDirection(mFront, grav, 0.01f) && !MR::isSameDirection(rDir, grav, 0.01f)) {
        MR::vecKillElement(mFront, grav, &front);
        MR::vecKillElement(rDir, grav, &side);
        if (front.dot(side) < cChaseRotateZDot) {
            turn = true;
        }
    }

    if (turn) {
        TVec3f up;
        up.cross(front, side);
        if (0.0f < up.dot(grav)) {
            mRotation.z = MR::clamp(mRotation.z + cChaseRotateZSpeed, -cChaseRotateZMax, cChaseRotateZMax);
        } else {
            mRotation.z = MR::clamp(mRotation.z - cChaseRotateZSpeed, -cChaseRotateZMax, cChaseRotateZMax);
        }
    } else {
        // TODO: this is likely MR::converge
        f32 angle = mRotation.z;
        if (angle < 0.0f) {
            angle += cChaseRotateZSpeed;
            if (angle > 0.0f) {
                angle = 0.0f;
            }
        } else {
            angle -= cChaseRotateZSpeed;
            if (angle < 0.0f) {
                angle = 0.0f;
            }
        }
        mRotation.z = angle;
    }
}

bool HomingKiller::processMove() {
    mMoveTime++;

    if (tryBindedBreak() || mMoveTime > cForceKillFrame || (isGravityIgnored() && isWaterBreak())) {
        setNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance);
        return false;
    }

    if (!isGravityIgnored()) {
        TVec3f front;
        if (!MR::isSameDirection(mFront, mGravity, 0.01f)) {
            MR::vecKillElement(mFront, mGravity, &front);
            MR::normalize(&front);
        } else {
            front.set(mFront);
        }
        MR::turnVecToVecCos(&mFront, mFront.copy(), front, 0.9995f, mGravity, cMoveRotateCosineMax);
    }

    updateVelocity();
    startMoveLevelSound(false);

    return true;
}

bool HomingKiller::processChase() {
    startMoveLevelSound(true);

    if (tryBindedBreak() || !MR::isNearPlayer(this, mChaseEndDist) || isWaterBreak()) {
        setNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance);
        return false;
    }

    TVec3f target;
    calcFrontVecToTarget(&target);

    if (MR::isShadowProjected(this, nullptr)) {
        TVec3f shadowPos;
        MR::getShadowProjectionPos(this, nullptr, &shadowPos);
        HitSensor* shadowSensor = MR::getShadowProjectedSensor(this, nullptr);
        if (shadowSensor->isType(ATYPE_BREAKABLE_CAGE)) {
            if (MR::isNear(this, shadowPos, cChaseHeightMin * 2)) {
                target.sub(shadowSensor->mPosition, mPosition);
                MR::normalize(&target);
            }
        } else {
            if (MR::isNear(this, shadowPos, cChaseHeightMin)) {
                TVec3f up;
                mBaseMtx.getYDir(up);
                if (!MR::isSameDirection(target, up, 0.01f)) {
                    MR::vecKillElement(target, up, &target);
                    MR::normalize(&target);
                }
            }
        }
    }

    if (isUpdateChaseFrontVec(target)) {
        f32 rotateSpeed = mChaseRotateSpeed;
        MR::turnVecToVecCos(&mFront, mFront.copy(), target, rotateSpeed, mGravity, cMoveRotateCosineMax);
    } else {
        target.set(mFront);
    }

    updateVelocity();
    updateRotateZ(target);
    return true;
}

bool HomingKiller::isChaseStart() const {
    if (mChaseInvalidTime < cChaseInvalidFrame || MR::isHiddenModel(this) || !MR::isNearPlayer(this, mChaseStartDist)) {
        return false;
    }

    TVec3f target;
    target.sub(*MR::getPlayerPos(), mPosition);
    if (MR::normalizeOrZero(&target)) {
        return false;
    }

    return MR::cosDegree(mChaseStartAngle * 0.5f) < target.dot(mFront);
}

bool HomingKiller::tryChaseStart() {
    mChaseInvalidTime++;
    if (cChaseInvalidFrame == mChaseInvalidTime) {
        MR::onBind(this);
    }

    if (mDisableChase || mIsLinearShot) {
        return false;
    }

    if (isChaseStart()) {
        MR::deleteEffect(this, mType == Type_Torpedo ? "Bubble" : "Smoke");
        setNerve(&NrvHomingKiller::HomingKillerNrvChaseStart::sInstance);
        return true;
    }

    return false;
}

bool HomingKiller::tryFreeze(const Nerve* pUnfreezeNerve) {
    if (mType == Type_MagnumKiller) {
        return false;
    }

    if (MR::isNoBind(this)) {
        return false;
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        mFreezeTime = 0;
        mFreezePos.set(mPosition);
        mUnfreezeNerve = pUnfreezeNerve;
        setNerve(&NrvHomingKiller::HomingKillerNrvFreeze::sInstance);
        return true;
    }

    return false;
}

bool HomingKiller::tryBindedBreak() {
    if (MR::isBinded(this)) {
        if (MR::isBindedGround(this)) {
            HitSensor* groundSensor = MR::getGroundSensor(this);
            if (isSensorType(groundSensor, cSensorTableAttackIfBinded, ARRAY_SIZEU(cSensorTableAttackIfBinded))) {
                MR::sendMsgEnemyAttackExplosion(groundSensor, getSensor("body"));
            }
        }

        if (MR::isBindedWall(this)) {
            HitSensor* wallSensor = MR::getWallSensor(this);
            if (isSensorType(wallSensor, cSensorTableAttackIfBinded, ARRAY_SIZEU(cSensorTableAttackIfBinded))) {
                MR::sendMsgEnemyAttackExplosion(wallSensor, getSensor("body"));
            }
        }

        if (MR::isBindedRoof(this)) {
            HitSensor* roofSensor = MR::getRoofSensor(this);
            if (isSensorType(roofSensor, cSensorTableAttackIfBinded, ARRAY_SIZEU(cSensorTableAttackIfBinded))) {
                MR::sendMsgEnemyAttackExplosion(roofSensor, getSensor("body"));
            }
        }

        MR::offBind(this);
        return true;
    }

    return false;
}

bool HomingKiller::tryToExplosion(HitSensor* pSender, HitSensor* pReceiver) {
    if (isSensorType(pReceiver, cSensorTableTryExplosion, ARRAY_SIZEU(cSensorTableTryExplosion)) || MR::isSensorEnemy(pReceiver)) {
        if (MR::sendMsgEnemyAttackExplosion(pReceiver, pSender)) {
            return true;
        }
    }

    return false;
}

void HomingKiller::sendMsgExplosionToNearActor() {
    HitSensor* receiver;
    HitSensor* eyeSensor = getSensor("eye");

    for (s32 idx = 0; idx < eyeSensor->mSensorCount; idx++) {
        receiver = eyeSensor->mSensors[idx];
        if (MR::isNear(eyeSensor, receiver, cExplosionDistance)) {
            tryToExplosion(eyeSensor, receiver);
        }
    }
}

bool HomingKiller::isUpdateChaseFrontVec(const TVec3f& rFront) const {
    if (!isNerve(&NrvHomingKiller::HomingKillerNrvGoToTarget::sInstance)) {
        return true;
    }

    TVec3f target;
    target.sub(mTargetSensor->mPosition, mPosition);
    if (MR::normalizeOrZero(&target)) {
        return false;
    }

    return target.dot(mFront) < target.dot(rFront);
}

void HomingKiller::calcFrontVecToTarget(TVec3f* pFront) const {
    // FIXME: TVec3 stack order
    // https://decomp.me/scratch/EdWwc

    TVec3f playerUp;
    MR::getPlayerUpVec(&playerUp);
    MR::normalize(&playerUp);
    playerUp.scale(mType == Type_Torpedo ? cChaseTargetHeightTorpedo : cChaseTargetHeight);
    TVec3f target;
    target.sub(MR::getPlayerPos()->addOtherInline(playerUp), mPosition);
    MR::normalize(target, pFront);
}

bool HomingKiller::isValidShowModel() const {
    if (MR::isJudgedToClipFrustum(mPosition, 300.0f)) {
        return false;
    }

    if (MR::isJudgedToNearClip(mPosition, mType == Type_MagnumKiller ? cNearClipDistanceMagnum : 200.0f)) {
        return false;
    }

    return true;
}

bool HomingKiller::isWaterBreak() const {
    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        return mType != Type_Torpedo;
    } else {
        return mType == Type_Torpedo;
    }
}

void HomingKiller::updateBaseMtxNoRotateZ() {
    if (MR::isSameDirection(mFront, mUp, 0.01f)) {
        mBaseMtx.setTransInline(mPosition);
    } else {
        MR::makeMtxFrontUpPos(&mBaseMtx, mFront, mUp, mPosition);
    }
}

void HomingKiller::startMoveLevelSound(bool playAlarm) {
    if (mType == Type_Torpedo) {
        MR::startLevelSound(this, "SE_EM_LV_TORPEDO_MOVE", -1, -1, -1);
        if (playAlarm) {
            MR::startLevelSound(this, "SE_EM_LV_TORPEDO_ALARM", MR::calcDistanceToPlayer(this), -1, -1);
        }
    } else if (mType == Type_MagnumKiller) {
        f32 pitch = MR::getLinerValueFromMinMax(MR::calcDistanceToPlayer(this), cMagnamFlySeDistMin, cMagnamFlySeDistMax, cMagnamFlySePitchMax,
                                                cMagnamFlySePitchMin);
        MR::startLevelSound(this, "SE_EM_LV_MAGKILLER_FLY", pitch * 100.0f, -1, -1);
    } else {
        MR::startLevelSound(this, "SE_EM_LV_KILLER_FLY", -1, -1, -1);
    }
}

void HomingKiller::setBckRate(f32 rate, bool setPropellerRate) {
    if (mType != Type_MagnumKiller) {
        MR::setBckRate(this, rate);
        if (setPropellerRate && mType == Type_Torpedo) {
            MR::setBckRate(mPropeller, rate);
        }
    }
}

bool HomingKiller::isChasing() const {
    if (isNerve(&NrvHomingKiller::HomingKillerNrvChaseStart::sInstance) || isNerve(&NrvHomingKiller::HomingKillerNrvChase::sInstance) ||
        isNerve(&NrvHomingKiller::HomingKillerNrvGoToTarget::sInstance)) {
        return true;
    }

    if (isNerve(&NrvHomingKiller::HomingKillerNrvFreeze::sInstance)) {
        if (mUnfreezeNerve == &NrvHomingKiller::HomingKillerNrvChaseStart::sInstance ||
            mUnfreezeNerve == &NrvHomingKiller::HomingKillerNrvChase::sInstance ||
            mUnfreezeNerve == &NrvHomingKiller::HomingKillerNrvGoToTarget::sInstance) {
            return true;
        }
    }

    return false;
}

void HomingKiller::exeAppear() {
    // FIXME: type check comparison count leading zeros "optimization"
    // https://decomp.me/scratch/lM12w

    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Start", nullptr);
        if (mType == Type_MagnumKiller) {  // FIXME
            MR::startBpk(this, "Move");
            MR::startBrk(this, "Move");
        }
        if (mType == Type_Torpedo) {  // FIXME
            MR::startBck(mPropeller, "RotateTorpedo", nullptr);
            mTorpedoLight->appear();
            MR::startBck(mTorpedoLight, "Appear", nullptr);
        }
        setBckRate(0.0f, true);
    }

    s32 step = getNerveStep();
    if (step >= cAppearMoveFrame) {
        step = cAppearMoveFrame;
    }

    f32 appearOffset = step * cAppearMoveDistance / cAppearMoveFrame;
    mPosition.add(mBasePos, mBaseFront.scaleInline(appearOffset));

    if (MR::isLessStep(this, cAppearMoveFrame)) {
        MR::startLevelSound(this, "SE_EM_LV_KILLER_STANDBY", -1, -1, -1);
    }

    if (MR::isStep(this, cAppearMoveFrame)) {
        MR::startSound(this, "SE_EM_KILLER_STANDBY_END", -1, -1);
    }

    if (MR::isGreaterStep(this, cAppearMoveFrame) && MR::isLessStep(this, cAppearMoveFrame + cAppearRumbleFrame)) {
        s32 step = getNerveStep() - cAppearMoveFrame;
        f32 scl = JMASinDegree(MR::repeatDegree(step * cAppearRumbleSpeed));
        f32 rumbleOffset = (cAppearRumbleFrame - step) * (scl * cAppearRumbleWidth) / cAppearRumbleFrame;

        mPosition.addInline(mBaseFront.scaleInline(rumbleOffset));
    }

    mFront.set(mBaseFront);

    if (MR::isStep(this, cAppearMoveFrame + cAppearRumbleFrame + cAppearStopFrame)) {
        setBckRate(1.0f, true);
        MR::emitEffect(this, "Shoot");
        MR::validateShadow(this, nullptr);
        if (mType == Type_MagnumKiller) {  // FIXME
            MR::startSound(this, "SE_EM_MAGKILLER_FIRING", -1, -1);
            setNerve(&NrvHomingKiller::HomingKillerNrvMove::sInstance);
        } else {
            MR::startSound(this, "SE_EM_KILLER_FIRING", -1, -1);
            setNerve(&NrvHomingKiller::HomingKillerNrvMoveStart::sInstance);
        }
    }
}

void HomingKiller::exeMoveStart() {
    if (processMove() && !tryFreeze(&NrvHomingKiller::HomingKillerNrvMoveStart::sInstance) && !tryChaseStart() && MR::isBckStopped(this)) {
        setNerve(&NrvHomingKiller::HomingKillerNrvMove::sInstance);
    }
}

void HomingKiller::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Move", nullptr);
    }

    if (processMove() && !tryFreeze(&NrvHomingKiller::HomingKillerNrvMove::sInstance) && !tryChaseStart()) {
        return;
    }
}

void HomingKiller::exeChaseStart() {
    if (MR::isFirstStep(this) && MR::tryStartBck(this, "ChaseStart", nullptr)) {
        MR::startBpk(this, "Chase");
        MR::startBrk(this, "Chase");
        MR::startSound(this, "SE_EM_KILLER_JET", -1, -1);
    }

    if (processChase() && !tryFreeze(&NrvHomingKiller::HomingKillerNrvChaseStart::sInstance) && MR::isBckStopped(this)) {
        setNerve(&NrvHomingKiller::HomingKillerNrvChase::sInstance);
    }
}

void HomingKiller::exeChase() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Chase", nullptr);
    }

    if (processChase() && !tryFreeze(&NrvHomingKiller::HomingKillerNrvChase::sInstance)) {
        return;
    }
}

void HomingKiller::exeFreeze() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::emitEffect(this, "Touch");
        if (mFreezeTime == 0) {
            MR::startDPDHitSound();
        }
        if (mType != Type_MagnumKiller) {
            MR::setBckRate(this, 0.0f);
        }
    }

    if (tryBindedBreak()) {
        setNerve(&NrvHomingKiller::HomingKillerNrvBreak::sInstance);
        return;
    }

    mFreezeTime++;
    MR::startDPDFreezeLevelSound(this);

    f32 rumbleOffset =
        (MR::cosDegree(MR::repeatDegree(mFreezeTime * cFreezeRumbleSpeed)) * cFreezeRumbleWidth) * (cFreezeFrame - getNerveStep()) / cFreezeFrame;

    TVec3f rumble;
    rumble.set(MR::getCamXdir());
    rumble.scale(rumbleOffset);
    mPosition.add(mFreezePos, rumble);

    if (isChasing()) {
        TVec3f target;
        calcFrontVecToTarget(&target);
        MR::turnVecToVecCos(&mFront, mFront.copy(), target, 0.9997f, mGravity, cFreezeRotateCosineMax);
        updateRotateZ(target);
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvHomingKiller::HomingKillerNrvFreeze::sInstance);
        return;
    }

    if (MR::isStep(this, cFreezeFrame)) {
        mPosition.set(mFreezePos);
        MR::deleteEffect(this, "Touch");
        setBckRate(1.0f, true);
        setNerve(mUnfreezeNerve);
    }
}

void HomingKiller::exeBreak() {
    if (MR::isFirstStep(this)) {
        sendMsgExplosionToNearActor();
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
        MR::offBind(this);
        mVelocity.zero();
        MR::deleteEffectAll(this);

        if (mTorpedoLight != nullptr) {
            mTorpedoLight->kill();
        }

        MR::emitEffect(this, "Explosion");

        if (MR::isBindedGround(this)) {
            MR::calcGravity(this);
            if (mType == Type_HomingKiller) {
                if (!MR::getGroundSensor(this)->isType(ATYPE_BREAKABLE_CAGE)) {
                    if (mGravity.dot(*MR::getGroundNormal(this)) < cSandColumnEmitDot) {
                        MR::emitEffect(this, "SandColumn");

                        TVec3f up;
                        up.negate(mGravity);
                        TVec3f shadowPos;
                        if (MR::isShadowProjected(this, nullptr)) {
                            MR::getShadowProjectionPos(this, nullptr, &shadowPos);
                        } else {
                            shadowPos.set(mPosition);
                        }

                        if (MR::isSameDirection(mFront, up, 0.01f)) {
                            MR::makeMtxUpNoSupportPos(&mEffectMtx, up, shadowPos);
                        } else {
                            MR::makeMtxUpFrontPos(&mEffectMtx, up, mFront, shadowPos);
                        }
                    }
                }
            }
        }

        if (mType == Type_Torpedo) {
            MR::startSound(this, "SE_EM_TORPEDO_EXPLOSION", -1, -1);
            MR::releaseSoundHandle(this, "SE_EM_TORPEDO_EXPLOSION");
        } else {
            MR::startSound(this, "SE_EM_KILLER_EXPLOSION", -1, -1);
            MR::releaseSoundHandle(this, "SE_EM_KILLER_EXPLOSION");
        }
        MR::startRumbleWithShakeCameraWeak(this, "強", "中", cCameraShakeDistance, cCameraShakeDistance * 2);
    }

    if (!MR::isEffectValid(this, "Explosion")) {
        if (mType != Type_HomingKiller || !MR::isEffectValid(this, "SandColumn")) {
            kill();
        }
    }
}

void HomingKiller::exeGoToTarget() {
    startMoveLevelSound(false);

    if (processChase() && !tryFreeze(&NrvHomingKiller::HomingKillerNrvGoToTarget::sInstance)) {
        HitSensor* eyeSensor = getSensor("eye");
        bool foundTarget = false;
        for (s32 idx = 0; idx < eyeSensor->mSensorCount; idx++) {
            if (eyeSensor->mSensors[idx] == mTargetSensor) {
                foundTarget = true;
                break;
            }
        }

        if (!foundTarget) {
            setNerve(&NrvHomingKiller::HomingKillerNrvChase::sInstance);
        }
    }
}

HomingKillerLauncher::HomingKillerLauncher(const char* pName) : LiveActor("ホーミングキラーランチャー"), mKiller(nullptr) {
}

void HomingKillerLauncher::init(const JMapInfoIter& rIter) {
    MR::connectToSceneEnemyMovement(this);
    MR::initDefaultPos(this, rIter);
    MR::needStageSwitchReadAppear(this, rIter);
    MR::invalidateClipping(this);
    initNerve(&NrvHomingKiller::HomingKillerLauncherNrvAppearKiller::sInstance);
    MR::syncStageSwitchAppear(this);
    mKiller = new HomingKiller("ホーミングキラー");
    mKiller->init(rIter);
    makeActorDead();
}

void HomingKillerLauncher::appear() {
    LiveActor::appear();
    setNerve(&NrvHomingKiller::HomingKillerLauncherNrvAppearKiller::sInstance);
}

bool HomingKillerLauncher::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::tryForceKillIfMsgStartPowerStarGet(this, msg);
}

void HomingKillerLauncher::exeAppearKiller() {
    if (MR::isFirstStep(this) && MR::isDead(mKiller)) {
        mKiller->appear();
    }

    if (MR::isDead(mKiller)) {
        setNerve(&NrvHomingKiller::HomingKillerLauncherNrvDeadKiller::sInstance);
    }
}

void HomingKillerLauncher::exeDeadKiller() {
    if (MR::isStep(this, cAppearIntervalFrame)) {
        setNerve(&NrvHomingKiller::HomingKillerLauncherNrvAppearKiller::sInstance);
    }
}
