#include "Game/Enemy/DharmaSambo.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/SamboFunction.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

namespace {
    const f32 hPursueGoParam[] = {0.3f, 1.0f, 0.97f, 3.0f};
    const f32 hPursueEscapeParam[] = {-0.25f, 1.0f, 0.97f, 3.0f};
    const f32 hOnlyTurnParam[] = {0.0f, 1.0f, 0.97f, 3.0f};
    const f32 hNoMoveNoTurnParam[] = {0.0f, 1.0f, 0.8f, 0.0f};
    const f32 hSpinHittedOnGroundParam[] = {0.0f, 1.0f, 0.8f, 0.0f};
    const f32 hSpinHittedAirParam[] = {0.0f, 2.0f, 0.97f, 0.0f};
    const f32 hEscapeOnGroundParam[] = {0.0f, 1.0f, 0.7f, 5.0f};
    const f32 hEscapeAirParam[] = {0.15f, 2.0f, 0.98f, 5.0f};
    const f32 hEscapeWaitOnGroundParam[] = {0.0f, 1.0f, 0.7f, 5.0f};
    const f32 hEscapeWaitAirParam[] = {0.0f, 2.0f, 0.98f, 5.0f};
    const f32 hPursueEscapeDist[] = {100.0f, 200.0f, 300.0f, 400.0f};
    const char* joint_name[] = {"Spine1", "Spine2", "Spine3", "Spine4"};
    const s32 hEscapeTime = 300;
    const f32 hEscapeRunVelH = 7.0f;
    const f32 hEscapeRunVelV = 30.0f;
    const f32 hEscapeWaitDist = 800.0f;
    const f32 hEscapeWaitJumpVel = 30.0f;
    // hGravity
    // hTurnLimitRadian
    // hUpdateUpRate
    const f32 hInitPosRange = 1250.0f;
    const s32 hAppearTime = 600;
    const s32 hBodyPartsNum = 4;
    const f32 hWaitUnderGroundDistance = 800.0f;
    const s32 hWaitUnderGroundTime = 120;
    // hPursueVel
    // hPursueStopDist
    const f32 hPursueGoAddDist = 100.0f;
    const s32 hToAttackTime = 120;
    const s32 hAttackTurnTime = 90;
    const s32 hHideTime = 300;
    const s32 hHitStopTime = 5;
    const f32 hSpinHittedMoveBodyRate = 0.5f;
    const f32 hSpinHitVelH = 10.0f;
    const f32 hSpinHitVelV = 30.0f;
    const s32 hSpinHittedBlowableTime = 50;
    const s32 hSpinHittedStarPointerBindableTime = 50;
    const s32 hSpinHittedLandTime = 27;
    const s32 hSpinHittedNoAttackTime = 30;
    const s32 hBindMinStep = 10;
    const s32 hStarPointerBindEndLandTime = 27;
    const s32 hAppearSeTiming = 60;
    const s32 hHideSeTiming = 65;
    const s32 hPreHideSeTiming = 30;
}  // namespace

void DharmaSambo::endStarPointerBind() {
    tearDownStarPointerBind();
}

void DharmaSamboParts::exeDoNothing() {
    MR::isFirstStep(this);
}

namespace NrvDharmaSambo {
    NEW_NERVE(HostTypeNrvWaitUnderGround, DharmaSambo, WaitUnderGround);
    NEW_NERVE(HostTypeNrvAppear, DharmaSambo, Appear);
    NEW_NERVE(HostTypeNrvAppearImmediately, DharmaSambo, Appear);
    NEW_NERVE_ONEND(HostTypeNrvPursue, DharmaSambo, Pursue, Pursue);
    NEW_NERVE(HostTypeNrvAttack, DharmaSambo, Attack);
    NEW_NERVE(HostTypeNrvHide, DharmaSambo, Hide);
    NEW_NERVE(HostTypeNrvHideAppear, DharmaSambo, HideAppear);
    NEW_NERVE(HostTypeNrvSpinHitted, DharmaSambo, SpinHitted);
    NEW_NERVE(HostTypeNrvStarPointerBindEnd, DharmaSambo, SpinHitted);
    NEW_NERVE(HostTypeNrvKilled, DharmaSambo, Killed);
    NEW_NERVE_ONEND(HostTypeNrvStarPointerBind, DharmaSambo, StarPointerBind, StarPointerBind);
    NEW_NERVE(HostTypeNrvEscapeRun, DharmaSambo, EscapeRun);
    NEW_NERVE(HostTypeNrvEscapeLand, DharmaSambo, EscapeLand);
    NEW_NERVE(HostTypeNrvEscapeWaitRun, DharmaSambo, EscapeRun);
    NEW_NERVE(HostTypeNrvEscapeWaitLand, DharmaSambo, EscapeLand);
    NEW_NERVE(HostTypeNrvTrampleDeath, DharmaSambo, TrampleDeath);
}  // namespace NrvDharmaSambo

