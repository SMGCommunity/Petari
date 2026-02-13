#include "Game/MapObj/Rock.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/MapObj/RockCreator.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <math_types.hpp>

namespace {
    const Vec cWeakSensorOffset = {0.0f, 0.0f, -150.0f};
    static const f32 cBinderRadius = 225.0f;
    static const f32 cSensorRadius = 225.0f;
    static const f32 cGravity = 1.5f;
    static const f32 cWeakGravity = 0.2f;
    static const f32 cWallBreakDot = -0.5f;
    static const f32 cRotateSpeedRate = 1.1f;
    static const f32 cRotateCosineMax = 0.999f;
    static const f32 cRumbleDistance = 1000.0f;
    static const s32 cNoBindJudgeFrame = 20;
    static const f32 cStarWandRadius3d = 275.0f;
    static const s32 cAppearMoveFrame = 45;
    static const s32 cAppearRumbleFrame = 15;
    static const s32 cAppearStopFrame = 25;
    static const s32 cAppearMoveFrameRock = 8;
    static const s32 cAppearStopFrameRock = 4;
    static const s32 cAppearInvalidBindFrame = 45;
    static const f32 cAppearMoveSpeed = 7.0f;
    static const f32 cAppearRumbleSpeed = 100.0f;
    static const f32 cAppearRumbleAngle = 5.0f;
    static const s32 cStarPieceNum = 9;
    static const s32 cStarPieceNumWanwanMini = 1;
    static const f32 cWeakSensorRadius = 125.0f;
    static const s32 cSeWanwanVoiceInterval = 120;
    static const f32 cSeWanwanVoiceDistance = 600.0f;
    static const s32 cRollSoundTime = 20;
    static const s32 cFreezeFrame = 20;
    static const f32 cFreezeRumbleSpeed = 75.0f;
    static const f32 cFreezeRumbleWidth = 5.0f;
    static const f32 cNearClipDistance = 400.0f;
    static const s32 cSlowMoveFrame = 150;
    static const s32 cSlowMoveRumbleFrame = 20;
    static const s32 cBreakStopStep = 2;
    static const s32 cBreakStopFrame = 12;
    static const s32 cAppearPowerStarStep = 2;
};  // namespace

namespace NrvRock {
    NEW_NERVE(RockNrvAppear, Rock, Appear);
    NEW_NERVE(RockNrvAppearMoveInvalidBind, Rock, AppearMoveInvalidBind);
    NEW_NERVE(RockNrvMove, Rock, Move);
    NEW_NERVE(RockNrvMoveInvalidBind, Rock, MoveInvalidBind);
    NEW_NERVE(RockNrvBreak, Rock, Break);
    NEW_NERVE(RockNrvFreeze, Rock, Freeze);
};  // namespace NrvRock

Rock::Rock(f32 moveSpeed, const char* pName)
    : LiveActor(pName), mCreator(nullptr), mRockType(NormalRock), mBreakModel(nullptr), mMoveSpeed(moveSpeed), mBreakModelOnRailGoal(false),
      mSlowDownOnAttack(false), mAppearPos(gZeroVec), mRadius(cBinderRadius), mRotateSpeed(0.0f), mFallVelocity(gZeroVec), mCurrentRailPoint(-1),
      mGravityRate(cGravity), mPrevPos(gZeroVec), mFront(0.0f, 0.0f, 1.0f), mAirTime(0), mIsNormalGravity(false), mWanwanVoiceTimer(0),
      mFreezeTime(0), mFreezePos(gZeroVec), mUnfreezeNerve(nullptr), mInvalidBindTime(0), mAppearTime(cAppearMoveFrameRock), mAppearAngle(0.0f),
      mRollSoundTimer(0), mSlowDownTimer(0) {
    mBaseMtx.identity();
}

