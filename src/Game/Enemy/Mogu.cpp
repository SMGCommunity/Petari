#include "Game/Enemy/Mogu.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/MoguStone.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"

namespace {
    f32 hCannonFleetSightParam[3] = {1500.0f, 10.0f, 90.0f};
    f32 hThrowableSightParam[3] = {900.0f, 10.0f, 90.0f};
}  // namespace

namespace NrvMogu {
    NEW_NERVE(HostTypeNrvHideWait, Mogu, HideWait);
    NEW_NERVE(HostTypeNrvHide, Mogu, Hide);
    NEW_NERVE(HostTypeNrvAppear, Mogu, Appear);
    NEW_NERVE(HostTypeNrvSearch, Mogu, Search);
    NEW_NERVE(HostTypeNrvTurn, Mogu, Search);
    NEW_NERVE_ONEND(HostTypeNrvThrow, Mogu, Throw, Throw);
    NEW_NERVE(HostTypeNrvSwoonStart, Mogu, SwoonStart);
    NEW_NERVE(HostTypeNrvSwoonEnd, Mogu, SwoonEnd);
    NEW_NERVE(HostTypeNrvSwoon, Mogu, Swoon);
    NEW_NERVE(HostTypeNrvHipDropReaction, Mogu, HipDropReaction);
    NEW_NERVE(HostTypeNrvStampDeath, Mogu, StampDeath);
    NEW_NERVE(HostTypeNrvHitBlow, Mogu, HitBlow);
}  // namespace NrvMogu

Mogu::Mogu(const char* pName)
    : LiveActor(pName), mNerveExecutor(nullptr), _90(nullptr), mStone(nullptr), mHole(nullptr), mSight(0, 0, 1), _A8(0, 1, 0), _B4(true),
      mIsCannonFleet(false) {}

void Mogu::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::useStageSwitchWriteDead(this, rIter);
        MR::useStageSwitchReadA(this, rIter);
        MR::useStageSwitchSleep(this, rIter);

        s32 value = -1;
        MR::getJMapInfoArg0NoInit(rIter, &value);

        if (value != -1) {
            _B4 = false;
        } else {
            _B4 = true;
        }
    }
    initModelManagerWithAnm("Mogu", nullptr, false);
    MtxPtr mtx = getBaseMtx();

    _A8.set< f32 >(mtx[0][1], mtx[0][2], mtx[0][3]);
    MR::connectToSceneEnemy(this);
    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);

    f32 scaleY = mScale.y;
    f32 f1 = scaleY * 160.0f;
    initBinder(f1, f1, 0);
    initHitSensor(2);

    TVec3f v1(-55.0f, 0.0f, 13.0f);
    TVec3f v2(v1);
    v2 *= scaleY;

    MR::addHitSensorAtJointEnemy(this, "body", "Head", 32, 150.0f * mScale.y, v2);
    initEffectKeeper(false, nullptr, false);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 50.0f, 0.0f));

    initSound(4, 0);
    initNerve(&NrvMogu::HostTypeNrvSearch::sInstance);
    mGravity.set(-_A8);

    MR::initShadowVolumeSphere(this, 60.0f * mRotation.y);
    MR::invalidateShadow(this, nullptr);
    MR::initLightCtrl(this);
    mNerveExecutor = new AnimScaleController(nullptr);

    makeActorAppeared();

    _90 = new FixedPosition(this, "ArmR2", TVec3f(67.38f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));

    // "Rock-throwing stone"
    mStone = new MoguStone("投石用の石", "MoguStone");
    mStone->initWithoutIter();

    // "Burrow model"
    mHole = new ModelObj("巣穴モデル", "MoguHole", nullptr, -2, -2, -2, false);
    mHole->mPosition.set(mPosition);
    mHole->mRotation.set(mRotation);
    mHole->initWithoutIter();

    mIsCannonFleet = MR::isEqualStageName("CannonFleetGalaxy");
}

void Mogu::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::emitEffect(this, "Death");
}

void Mogu::control() {
    mNerveExecutor->updateNerve();
}

void Mogu::endClipped() {
    LiveActor::endClipped();
    if (isNerve(&NrvMogu::HostTypeNrvTurn::sInstance)) {
        setNerve(&NrvMogu::HostTypeNrvTurn::sInstance);
    }
}

void Mogu::exeHideWait() {
    TVec3f normalToPlayer(*MR::getPlayerPos());
    normalToPlayer -= mPosition;
    MR::normalizeOrZero(&normalToPlayer);
    f32 dot = mGravity.dot(normalToPlayer);
    if (!(dot < -0.75f)) {
        f32 distanceToPlayer = MR::calcDistanceToPlayer(this);
        if (MR::isGreaterStep(this, 0x78) && 400.0f < distanceToPlayer && distanceToPlayer < 2000.0f) {
            setNerve(&NrvMogu::HostTypeNrvAppear::sInstance);
        }
    }
}