namespace NrvDharmaSamboParts {
    NEW_NERVE(HostTypeNrvDoNothing, DharmaSamboParts, DoNothing);
    NEW_NERVE(HostTypeNrvBlow, DharmaSamboParts, Blow);
    NEW_NERVE_ONEND(HostTypeNrvPartsStarPointerBind, DharmaSamboParts, StarPointerBind, StarPointerBind);
}  // namespace NrvDharmaSamboParts

DharmaSambo::DharmaSambo(const char* pName)
    : LiveActor(pName), mStarPointerBindEndNerve(), mInitialPos(0, 0, 0), mHideTimer(hAppearTime), mPartsCount(hBodyPartsNum), mHitSensor(),
      mAnimScaleController() {
}

void DharmaSambo::init(const JMapInfoIter& rIter) {
    bool dead;
    MR::initDefaultPos(this, rIter);
    mInitialPos.set(mPosition);
    MR::needStageSwitchReadAppear(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::syncStageSwitchAppear(this);
    initModelManagerWithAnm("DharmaSambo", nullptr, false);
    MR::connectToSceneEnemy(this);
    f32 scale = mScale.y;
    initBinder(80.0f * scale, 80.0f * scale, 0);
    initEffectKeeper(1, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, false);
    initNerve(GET_NERVE(DharmaSambo, HostTypeNrvWaitUnderGround));
    MR::onCalcGravity(this);
    MR::declareStarPiece(this, 6);

    if (MR::isValidSwitchAppear(this)) {
        dead = true;
        makeActorDead();
    } else {
        dead = false;
        makeActorAppeared();
    }

    mAnimScaleParam = new AnimScaleParam;
    mAnimScaleController = SamboFunction::createAnimScaleController(mAnimScaleParam);
    mParts.init(hBodyPartsNum);
    mFixedPositions.init(hBodyPartsNum);

    for (s32 i = 0; i < hBodyPartsNum; i++) {
        MR::hideJoint(this, joint_name[i]);
        DharmaSamboParts* part = new DharmaSamboParts(this, "だるまサンボパーツ", "DharmaSamboParts", nullptr, 18, 43, -2);
        part->initWithoutIter();
        part->mOriginalIndex = i;
        part->setHead(i == hBodyPartsNum - 1);

        if (dead) {
            part->makeActorDead();
        } else {
            part->makeActorAppeared();
        }

        mParts.push_back(part);
        mFixedPositions.push_back(new FixedPosition(this, joint_name[i], TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, -90.0f, -90.0f)));
    }
}

void DharmaSambo::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void DharmaSambo::appear() {
    LiveActor::appear();

    for (s32 i = 0; i < mParts.size(); i++) {
        mParts[i]->appear();
    }

    setNerve(GET_NERVE(DharmaSambo, HostTypeNrvHideAppear));
}