void Rock::init(const JMapInfoIter& rIter) {
    // FIXME: load order of offset vec (+ probably not written this way)
    // https://decomp.me/scratch/4hrlO

    mRockType = Rock::getType(rIter);

    if (mRockType != NormalRock) {
        switch (mRockType) {
        case WanwanRolling:
            setName("ワンワン");
            break;
        case WanwanRollingMini:
            setName("ミニワンワン");
            break;
        case WanwanRollingGold:
            setName("ゴールドワンワン");
            MR::declarePowerStar(this);
            break;
        }

        mAppearTime = cAppearMoveFrame;
    }

    initMapToolInfo(rIter);
    initModel();
    MR::connectToSceneNoShadowedMapObjStrongLight(this);
    MR::initLightCtrl(this);
    initSensor();
    initBinder(mRadius, 0.0f, 0);
    initRailRider(rIter);
    initEffect();

    TVec3f offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    MR::initStarPointerTarget(this, cStarWandRadius3d * getRadius(), offset);
    initSound(5, false);

    f32 shadowDrop;
    if (MR::getJMapInfoArg4NoInit(rIter, &shadowDrop)) {
        MR::initShadowVolumeCylinder(this, cBinderRadius * getRadius());
        MR::setShadowDropLength(this, nullptr, shadowDrop);
    } else {
        MR::initShadowVolumeSphere(this, cBinderRadius * getRadius());
    }

    initNerve(&NrvRock::RockNrvAppear::sInstance);
    makeActorDead();
}

void Rock::appear() {
    mFallVelocity.zero();
    mAirTime = 0;
    mGravityRate = cGravity;
    mCurrentRailPoint = -1;
    mInvalidBindTime = 0;
    mPosition.set(mAppearPos);
    mPrevPos.set(mAppearPos);
    mRotation.zero();

    if (!MR::isZeroGravity(this)) {
        MR::calcGravity(this);
    }

    MR::moveCoordAndTransToNearestRailPos(this);
    MR::showModel(this);
    MR::invalidateClipping(this);
    MR::validateHitSensors(this);
    MR::vecKillElement(MR::getRailDirection(this), mGravity, &mFront);
    MR::normalize(&mFront);

    if (mRockType == NormalRock) {
        setBtkForEnvironmentMap(this, "Size");

    } else if (mRockType == WanwanRolling) {
        setBtkForEnvironmentMap(this, "WanwanRolling");
    }

    LiveActor::appear();
    if (!MR::isLoopRail(this)) {
        MR::offBind(this);
        setNerve(&NrvRock::RockNrvAppear::sInstance);
    } else {
        MR::onBind(this);
        setNerve(&NrvRock::RockNrvMove::sInstance);
    }
}