bool Mogu::isPlayerExistUp() {
    TVec3f vecToPlayer(*MR::getPlayerCenterPos());
    vecToPlayer -= mPosition;
    f32 dot = _A8.dot(vecToPlayer);
    if (dot < 0.0f) {
        return false;
    }

    TVec3f* playerGravity = MR::getPlayerGravity();
    MR::vecKillElement(vecToPlayer, *playerGravity, &vecToPlayer);
    return vecToPlayer.length() < 400.0f;
}

void Mogu::tearDownThrow() {
    if (mStone->isTaken()) {
        mStone->kill();
    }
}

void Mogu::exeHipDropReaction() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "HipDropReaction");
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMogu::HostTypeNrvSwoon::sInstance);
    }
}

void Mogu::exeSwoonEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SwoonEnd");
    }

    MR::startLevelSound(this, "SE_EM_LV_MOGU_SWOON_RECOVER", -1, -1, -1);

    if (MR::isActionEnd(this)) {
        MR::startAction(mHole, "Close");
        MR::startSound(this, "SE_EM_MOGUHOLE_CLOSE", -1, -1);
        setNerve(&NrvMogu::HostTypeNrvHideWait::sInstance);
    }
}

void Mogu::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hide");
        MR::startAction(mHole, "Hide");
        MR::startSound(this, "SE_EM_MOGU_HIDE", -1, -1);

        TVec3f v1(mPosition);
        v1 -= *MR::getPlayerCenterPos();
        f32 length = v1.length();

        MR::vecKillElement(v1, mGravity, &v1);
        MR::normalizeOrZero(&v1);

        f32 dot = MR::getPlayerVelocity()->dot(v1);
        if (8.0f < dot || length < 200.0f) {
            MR::setBckRate(this, 1.5f);
        }
    }
    if (MR::isActionEnd(this)) {
        MR::startSound(this, "SE_EM_MOGUHOLE_CLOSE", -1, -1);
        setNerve(&NrvMogu::HostTypeNrvHideWait::sInstance);
    }
}

void Mogu::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Appear");
        MR::startAction(mHole, "Open");
        MR::startSound(this, "SE_EM_MOGUHOLE_OPEN", -1, -1);
        MR::startSound(this, "SE_EM_MOGU_APPEAR", -1, -1);
        TVec3f v1;
        MR::calcVecToPlayerH(&v1, this, nullptr);
        MR::turnVecToVecRadian(&mSight, mSight, v1, PI, _A8);
    }

    // "Strong"
    if (MR::isStarPointerPointing2POnTriggerButton(this, "強", true, false)) {
        MR::start2PAttackAssistSound();
        setNerve(&NrvMogu::HostTypeNrvSwoonStart::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, 0x1e) && isNearPlayerHipDrop()) {
        setNerve(&NrvMogu::HostTypeNrvSwoonStart::sInstance);
        return;
    }

    f32 distanceToPlayer = MR::calcDistanceToPlayer(this);
    if (distanceToPlayer < 400.0f || isPlayerExistUp()) {
        setNerve(&NrvMogu::HostTypeNrvHide::sInstance);
        return;
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMogu::HostTypeNrvSearch::sInstance);
    }
}

void Mogu::exeSearch() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvMogu::HostTypeNrvTurn::sInstance)) {
            MR::startAction(this, "Turn");
        } else {
            MR::startAction(this, "Wait");
        }
    }

    // "Strong"
    if (MR::isStarPointerPointing2POnTriggerButton(this, "強", true, false)) {
        MR::start2PAttackAssistSound();
        setNerve(&NrvMogu::HostTypeNrvSwoonStart::sInstance);
        return;
    }

    if (isNearPlayerHipDrop()) {
        setNerve(&NrvMogu::HostTypeNrvSwoonStart::sInstance);
        return;
    }

    if (isNerve(&NrvMogu::HostTypeNrvTurn::sInstance)) {
        TVec3f v1;
        MR::calcVecToPlayerH(&v1, this, nullptr);
        MR::turnVecToVecRadian(&mSight, mSight, v1, 0.03f, _A8);
    }

    f32 distanceToPlayer = MR::calcDistanceToPlayer(this);

    f32 sightParam;
    if (mIsCannonFleet) {
        sightParam = hCannonFleetSightParam[0];
    } else {
        sightParam = hThrowableSightParam[0];
    }

    if (distanceToPlayer < 400.0f || isPlayerExistUp()) {
        setNerve(&NrvMogu::HostTypeNrvHide::sInstance);
        return;
    }

    if (2000.0f < distanceToPlayer) {
        setNerve(&NrvMogu::HostTypeNrvHide::sInstance);
        return;
    }

    if (isNerve(&NrvMogu::HostTypeNrvSearch::sInstance) && distanceToPlayer < sightParam) {
        if (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this)) {
            setNerve(&NrvMogu::HostTypeNrvTurn::sInstance);
            return;
        }
    }

    if (isNerve(&NrvMogu::HostTypeNrvTurn::sInstance) && distanceToPlayer < sightParam) {
        setNerve(&NrvMogu::HostTypeNrvSearch::sInstance);
        return;
    }

    if (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this)) {
        f32* sight2 = hThrowableSightParam;
        if (mIsCannonFleet) {
            sight2 = hCannonFleetSightParam;
        }

        if (MR::isInSightFanPlayer(this, mSight, sight2[0], sight2[1], sight2[2]) && MR::isGreaterStep(this, 0x2d) && MR::isDead(mStone)) {
            setNerve(&NrvMogu::HostTypeNrvThrow::sInstance);
        }
    }
}