void DharmaSambo::kill() {
    LiveActor::kill();

    for (s32 i = 0; i < mParts.size(); i++) {
        DharmaSamboParts* part = mParts[i];

        if (!MR::isDead(part)) {
            part->kill();
        }
    }

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void DharmaSambo::control() {
    mAnimScaleController->updateNerve();
    s32 count = mParts.size();

    if (mPartsCount == 1 && MR::isDead(getHeadParts())) {
        kill();
        return;
    }

    if (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvPursue)) || isNerve(GET_NERVE(DharmaSambo, HostTypeNrvAttack)) ||
        isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeRun)) || isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeLand)) ||
        isNerve(GET_NERVE(DharmaSambo, HostTypeNrvStarPointerBind)) ||
        (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvSpinHitted)) && MR::isGreaterStep(this, hSpinHittedBlowableTime))) {
        f32 closest = 99999.0f;
        DharmaSamboParts* hit = nullptr;

        for (s32 i = 0; i < count; i++) {
            DharmaSamboParts* part = mParts[i];
            bool isHitted = 0.0f <= part->mHitDistance;

            if (isHitted && part->mHitDistance < closest) {
                hit = part;
                closest = part->mHitDistance;
            }
        }

        if (hit != nullptr) {
            MR::emitEffectHitBetweenSensors(this, hit->getSensor("body"), mHitSensor, 0.0f, nullptr);
            mPartsCount--;
            MR::stopSceneForDefaultHit(hHitStopTime);

            if (hit->mIsHead) {
                hit->mIndex = -1;
                hit->setNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvBlow));
                setNerve(GET_NERVE(DharmaSambo, HostTypeNrvKilled));
            } else {
                hit->mIndex = -1;
                hit->setNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvBlow));
                TVec3f direction(mPosition - *MR::getPlayerCenterPos());
                MR::addVelocitySeparateHV(this, direction, hSpinHitVelH, hSpinHitVelV);
                MR::stopBck(this);
                setNerve(GET_NERVE(DharmaSambo, HostTypeNrvSpinHitted));
                s32 index = 0;

                for (s32 i = 0; i < count; i++) {
                    if (mParts[i]->mIndex != -1) {
                        if (hit == mParts[i]) {
                            mParts[i]->mIndex = -1;
                        } else {
                            mParts[i]->mIndex = index++;
                        }
                    }
                }
            }
        }
    }

    for (s32 i = 0; i < count; i++) {
        mParts[i]->mHitDistance = -1.0f;
    }

    if (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvSpinHitted))) {
        for (s32 i = 0; i < count; i++) {
            mFixedPositions[i]->calc();

            if (mParts[i]->mIndex != -1) {
                s32 index = mParts[i]->mIndex;
                TVec3f direction;
                mFixedPositions[index]->copyTrans(&direction);
                direction -= mParts[i]->mPosition;
                mParts[i]->mPosition.add(direction * hSpinHittedMoveBodyRate);
                mFixedPositions[index]->copyRotate(&mParts[i]->mRotation);
            }
        }
    } else {
        for (s32 i = 0; i < count; i++) {
            mFixedPositions[i]->calc();

            if (mParts[i]->mIndex != -1) {
                s32 index = mParts[i]->mIndex;
                mFixedPositions[index]->copyTrans(&mParts[i]->mPosition);
                mFixedPositions[index]->copyRotate(&mParts[i]->mRotation);
            }
        }
    }

    if (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvPursue)) || isNerve(GET_NERVE(DharmaSambo, HostTypeNrvAttack)) ||
        (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvSpinHitted)) && MR::isGreaterStep(this, hSpinHittedStarPointerBindableTime)) ||
        isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeRun)) || isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeLand)) ||
        isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeWaitRun)) || isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeWaitLand))) {
        mStarPointerBindEndNerve = mSpine->getCurrentNerve();

        if (isStarPointerPointingParts()) {
            setNerve(GET_NERVE(DharmaSambo, HostTypeNrvStarPointerBind));
        }
    }
}

void DharmaSambo::exeWaitUnderGround() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "HideWait");
        startActionAllParts("HideWait");
        mPosition.set(mInitialPos);
        mVelocity.zero();
    }

    MR::startLevelSound(this, "SE_EM_LV_DHARMASAMBO_MOVE_UNDER");
    MR::moveAndTurnToPlayer(this, hOnlyTurnParam[0], hOnlyTurnParam[1], hOnlyTurnParam[2], hOnlyTurnParam[3]);
    f32 distance = MR::calcDistanceToPlayer(this);

    if (MR::isGreaterStep(this, hWaitUnderGroundTime) && distance < hWaitUnderGroundDistance) {
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvAppear));
    }
}