void Rock::kill() {
    LiveActor::kill();

    if (mRockType == WanwanRollingMini) {
        appearStarPiece();
        MR::emitEffect(this, "MiniBreak");
        MR::startSound(this, "SE_EM_WANWANROLLMINI_EXPLOSION", -1, -1);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
}

s32 Rock::getAppearFrame() {
    return cAppearMoveFrame + cAppearRumbleFrame + cAppearStopFrame;
}

Rock::Type Rock::getType(const JMapInfoIter& rIter) {
    if (MR::isEqualObjectName(rIter, "WanwanRolling")) {
        return WanwanRolling;
    }

    if (MR::isEqualObjectName(rIter, "WanwanRollingMini")) {
        return WanwanRollingMini;
    }

    if (MR::isEqualObjectName(rIter, "WanwanRollingGold")) {
        return WanwanRollingGold;
    }

    return NormalRock;
}

s32 Rock::getAppearStarPieceNum(Type type) {
    return (type == WanwanRollingMini) ? cStarPieceNumWanwanMini : cStarPieceNum;
}

bool Rock::isMoveEnabled() const {
    return isNerve(&NrvRock::RockNrvAppear::sInstance) || isNerve(&NrvRock::RockNrvAppearMoveInvalidBind::sInstance) ||
           isNerve(&NrvRock::RockNrvMove::sInstance) || isNerve(&NrvRock::RockNrvMoveInvalidBind::sInstance);
}

void Rock::control() {
    if (isNerve(&NrvRock::RockNrvBreak::sInstance)) {
        return;
    }

    bool updateFront;
    bool hasMoved = false;
    TVec3f front;
    front.subInline(mPosition, mPrevPos);

    if (!MR::isNearZero(front)) {
        MR::normalize(&front);
        hasMoved = true;
        mPrevPos.set(mPosition);
    }

    updateFront = false;
    if (hasMoved && isMoveEnabled()) {
        updateFront = true;
    }

    if (isInClippingRange()) {
        MR::hideModel(this);
        MR::deleteEffect(this, "Smoke");
        if (mRockType == WanwanRollingGold) {
            MR::deleteEffect(this, "Light");
        }

        if (updateFront) {
            mFront.set(front);
        }
    } else {
        MR::showModel(this);
        if (updateFront) {
            TVec3f v2(mFront);
            MR::turnVecToVecCos(&mFront, v2, front, cRotateCosineMax, mGravity, 0.02f);
        }
    }

    if (MR::isOnGround(this)) {
        mFallVelocity.zero();
    }

    if (!isNerve(&NrvRock::RockNrvMove::sInstance)) {
        return;
    }

    MR::vecKillElement(MR::getRailPos(this) - mPosition, mGravity, &mVelocity);
    TVec3f grav(mGravity);
    grav.scale(mGravityRate);
    mFallVelocity.addInline(grav);
    mVelocity.addInline(mFallVelocity);

    if (30.0f < mVelocity.length()) {
        mVelocity.setLength(30.0f);
    }
}

void Rock::calcAndSetBaseMtx() {
    TVec3f grav(mGravity);
    TPos3f mtx;
    calcBaseMtx(&mtx);
    MR::setBaseTRMtx(this, mtx);

    if (isNerve(&NrvRock::RockNrvAppearMoveInvalidBind::sInstance) || (isNerve(&NrvRock::RockNrvMove::sInstance) && MR::isOnGround(this)) ||
        isNerve(&NrvRock::RockNrvMoveInvalidBind::sInstance)) {
        TVec3f up;
        if (isNerve(&NrvRock::RockNrvMove::sInstance)) {
            up.set(*MR::getGroundNormal(this));
        } else {
            up.negateOtherInternal(grav);
        }

        TVec3f pos;
        pos.subInline(mPosition, up.scaleInline(mRadius));

        if (MR::isSameDirection(up, mFront, 0.01f)) {
            MR::makeMtxUpNoSupportPos(&mBaseMtx, up, pos);
        } else {
            MR::makeMtxUpFrontPos(&mBaseMtx, up, mFront, pos);
        }

        mBaseMtx.scaleInline(getRadius());
    }
}

void Rock::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    // smells like inline
    bool b = false;
    if (isBodySensor(pSender) && !isNerve(&NrvRock::RockNrvBreak::sInstance)) {
        b = true;
    }

    if (!b) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver) || MR::isSensorRide(pReceiver)) {
        if (!isNerve(&NrvRock::RockNrvFreeze::sInstance) && !isNerve(&NrvRock::RockNrvAppear::sInstance)) {
            if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
                if (mSlowDownOnAttack && isNerve(&NrvRock::RockNrvMove::sInstance)) {
                    mSlowDownTimer = cSlowMoveFrame;
                }
                rumblePadAndCamera();
                return;
            }

            MR::sendMsgPush(pReceiver, pSender);
            return;
        }

        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (isRockSensor(pReceiver)) {
        if (MR::sendMsgEnemyAttack(pReceiver, pSender) && mRockType != WanwanRollingGold) {
            setNerveBreak(true);
        }
        return;
    }

    if (!isNerve(&NrvRock::RockNrvFreeze::sInstance)) {
        MR::sendMsgToEnemyAttackBlowOrTrample(pReceiver, pSender, 0.25f);
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool Rock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvRock::RockNrvBreak::sInstance)) {
        if (isBodySensor(pReceiver)) {
            if (MR::isMsgInvincibleAttack(msg)) {
                setNerveBreak(true);
                return true;
            }

            if (MR::isMsgStarPieceReflect(msg)) {
                return true;
            }

            if (MR::isMsgFireBallAttack(msg)) {
                return true;
            }
        } else {
            if (mRockType == NormalRock && !isNerve(&NrvRock::RockNrvAppear::sInstance)) {
                if (MR::isMsgPlayerHitAll(msg)) {
                    setNerveBreak(true);
                    return true;
                }
            }
        }
    }

    return false;
}

bool Rock::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    // smells like inline
    bool b = false;
    if (isBodySensor(pReceiver) && !isNerve(&NrvRock::RockNrvBreak::sInstance)) {
        b = true;
    }

    if (b && mRockType != WanwanRollingGold) {
        if (MR::isMsgExplosionAttack(msg) || MR::isMsgToEnemyAttackBlow(msg) || isRockSensor(pSender)) {
            setNerveBreak(true);
            return true;
        }
    }

    return false;
}