void Mogu::exeThrow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Throw");
        mStone->appear();
        MR::startAction(mStone, "Rotate");
        MR::startSound(this, "SE_EM_MOGU_TAKE_ITEM", -1, -1);
    }

    // "Strong"
    if (MR::isStarPointerPointing2POnTriggerButton(this, "強", true, false)) {
        MR::start2PAttackAssistSound();
        setNerve(&NrvMogu::HostTypeNrvSwoonStart::sInstance);
        return;
    }

    if (isNearPlayerHipDrop()) {
        setNerve(&NrvMogu::HostTypeNrvSwoonStart::sInstance);
        return;
    }

    if (MR::isStep(this, 0x2f)) {
        TVec3f upVec;
        MR::calcUpVec(&upVec, this);

        TVec3f stoneToPlayer(*MR::getPlayerCenterPos());
        stoneToPlayer -= mStone->mPosition;

        TVec3f killElementOut;
        MR::vecKillElement(stoneToPlayer, upVec, &killElementOut);

        f32 dot = upVec.dot(stoneToPlayer);

        TVec3f v4;
        v4.scale(dot, upVec);

        TVec3f* stonePos = &mStone->mPosition;

        f32 f1 = killElementOut.length();

        // Stack swap between 0x20 and 0x8, which are v5 and v7 here
        TVec3f v5(mSight);
        v5 *= f1;

        TVec3f v6 = mStone->mPosition;
        v6 += v5;

        TVec3f v7(v6);
        v5 += v4;

        mStone->emit(_B4, *stonePos, v7, 15.0f);
        MR::startSound(this, "SE_EM_MOGU_THROW", -1, -1);
    }

    f32 distanceToPlayer = MR::calcDistanceToPlayer(this);
    if (distanceToPlayer < 400.0f || isPlayerExistUp()) {
        setNerve(&NrvMogu::HostTypeNrvHide::sInstance);
        return;
    }

    if (2000.0f < distanceToPlayer) {
        setNerve(&NrvMogu::HostTypeNrvHide::sInstance);
        return;
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMogu::HostTypeNrvSearch::sInstance);
    }
}

void Mogu::endThrow() {
    tearDownThrow();
}

void Mogu::exeSwoonStart() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_MOGU_TURNOVER", -1, -1);
        MR::startSound(this, "SE_EV_MOGU_TURNOVER", -1, -1);
        MR::startAction(this, "SwoonStart");

        if (!MR::isBckPlaying(mHole, "Open")) {
            MR::startAction(mHole, "Break");
        }
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMogu::HostTypeNrvSwoon::sInstance);
    }
}

void Mogu::exeSwoon() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Swoon");
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);

    if (isNearPlayerHipDrop()) {
        setNerve(&NrvMogu::HostTypeNrvHipDropReaction::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, 0x78)) {
        setNerve(&NrvMogu::HostTypeNrvSwoonEnd::sInstance);
    }
}

void Mogu::exeStampDeath() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Down");
        MR::startAction(mHole, "Close");
        MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
    }

    if (MR::isGreaterStep(this, 0x3c)) {
        kill();
        MR::emitEffect(this, "Death");
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
        MR::startAction(mHole, "Down");
        TVec3f coinPos(mPosition);
        MR::appearCoinPop(this, coinPos, 1);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
}

void Mogu::exeHitBlow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "PunchDown");
        MR::startAction(mHole, "Close");
        MR::startSound(this, "SE_EM_MOGUHOLE_CLOSE", -1, -1);
        MR::startBlowHitSound(this);
        MR::validateShadow(this, nullptr);
        TVec3f vecToPlayerH;
        MR::calcVecToPlayerH(&vecToPlayerH, this, nullptr);
        MR::turnVecToVecRadian(&mSight, mSight, vecToPlayerH, PI, _A8);
    }
    MR::applyVelocityDampAndGravity(this, 2.0f, 0.8f, 0.98f, 0.98f, 1.0f);

    if (MR::isGreaterStep(this, 0x14)) {
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
        kill();
        MR::startAction(mHole, "Down");
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
}