void DharmaSambo::exeAppear() {
    if (MR::isFirstStep(this)) {
        mPartsCount = hBodyPartsNum;
        MR::startAction(this, "Appear");
        mVelocity.zero();
        mHideTimer = hAppearTime;
        startActionAllParts("Appear");

        if (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvAppearImmediately))) {
            MR::setBckFrameAndStop(this, 59.0f);
            MR::setBckRate(this, 1.0f);
        }
    }

    if (MR::isStep(this, hAppearSeTiming)) {
        MR::startSound(this, "SE_EM_SFSAMBO_APPEAR");
    }

    MR::moveAndTurnToPlayer(this, hOnlyTurnParam[0], hOnlyTurnParam[1], hOnlyTurnParam[2], hOnlyTurnParam[3]);

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvPursue));
    }
}

void DharmaSambo::exePursue() {
    if (MR::isFirstStep(this) && !MR::isBckPlaying(this, "Wait")) {
        MR::startAction(this, "Wait");
        startActionAllParts("Wait");
    }

    MR::startLevelSound(this, "SE_EM_LV_DHARMASAMBO_MOVE");
    bool inRange = false;
    f32 distance = MR::calcDistanceToPlayer(this);

    if (2000.0f < distance || hInitPosRange * hInitPosRange < mPosition.squared(mInitialPos)) {
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvHide));
        return;
    }

    f32 stopDistance = hPursueEscapeDist[mPartsCount - 1];

    if (hPursueGoAddDist + stopDistance < distance) {
        MR::moveAndTurnToPlayer(this, hPursueGoParam[0], hPursueGoParam[1], hPursueGoParam[2], hPursueGoParam[3]);
    } else if (distance < stopDistance) {
        MR::moveAndTurnToPlayer(this, hPursueEscapeParam[0], hPursueEscapeParam[1], hPursueEscapeParam[2], hPursueEscapeParam[3]);
    } else {
        MR::moveAndTurnToPlayer(this, hOnlyTurnParam[0], hOnlyTurnParam[1], hOnlyTurnParam[2], hOnlyTurnParam[3]);
        inRange = true;
    }

    if (inRange && MR::checkPassBckFrame(this, hToAttackTime)) {
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvAttack));
        return;
    }

    decCountAndTryToHide(true);
}

void DharmaSambo::endPursue() {
    MR::deleteEffect(this, "WaitSmoke");
}

void DharmaSambo::exeAttack() {
    if (MR::isFirstStep(this)) {
        if (!MR::isBckPlaying(this, "Attack")) {
            MR::startAction(this, "Attack");
            mVelocity.zero();
            startActionAllParts("Attack");
        }

        if (mPartsCount == 2) {
            MR::setBckRate(this, 1.2f);
        }
    }

    s32 turnTime = hAttackTurnTime;

    if (mPartsCount == 2) {
        turnTime = 75;
    }

    if (turnTime < MR::getBckFrame(this)) {
        MR::moveAndTurnToPlayer(this, hNoMoveNoTurnParam[0], hNoMoveNoTurnParam[1], hNoMoveNoTurnParam[2], hNoMoveNoTurnParam[3]);
    } else {
        MR::moveAndTurnToPlayer(this, hOnlyTurnParam[0], hOnlyTurnParam[1], hOnlyTurnParam[2], hOnlyTurnParam[3]);
    }

    bool end = MR::isActionEnd(this);
    s32 attackTime = 110;

    if (MR::checkPassBckFrame(this, attackTime) && !MR::checkPassBckFrame(this, attackTime + 1)) {
        MR::emitEffect(getHeadParts(), "Attack");
        MR::startSound(this, "SE_EM_SFSAMBO_ATTACK");
    }

    if (!decCountAndTryToHide(end) && end) {
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvPursue));
    }
}