void Rock::initMapToolInfo(const JMapInfoIter& rIter) {
    // FIXME: float regswap
    // https://decomp.me/scratch/QfKvF

    MR::initDefaultPos(this, rIter);
    mAppearPos.set(mPosition);

    mRadius = getRadius() * cBinderRadius;
    mRotateSpeed = (mMoveSpeed * 180.0f * cRotateSpeedRate) / (PI * mRadius);  // regswap

    MR::getJMapInfoArg3NoInit(rIter, &mAppearTime);
    if (mRockType == NormalRock) {
        MR::getJMapInfoArg5NoInit(rIter, &mSlowDownOnAttack);
    }

    if (!MR::isZeroGravity(this)) {
        MR::onCalcGravity(this);
    } else {
        TPos3f mtx;
        MR::makeMtxTR(mtx, this);
        mtx.getYDirInline(mGravity);
        mGravity.negateInternal();
    }
}

void Rock::initModel() {
    switch (mRockType) {
    case NormalRock:
        initModelManagerWithAnm("Rock", nullptr, false);
        mBreakModel = MR::createModelObjMapObjStrongLight("ゴロゴロ岩壊れモデル", "RockBreak", nullptr);
        mBreakModel->initWithoutIter();
        mBreakModel->mScale.set(mScale);
        mBreakModel->makeActorDead();
        break;
    case WanwanRolling:
        initModelManagerWithAnm("WanwanRolling", nullptr, false);
        mBreakModel = MR::createModelObjMapObjStrongLight("ワンワン壊れモデル", "WanwanRollingBreak", nullptr);
        mBreakModel->initWithoutIter();
        mBreakModel->mScale.set(mScale);
        mBreakModel->makeActorDead();
        break;
    case WanwanRollingMini:
        initModelManagerWithAnm("WanwanRollingMini", nullptr, false);
        break;
    case WanwanRollingGold:
        initModelManagerWithAnm("WanwanRollingGold", nullptr, false);
        mBreakModel = MR::createModelObjMapObjStrongLight("ゴールドワンワン壊れモデル", "WanwanRollingGoldBreak", nullptr);
        mBreakModel->initWithoutIter();
        mBreakModel->mScale.set(mScale);
        mBreakModel->makeActorDead();
        break;
    }

    if (mBreakModel != nullptr) {
        MR::registerDemoSimpleCastAll(mBreakModel);
    }
}

void Rock::initSensor() {
    // FIXME: extra load of mRockType due to getRadius inline
    // https://decomp.me/scratch/SPLdp

    s32 sensorType;
    if (mRockType == NormalRock) {
        initHitSensor(2);
        sensorType = ATYPE_ROCK;
    } else {
        initHitSensor(1);
        sensorType = ATYPE_WANWAN;
    }

    MR::addHitSensor(this, "body", sensorType, 16, cSensorRadius * getRadius(), TVec3f(0.0f, 0.0f, 0.0f));

    if (mRockType == NormalRock) {
        MR::addHitSensor(this, "weak", sensorType, 16, cWeakSensorRadius * getRadius(),
                         static_cast< TVec3f >(cWeakSensorOffset).scaleInline(getRadius()));
    }
}

void Rock::initEffect() {
    if (mRockType == WanwanRollingMini) {
        initEffectKeeper(0, "WanwanRolling", false);
    } else {
        initEffectKeeper(0, nullptr, false);
    }

    MR::setEffectHostMtx(this, "Smoke", mBaseMtx);
    MR::setEffectHostMtx(this, "Land", mBaseMtx);
}

bool Rock::isBreakByWall() const {
    if (MR::isBindedWall(this)) {
        const TVec3f& dir = MR::getRailDirection(this);
        return dir.dot(*MR::getWallNormal(this)) < cWallBreakDot;
    }

    return false;
}

bool Rock::isBodySensor(HitSensor* pSensor) const {
    return pSensor == getSensor("body");
}

bool Rock::move(f32 speed) {
    s32 curPnt = mCurrentRailPoint;
    MR::moveCoord(this, speed);
    mCurrentRailPoint = MR::getCurrentRailPointNo(this);

    if (curPnt != mCurrentRailPoint) {
        s32 pntArg;
        MR::getCurrentRailPointArg0WithInit(this, &pntArg);

        if (pntArg >= 0) {
            mIsNormalGravity = false;
            mGravityRate = cWeakGravity;
        }

        MR::getCurrentRailPointArg1WithInit(this, &pntArg);

        if (pntArg >= 0) {
            setNerve(&NrvRock::RockNrvMoveInvalidBind::sInstance);
            return false;
        }
    }

    return true;
}

void Rock::calcBaseMtx(TPos3f* pMtx) const {
    TVec3f up;
    up.negateOtherInternal(mGravity);

    if (MR::isSameDirection(up, mFront, 0.01f)) {
        MR::makeMtxUpNoSupportPos(pMtx, up, mPosition);
    } else {
        MR::makeMtxUpFrontPos(pMtx, up, mFront, mPosition);
    }

    TPos3f mtx;
    mtx.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), getAngle());
    pMtx->concat(*pMtx, mtx);
}

bool Rock::isInClippingRange() const {
    if (MR::isJudgedToClipFrustum(mPosition, (300.0f + (250.0f * getRadius())))) {
        return true;
    }

    if (MR::isJudgedToNearClip(mPosition, (cNearClipDistance * getRadius()))) {
        return true;
    }

    return false;
}

void Rock::startSoundWanwanVoice() {
    if (mRockType != NormalRock) {
        if (mWanwanVoiceTimer >= cSeWanwanVoiceInterval) {
            if (MR::calcDistanceToPlayer(this) <= cSeWanwanVoiceDistance) {
                if (mRockType == WanwanRollingMini) {
                    MR::startSound(this, "SE_EV_WANWANROLLMINI_WANWAN", -1, -1);
                } else {
                    MR::startSound(this, "SE_EV_WANWANROLL_WANWAN", -1, -1);
                }

                mWanwanVoiceTimer = 0;
            }
        } else {
            mWanwanVoiceTimer++;
        }
    }
}

void Rock::startRollLevelSound(bool resetTimer) {
    if (resetTimer) {
        mRollSoundTimer = cRollSoundTime;
    }

    if (mRollSoundTimer > 0) {
        if (mRockType == NormalRock) {
            MR::startLevelSound(this, "SE_OJ_LV_ROCK_MOVE", -1, -1, -1);
        } else if (mRockType == WanwanRollingMini) {
            MR::startLevelSound(this, "SE_EM_LV_WANWANROLLMINI_MOVE", -1, -1, -1);
        } else {
            MR::startLevelSound(this, "SE_EM_LV_WANWANROLL_MOVE", -1, -1, -1);
        }

        if (MR::isBindedGroundBrake(this)) {
            MR::startLevelSound(this, "SE_OJ_LV_ROCK_MOVE_MUD", -1, -1, -1);
        }
        mRollSoundTimer--;
    }

    if (mRockType == WanwanRollingGold) {
        MR::startLevelSound(this, "SE_EM_LV_WANWANROLLGOLD_GLITTER", -1, -1, -1);
    }
}

bool Rock::tryFreeze(const Nerve* pNerve) {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        mFreezeTime = 0;
        mFreezePos.set(mPosition);
        mUnfreezeNerve = pNerve;
        setNerve(&NrvRock::RockNrvFreeze::sInstance);
        return true;
    }

    return false;
}

void Rock::setBtkForEnvironmentMap(LiveActor* pActor, const char* pBtkName) {
    f32 frame;
    if (mRockType == NormalRock) {
        if (MR::isNearZero(mScale.x - 0.5f)) {
            frame = 0.0f;
        } else if (MR::isNearZero(mScale.x - 2.0f)) {
            frame = 2.0f;
        } else {
            frame = 1.0f;
        }
    } else if (mRockType == WanwanRolling) {
        if (MR::isNearZero(mScale.x - 0.9f)) {
            frame = 1.0f;
        } else if (MR::isNearZero(mScale.x - 0.5f)) {
            frame = 2.0f;
        } else {
            frame = 0.0f;
        }
    }

    MR::startBtkAndSetFrameAndStop(pActor, pBtkName, frame);
}