void DharmaSambo::exeSpinHitted() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvStarPointerBindEnd))) {
            MR::startAction(this, "BindAfter");
            startActionAllParts("Wait");
        } else if (!MR::isBckPlaying(this, "SpinReaction")) {
            MR::startAction(this, "SpinReaction");
            startBtkAllParts("Press");
        }
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, hSpinHittedOnGroundParam[0], hSpinHittedOnGroundParam[1], hSpinHittedOnGroundParam[2],
                                hSpinHittedOnGroundParam[3]);
    } else {
        MR::moveAndTurnToPlayer(this, hSpinHittedAirParam[0], hSpinHittedAirParam[1], hSpinHittedAirParam[2], hSpinHittedAirParam[3]);
    }

    if (mPartsCount == 1) {
        if (MR::isOnGround(this)) {
            mHideTimer = hEscapeTime;
            setNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeLand));
        }
    } else {
        s32 landTime = isNerve(GET_NERVE(DharmaSambo, HostTypeNrvSpinHitted)) ? hSpinHittedLandTime : hStarPointerBindEndLandTime;

        if (MR::checkPassBckFrame(this, landTime) && !MR::checkPassBckFrame(this, landTime + 1)) {
            startActionAllParts("Land");
        }

        if (MR::isOnGround(this) && MR::isBckOneTimeAndStopped(this)) {
            setNerve(GET_NERVE(DharmaSambo, HostTypeNrvPursue));
        }
    }
}

void DharmaSambo::exeStarPointerBind() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 0.0f);
        MR::setBckRate(getHeadParts(), 0.0f);
        s32 count = mParts.size();

        for (s32 i = 0; i < count; i++) {
            DharmaSamboParts* part = mParts[i];

            if (!part->isNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvBlow))) {
                part->setNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvPartsStarPointerBind));
            }
        }

        mAnimScaleController->startDpdHitVibration();
    }

    mVelocity.zero();

    if (MR::isGreaterStep(this, hBindMinStep) && !isStarPointerPointingParts()) {
        setNerve(mStarPointerBindEndNerve);
    }
}

void DharmaSambo::tearDownStarPointerBind() {
    mAnimScaleController->startAnim();
    s32 count = mParts.size();

    for (s32 i = 0; i < count; i++) {
        DharmaSamboParts* part = mParts[i];

        if (!part->isNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvBlow))) {
            part->setNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvDoNothing));
        }
    }

    MR::setBckRate(this, 1.0f);
    MR::setBckRate(getHeadParts(), 1.0f);
}

void DharmaSambo::exeEscapeRun() {
    if (MR::isFirstStep(this)) {
        if (!MR::isBckPlaying(getHeadParts(), "Run")) {
            MR::startAction(getHeadParts(), "Run");
            MR::startSound(this, "SE_EM_SAMBOHEAD_HOP");
        }

        MR::setBckRate(getHeadParts(), 1.2f);
    }

    if (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeWaitRun))) {
        if (MR::isOnGround(this)) {
            MR::moveAndTurnToPlayer(this, hEscapeWaitOnGroundParam[0], hEscapeWaitOnGroundParam[1], hEscapeWaitOnGroundParam[2],
                                    hEscapeWaitOnGroundParam[3]);
        } else {
            MR::moveAndTurnToPlayer(this, hEscapeWaitAirParam[0], hEscapeWaitAirParam[1], hEscapeWaitAirParam[2], hEscapeWaitAirParam[3]);
        }

        decCountAndTryToHide(false);

        if (MR::isOnGround(this) && isActionEndHead()) {
            setNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeWaitLand));
        }
    } else {
        TVec3f direction(mPosition);
        direction -= *MR::getPlayerPos();
        MR::vecKillElement(direction, mGravity, &direction);
        MR::normalizeOrZero(&direction);

        if (MR::isOnGround(this)) {
            MR::moveAndTurnToDirection(this, direction, hEscapeOnGroundParam[0], hEscapeOnGroundParam[1], hEscapeOnGroundParam[2],
                                       hEscapeOnGroundParam[3]);
        } else {
            MR::moveAndTurnToDirection(this, direction, hEscapeAirParam[0], hEscapeAirParam[1], hEscapeAirParam[2], hEscapeAirParam[3]);
        }

        decCountAndTryToHide(false);

        if (MR::isOnGround(this) && isActionEndHead()) {
            setNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeLand));
        }
    }
}