void Rock::rumblePadAndCamera() {
    MR::startRumbleWithShakeCameraWeak(this, "強", "中", cRumbleDistance, cRumbleDistance * 2);

    f32 dist = MR::calcDistanceToPlayer(this);
    if (dist < cRumbleDistance) {
        MR::shakeCameraNormal();
    } else if (dist < cRumbleDistance * 2) {
        MR::shakeCameraWeak();
    }
}

void Rock::setNerveBreak(bool createStarPieces) {
    if (createStarPieces && (u32)mRockType <= WanwanRolling) {
        appearStarPiece();
    }

    setNerve(&NrvRock::RockNrvBreak::sInstance);
}

void Rock::updateRotateX(f32 angle) {
    mRotation.x = MR::repeatDegree(angle);
}

void Rock::appearStarPiece() {
    TVec3f pieceDir(mGravity);
    pieceDir.negateInternal();
    if (MR::appearStarPieceToDirection(mCreator, mPosition, pieceDir, getAppearStarPieceNum(mRockType), 10.0f, 40.0f, false)) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
}

void Rock::moveOnRail(f32 speed, f32 rotateSpeed, bool isValidBind) {
    MR::moveCoordAndFollowTrans(this, speed);
    updateRotateX(mRotation.x + rotateSpeed);
    if (isValidBind) {
        TVec3f pos;
        if (MR::getFirstPolyOnLineToMap(&pos, nullptr, mPosition, mGravity.scaleInline(mRadius * 2.0f))) {
            mPosition.add(pos, mGravity.scaleInline(-mRadius));
        }
    }
}

bool Rock::isForceInvalidBindSection() const {
    for (s32 idx = MR::getCurrentRailPointNo(this); idx < MR::getRailPointNum(this); idx++) {
        bool invalidBind;
        MR::getNextRailPointArg2WithInit(this, &invalidBind);

        if (invalidBind) {
            return true;
        }
    }

    return false;
}

bool Rock::tryBreakReachedGoal() {
    if (MR::isRailReachedGoal(this)) {
        if (mBreakModelOnRailGoal) {
            setNerve(&NrvRock::RockNrvBreak::sInstance);
        } else {
            makeActorDead();
        }
        return true;
    }

    return false;
}

const char* Rock::getTouchEffect() const {
    return mRockType == WanwanRollingMini ? "MiniTouch" : "Touch";
}

void Rock::exeAppear() {
    // FIXME: float regswaps and swapped load order near beginning
    // https://decomp.me/scratch/gOOpa

    f32 rotateSpeed = (cAppearMoveSpeed * 180.0f * cRotateSpeedRate) / (PI * mRadius);
    s32 spawnDelay = mRockType == NormalRock ? cAppearStopFrameRock : cAppearStopFrame;

    if (MR::isFirstStep(this)) {
        mAppearAngle = mRockType == WanwanRollingMini ? MR::getRandomDegree() : 0.0f;

        updateRotateX(mAppearAngle - rotateSpeed * mAppearTime);
    }

    if (MR::isLessStep(this, mAppearTime)) {
        moveOnRail(cAppearMoveSpeed, rotateSpeed, false);
        if (mRockType != NormalRock) {
            MR::startLevelSound(this, "SE_EM_LV_WANWANROLL_STANDBY", -1, -1, -1);
        }
    }

    if (MR::isStep(this, mAppearTime) && mRockType != NormalRock) {
        MR::startSound(this, "SE_EM_WANWANROLL_STANDBY_END", -1, -1);
    }

    if (MR::isGreaterStep(this, mAppearTime) && MR::isLessStep(this, mAppearTime + cAppearRumbleFrame)) {
        s32 step = getNerveStep() - mAppearTime;
        f32 f1 = MR::sinDegree(MR::repeatDegree(step * cAppearRumbleSpeed));
        f32 f2 = (cAppearRumbleFrame - step) * (f1 * cAppearRumbleAngle) / cAppearRumbleFrame;
        mRotation.x = f2;
        updateRotateX(f2 + mAppearAngle);
    }

    if (MR::isStep(this, spawnDelay + mAppearTime + cAppearRumbleFrame)) {
        setNerve(&NrvRock::RockNrvAppearMoveInvalidBind::sInstance);
    }
}

void Rock::exeAppearMoveInvalidBind() {
    bool isInvalid = isForceInvalidBindSection();
    if (!MR::isHiddenModel(this)) {
        MR::emitEffect(this, "Smoke");
        if (mRockType == WanwanRollingGold) {
            MR::emitEffect(this, "Light");
        }
    }

    startRollLevelSound(true);
    startSoundWanwanVoice();

    if (tryFreeze(&NrvRock::RockNrvAppearMoveInvalidBind::sInstance)) {
        return;
    }

    moveOnRail(mMoveSpeed, mRotateSpeed, isInvalid);
    mInvalidBindTime++;

    if (!isInvalid && mInvalidBindTime > cAppearInvalidBindFrame) {
        MR::onBind(this);
        setNerve(&NrvRock::RockNrvMove::sInstance);
    }
}

void Rock::exeMove() {
    // FIXME: float regswaps
    // https://decomp.me/scratch/uZucx

    if (MR::isOnGround(this)) {
        if (mRockType == NormalRock && MR::isBindedGroundDamageFire(this)) {
            setNerve(&NrvRock::RockNrvBreak::sInstance);
            return;
        }

        if (!MR::isHiddenModel(this)) {
            MR::emitEffect(this, "Smoke");
            if (mRockType == WanwanRollingGold) {
                MR::emitEffect(this, "Light");
            }
        }

        if (mAirTime >= cNoBindJudgeFrame) {
            if (!MR::isHiddenModel(this)) {
                MR::emitEffect(this, "Land");
                rumblePadAndCamera();
            }

            if (mRockType == NormalRock) {
                MR::startSound(this, "SE_OJ_ROCK_BOUND", -1, -1);
            } else {
                MR::startSound(this, "SE_EM_WANWANROLL_BOUND", -1, -1);
            }
        }
        mAirTime = 0;
    } else {
        if (mAirTime < cNoBindJudgeFrame) {
            mAirTime++;
        } else {
            MR::deleteEffect(this, "Smoke");
        }
    }

    startRollLevelSound(true);
    startSoundWanwanVoice();

    if (MR::isOnGround(this)) {
        if (mIsNormalGravity && mGravityRate != cGravity) {
            mGravityRate = cGravity;
        }
    } else {
        if (!mIsNormalGravity) {
            mIsNormalGravity = true;
        }
    }

    if (isBreakByWall()) {
        if (mRockType == NormalRock) {
            setNerve(&NrvRock::RockNrvBreak::sInstance);
        } else {
            makeActorDead();
        }
        return;
    }

    if (tryBreakReachedGoal()) {
        return;
    }

    f32 moveSpeed = mMoveSpeed;
    if (mSlowDownTimer > 0) {
        moveSpeed = MR::getEaseOutValue(mSlowDownTimer, moveSpeed, 0.0f, cSlowMoveFrame);
        mSlowDownTimer--;
    }

    move(moveSpeed);

    f32 rotateSpeed = mRotateSpeed * moveSpeed / mMoveSpeed;
    if (mSlowDownTimer > cSlowMoveFrame - cSlowMoveRumbleFrame) {
        s32 step = cSlowMoveFrame - mSlowDownTimer;
        f32 f1 = MR::sinDegree(MR::repeatDegree(step * cAppearRumbleSpeed));
        rotateSpeed += (cSlowMoveRumbleFrame - step) * (f1 * cAppearRumbleAngle) / cSlowMoveRumbleFrame;
    }
    updateRotateX(mRotation.x + rotateSpeed);

    if (tryFreeze(&NrvRock::RockNrvMove::sInstance)) {
        return;
    }
}

void Rock::exeMoveInvalidBind() {
    if (MR::isFirstStep(this)) {
        MR::moveCoordAndTransToNearestRailPos(this);
        mVelocity.zero();
        MR::offBind(this);
        MR::deleteEffect(this, "Smoke");
    }

    startRollLevelSound(true);
    startSoundWanwanVoice();

    if (!tryBreakReachedGoal()) {
        moveOnRail(mMoveSpeed, mRotateSpeed, false);
    }
}