void DharmaSambo::exeEscapeLand() {
    if (MR::isFirstStep(this)) {
        if (!MR::isBckPlaying(getHeadParts(), "Land")) {
            MR::startAction(getHeadParts(), "Land");
        }

        MR::setBckRate(getHeadParts(), 1.2f);
    }

    if (isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeWaitLand))) {
        if (MR::isOnGround(this)) {
            MR::moveAndTurnToPlayer(this, hEscapeWaitOnGroundParam[0], hEscapeWaitOnGroundParam[1], hEscapeWaitOnGroundParam[2],
                                    hEscapeWaitOnGroundParam[3]);
        } else {
            MR::moveAndTurnToPlayer(this, hEscapeWaitAirParam[0], hEscapeWaitAirParam[1], hEscapeWaitAirParam[2], hEscapeWaitAirParam[3]);
        }
    } else {
        TVec3f direction(mPosition);
        direction -= *MR::getPlayerPos();
        MR::vecKillElement(direction, mGravity, &direction);
        MR::normalizeOrZero(&direction);
        MR::moveAndTurnToDirection(this, direction, hEscapeOnGroundParam[0], hEscapeOnGroundParam[1], hEscapeOnGroundParam[2],
                                   hEscapeOnGroundParam[3]);
    }

    decCountAndTryToHide(true);

    if (isActionEndHead()) {
        if (hEscapeWaitDist < MR::calcDistanceToPlayer(this)) {
            MR::addVelocityJump(this, hEscapeWaitJumpVel);
            setNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeWaitRun));
            return;
        }

        const TVec3f* playerPos = MR::getPlayerCenterPos();
        TVec3f direction(mPosition);
        direction -= *playerPos;
        MR::addVelocitySeparateHV(this, direction, hEscapeRunVelH, hEscapeRunVelV);
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeRun));
    }
}

void DharmaSambo::exeTrampleDeath() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(this);
        startActionHead("Down");
        MR::startSound(this, "SE_EM_STOMPED_S");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, hNoMoveNoTurnParam[0], hNoMoveNoTurnParam[1], hNoMoveNoTurnParam[2], hNoMoveNoTurnParam[3]);
    } else {
        MR::moveAndTurnToPlayer(this, hSpinHittedAirParam[0], hSpinHittedAirParam[1], hSpinHittedAirParam[2], hSpinHittedAirParam[3]);
    }

    if (isActionEndHead()) {
        kill();
    }
}

void DharmaSambo::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Hide");
        mVelocity.zero();
        startActionAllParts("Hide");

        if (mPartsCount == 1) {
            MR::setBckRate(this, 1.2f);
        }
    }

    if (MR::isStep(this, hPreHideSeTiming)) {
        MR::startSound(this, "SE_EM_DHARMASAMBO_PRE_HIDE");
    }

    if (MR::isStep(this, hHideSeTiming)) {
        MR::startSound(this, "SE_EM_SFSAMBO_HIDE");
    }

    MR::moveAndTurnToPlayer(this, hOnlyTurnParam[0], hOnlyTurnParam[1], hOnlyTurnParam[2], hOnlyTurnParam[3]);

    if (MR::isGreaterStep(this, mPartsCount == 1 ? 360 : hHideTime)) {
        MR::resetPosition(this, mInitialPos);
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvHideAppear));
    }
}

void DharmaSambo::exeHideAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "HideAppear");
        MR::startSound(this, "SE_EM_SAMBOHEAD_APPEAR");
        s32 count = mParts.size();

        for (s32 i = 0; i < count; i++) {
            DharmaSamboParts* part = mParts[i];

            if (MR::isDead(part)) {
                part->appear();
            }

            part->mIndex = i;
            part->setHead(i == count - 1);
            part->setNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvDoNothing));
            MR::startAction(part, "Wait");
            MR::setBckFrameAndStop(part, 5.0f);
            mFixedPositions[i]->copyTrans(&part->mPosition);
            MR::resetPosition(part);
        }

        startActionAllParts("HideAppear");
    }

    MR::moveAndTurnToPlayer(this, hOnlyTurnParam[0], hOnlyTurnParam[1], hOnlyTurnParam[2], hOnlyTurnParam[3]);

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvWaitUnderGround));
    }
}

void DharmaSambo::exeKilled() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::stopBck(this);
        s32 count = mParts.size();

        for (s32 i = 0; i < count; i++) {
            if (mParts[i]->mIndex != -1) {
                MR::stopBck(mParts[i]);
            }

            mParts[i]->getSensor("body")->invalidate();
        }

        MR::startAction(this, "Hit");
        mVelocity.zero();
    }

    MR::moveAndTurnToPlayer(this, hNoMoveNoTurnParam[0], hNoMoveNoTurnParam[1], hNoMoveNoTurnParam[2], hNoMoveNoTurnParam[3]);

    if (MR::isActionEnd(this)) {
        kill();
    }
}

DharmaSamboParts* DharmaSambo::getHeadParts() {
    for (s32 i = 0; i < mParts.size(); i++) {
        if (mParts[i]->mIsHead) {
            return mParts[i];
        }
    }

    return nullptr;
}

bool DharmaSambo::isActionEndHead() {
    for (s32 i = 0; i < mParts.size(); i++) {
        if (mParts[i]->mIsHead) {
            return MR::isActionEnd(mParts[i]);
        }
    }

    return false;
}

void DharmaSambo::startActionHead(const char* pAction) {
    for (s32 i = 0; i < mParts.size(); i++) {
        if (mParts[i]->mIsHead) {
            MR::startAction(mParts[i], pAction);
            return;
        }
    }
}

void DharmaSambo::startActionAllParts(const char* pAction) {
    s32 count = mParts.size();

    for (s32 i = 0; i < count; i++) {
        MR::startAction(mParts[i], pAction);
    }
}

void DharmaSambo::startBtkAllParts(const char* pBtk) {
    s32 count = mParts.size();

    for (s32 i = 0; i < count; i++) {
        MR::startBtk(mParts[i], pBtk);
    }
}

bool DharmaSambo::decCountAndTryToHide(bool hide) {
    mHideTimer--;

    if (hide && mHideTimer <= 0) {
        setNerve(GET_NERVE(DharmaSambo, HostTypeNrvHide));
        return true;
    }

    return false;
}

bool DharmaSambo::isStarPointerPointingParts() {
    for (s32 i = 0; i < mParts.size(); i++) {
        if (!MR::isDead(mParts[i]) && MR::isStarPointerPointing2POnPressButton(mParts[i], "弱", true, false)) {
            return true;
        }
    }

    return false;
}

DharmaSamboParts::DharmaSamboParts(DharmaSambo* pHost, const char* pName, const char* pModel, MtxPtr mtx, int movement, int animation, int draw)
    : ModelObj(pName, pModel, mtx, movement, animation, draw, false), mHost(pHost), mOriginalIndex(-1), mIndex(-1), mSpinHitController(),
      mHitDistance(-1.0f), mIsHead() {
}

void DharmaSamboParts::init(const JMapInfoIter& rIter) {
    mSpinHitController = new SpinHitController(this, 0, false);
    initNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvDoNothing));
    f32 scale = mScale.y;
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "body", 16, 80.0f * scale, TVec3f(0.0f, 80.0f * scale, 0.0f));
    MR::initLightCtrl(this);
    MR::initShadowVolumeSphere(this, 80.0f * mScale.y);
    MR::initStarPointerTarget(this, 110.0f, TVec3f(0.0f, 80.0f, 0.0f));
}

void DharmaSamboParts::appear() {
    LiveActor::appear();
    mIndex = mOriginalIndex;
    setNerve(GET_NERVE(DharmaSamboParts, HostTypeNrvDoNothing));
}

void DharmaSamboParts::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");

    if (mIsHead) {
        MR::appearStarPiece(mHost, mPosition, MR::getDeclareRemnantStarPieceCount(mHost), 10.0f, 40.0f, false);
        MR::startSound(mHost, "SE_OJ_STAR_PIECE_BURST");
    }
}

void DharmaSamboParts::control() {
    mGravity.set(mHost->mGravity);
}

void DharmaSamboParts::exeBlow() {
    if (MR::isFirstStep(this)) {
        mSpinHitController->start(this, *MR::getPlayerPos(), mPosition);
        MR::startAction(this, "Blow");
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_EM_DHARMASAMBO_BLOW");
    }

    mSpinHitController->execute(this);
}