void Rock::exeBreak() {
    // FIXME: operation order in the TODO section
    // https://decomp.me/scratch/wW8CP

    if (MR::isFirstStep(this)) {
        bool clipped = isInClippingRange();
        if (clipped || mBreakModel == nullptr) {
            makeActorDead();
            if (!clipped && mRockType == WanwanRollingMini) {
                MR::emitEffect(this, "MiniBreak");
                MR::startSound(this, "SE_EM_WANWANROLLMINI_EXPLOSION", -1, -1);
            }
            return;
        }

        MR::hideModel(this);
        MR::invalidateHitSensors(this);
        mRotation.zero();
        mVelocity.zero();
        mFallVelocity.zero();
        MR::deleteEffectAll(this);

        mBreakModel->mPosition.set(mPosition);

        TPos3f mtx;
        calcBaseMtx(&mtx);

        TPos3f rotMtx;
        rotMtx.makeRotate(mGravity, MR::getRandom(0.0f, TWO_PI));
        mtx.concat(rotMtx, mtx);

        // TODO: this logic is reused in many places, is this an inline from ActorMovementUtil?
        TVec3f rot;
        if ((mtx[2][0] - 1.0f) >= -0.0000038146973f) {
            rot.x = JMath::sAtanTable.atan2_(-mtx[0][1], mtx[1][1]);
            rot.y = -HALF_PI;
            rot.z = 0.0f;
        } else if ((mtx[2][0] + 1.0f) <= 0.0000038146973f) {
            rot.x = JMath::sAtanTable.atan2_(mtx[0][1], mtx[1][1]);
            rot.y = HALF_PI;
            rot.z = 0.0f;
        } else {
            rot.x = JMath::sAtanTable.atan2_(mtx[2][1], mtx[2][2]);
            rot.y = JMath::sAtanTable.atan2_(mtx[1][0], mtx[0][0]);
            rot.z = JGeometry::TUtil< f32 >::asin(-mtx[2][0]);
        }
        mBreakModel->mRotation.x = _180_PI * rot.x;
        mBreakModel->mRotation.y = _180_PI * rot.y;
        mBreakModel->mRotation.z = _180_PI * rot.z;

        mBreakModel->appear();

        MR::invalidateClipping(mBreakModel);
        MR::startBck(mBreakModel, "Break", nullptr);

        if (mRockType == NormalRock) {
            MR::startSound(this, "SE_OJ_ROCK_BREAK", -1, -1);
        } else {
            MR::startSound(this, "SE_EM_WANWANROLL_EXPLOSION", -1, -1);
            if (mRockType == WanwanRolling) {
                setBtkForEnvironmentMap(mBreakModel, "WanwanRollingBreak");
            }
        }

        rumblePadAndCamera();
    }

    if (mRockType == WanwanRollingGold) {
        MR::stopSceneAtStep(this, cBreakStopStep, cBreakStopFrame);
        if (MR::isStep(this, cAppearPowerStarStep)) {
            MR::requestAppearPowerStar(this, mBreakModel->mPosition);
        }
    }

    if (MR::isBckStopped(mBreakModel)) {
        kill();
        mBreakModel->kill();
    }
}

void Rock::exeFreeze() {
    // FIXME: issues with getTouchEffect
    // https://decomp.me/scratch/SSFkq

    if (MR::isFirstStep(this)) {
        mVelocity.zero();
        MR::emitEffect(this, getTouchEffect());
        MR::deleteEffect(this, "Smoke");
        if (mFreezeTime == 0) {
            MR::startDPDHitSound();
        }
    }

    mFreezeTime++;
    MR::startDPDFreezeLevelSound(this);

    f32 f1 = JMath::sSinCosTable.cosLap(MR::repeatDegree(mFreezeTime * cFreezeRumbleSpeed));
    s32 step = cFreezeFrame - getNerveStep();
    f32 f2 = step * (f1 * cFreezeRumbleWidth) / cFreezeFrame;
    TVec3f v1;
    v1.set(MR::getCamXdir());
    v1.scale(f2);
    mPosition.add(mFreezePos, v1);

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvRock::RockNrvFreeze::sInstance);
        return;
    }

    if (MR::isStep(this, cFreezeFrame)) {
        mPosition.set(mFreezePos);
        MR::deleteEffect(this, getTouchEffect());
        setNerve(mUnfreezeNerve);
    }
}