void DharmaSamboParts::exeStarPointerBind() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        MR::setBckRate(this, 0.0f);
        MR::emitEffect(this, "Touch");
    }

    MR::startDPDFreezeLevelSound(this);
}

void DharmaSamboParts::endStarPointerBind() {
    MR::deleteEffect(this, "Touch");
    MR::setBckRate(this, 1.0f);
}

void DharmaSamboParts::setHead(bool head) {
    mIsHead = head;

    if (head) {
        MR::hideMaterial(this, "SamboMatBody_v");
        MR::showMaterial(this, "SamboMatFace_v");
        MR::showMaterial(this, "SamboMatEye_v");
        MR::showMaterial(this, "FlowerMat_v");
        return;
    }

    MR::showMaterial(this, "SamboMatBody_v");
    MR::hideMaterial(this, "SamboMatFace_v");
    MR::hideMaterial(this, "SamboMatEye_v");
    MR::hideMaterial(this, "FlowerMat_v");
}

void DharmaSambo::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    TVec3f scale(mAnimScaleController->_C);
    scale.mul(scale, mScale);
    MR::setBaseScale(this, scale);
}

void DharmaSamboParts::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();

    if (mIsHead && mHost->mPartsCount == 1 && mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvStarPointerBind))) {
        TVec3f scale(mHost->mAnimScaleController->_C);
        scale.mul(scale, mScale);
        MR::setBaseScale(this, scale);
    } else {
        MR::setBaseScale(this, TVec3f(1, 1, 1));
    }
}

void DharmaSamboParts::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (mIsHead) {
        if (mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeRun)) || mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvEscapeLand))) {
            if (MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25f) || MR::isPlayerHipDropFalling() || MR::isPlayerHipDropLand()) {
                MR::sendMsgPush(pReceiver, pSender);
                return;
            }

            if (!MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                MR::sendMsgPush(pReceiver, pSender);
            }
        } else {
            if (mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvWaitUnderGround)) || mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvHide)) ||
                mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvHideAppear)) ||
                mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvAppearImmediately))) {
                MR::sendMsgPush(pReceiver, pSender);
                return;
            }

            if (MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25f) || MR::isPlayerHipDropFalling() || MR::isPlayerHipDropLand()) {
                MR::sendMsgPush(pReceiver, pSender);
                return;
            }

            if (mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvAttack))) {
                if (!MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                    MR::sendMsgPush(pReceiver, pSender);
                }
            } else {
                MR::sendMsgPush(pReceiver, pSender);
            }
        }
    } else {
        if ((mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvSpinHitted)) && MR::isLessStep(this, hSpinHittedNoAttackTime)) ||
            mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvWaitUnderGround)) ||
            mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvStarPointerBind)) ||
            mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvAppearImmediately)) || mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvHide)) ||
            mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvHideAppear)) || MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25f) ||
            MR::isPlayerHipDropFalling() || MR::isPlayerHipDropLand()) {
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }

        if (!MR::sendMsgEnemyAttack(pReceiver, pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool DharmaSamboParts::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvKilled)) || mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvTrampleDeath))) {
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        if (!mIsHead) {
            MR::startSound(this, "SE_EM_DHARMASAMBO_TRAMPLE");
            return true;
        }

        if (mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvWaitUnderGround)) || mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvHideAppear))) {
            mHost->setNerve(GET_NERVE(DharmaSambo, HostTypeNrvAppearImmediately));
            MR::sendMsgAwayJump(pSender, pReceiver);
            return true;
        }

        if (!mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvAppear)) && !mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvAppearImmediately)) &&
            !mHost->isNerve(GET_NERVE(DharmaSambo, HostTypeNrvHide))) {
            mHost->setNerve(GET_NERVE(DharmaSambo, HostTypeNrvTrampleDeath));
            MR::startSound(this, "SE_EM_DHARMASAMBO_TRAMPLE");
            return true;
        }

        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        TVec3f direction(pSender->mPosition);
        direction -= pReceiver->mPosition;
        mHitDistance = direction.length();
        mHost->mHitSensor = pSender;
    }

    return false;
}

DharmaSambo::~DharmaSambo() {
}

DharmaSamboParts::~DharmaSamboParts() {
}
