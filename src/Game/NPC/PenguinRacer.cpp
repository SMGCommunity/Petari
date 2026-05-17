#include "Game/NPC/PenguinRacer.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/RaceRail.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/NPC/NPCActorItem.hpp"

namespace {
    // static const f32 sBrake = ;
    static const f32 sGoalBrake = 0.005f;
    static const f32 sFallBrake = 0.002f;
    static const s32 sTurnFrame = 60;
    static const s32 sFallFrame = 240;
    static const f32 sSensorRadius = 80.0f;
    static const f32 sNearClipping = 200.0f;
    static const f32 sSwimSoundPlayDistance = 500.0f;

    // TODO: this is an exact copy of MR::isInWater, except it needs to be inlined.
    inline bool isInWater(const TVec3f& rPos) {
        WaterInfo waterInfo;
        return MR::getWaterAreaObj(&waterInfo, rPos);
    }
}  // namespace

namespace NrvPenguinRacer {
    NEW_NERVE(PenguinRacerNrvWait, PenguinRacer, Wait);
    NEW_NERVE(PenguinRacerNrvReaction, PenguinRacer, Reaction);
    NEW_NERVE(PenguinRacerNrvRacePrep, PenguinRacer, RaceWait);
    NEW_NERVE(PenguinRacerNrvRaceWalk, PenguinRacer, RaceWalk);
    NEW_NERVE(PenguinRacerNrvRaceSwim, PenguinRacer, RaceSwim);
    NEW_NERVE(PenguinRacerNrvRaceStoW, PenguinRacer, RaceStoW);
    NEW_NERVE(PenguinRacerNrvRaceWtoS, PenguinRacer, RaceWtoS);
    NEW_NERVE(PenguinRacerNrvRaceFall, PenguinRacer, RaceFall);
    NEW_NERVE(PenguinRacerNrvRaceGoal, PenguinRacer, RaceGoal);
    NEW_NERVE(PenguinRacerNrvRaceTurn, PenguinRacer, RaceTurn);
}  // namespace NrvPenguinRacer

PenguinRacer::PenguinRacer(const char* pName) : NPCActor(pName), mRaceDisabled(0), mMoveSpeed(0.0f), mSpeed(1.0f), mRailPointNo(0) {
}

void PenguinRacer::init(const JMapInfoIter& rIter) {
    MR::initDefaultPose(this, rIter);
    initModel();

    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);

    initSensor();

    initSound(8, false);
    initShadow();
    initBind();
    initEffectKeeper(0, nullptr, false);
    MR::onCalcGravity(this);

    initColor(rIter);

    TVec3f target;
    target.set(0.0f);
    MR::initStarPointerTarget(this, 100.0f, target);
    initEtc(rIter);

    mMsgCtrl = MR::createTalkCtrl(this, rIter, "PenguinRacer", TVec3f(0.0f, 130.0f, 0.0f), nullptr);
    MR::onRootNodeAutomatic(mMsgCtrl);

    initNerve(&NrvPenguinRacer::PenguinRacerNrvWait::sInstance);

    if (mRaceDisabled == 0) {
        initRailRider(rIter);
        RaceManagerFunction::entryRacerOthers(this);
    }

    MR::addToAttributeGroupSearchTurtle(this);
    mLodCtrl = MR::createLodCtrlNPC(this, rIter);
    makeActorAppeared();

    NPCActorItem item("PenguinRacer");
    MR::getNPCItemData(&item, 0);
    equipment(item, false);
}

void PenguinRacer::initModel() {
    initModelManagerWithAnm("Penguin", nullptr, false);
}

void PenguinRacer::initSensor() {
    initHitSensor(1);
    MR::addHitSensorPush(this, "push", 8, sSensorRadius, TVec3f(0.0f, sSensorRadius, 0.0f));
}

void PenguinRacer::initShadow() {
    MR::initShadowVolumeSphere(this, 50.0f);
}

void PenguinRacer::initBind() {
    initBinder(sSensorRadius, sSensorRadius, 0);
}

void PenguinRacer::initColor(const JMapInfoIter& rIter) {
    s32 color = 0;
    MR::getJMapInfoArg7NoInit(rIter, &color);

    MR::startBrk(this, "ColorChange");
    MR::setBrkFrameAndStop(this, color);
}

void PenguinRacer::initEtc(const JMapInfoIter& rIter) {
    mParam.setMoveAction("RacerWait", "Turn");
    mParam.setTalkAction("RacerTalk", "TalkTurn");
    setDefaults();
    MR::getJMapInfoArg0NoInit(rIter, &mRaceDisabled);
}

bool PenguinRacer::tryReaction() {
    return MR::tryStartReactionAndPushNerve(this, &NrvPenguinRacer::PenguinRacerNrvReaction::sInstance);
}

void PenguinRacer::exeReaction() {
    if (_D8) {
        MR::startSound(this, "SE_SM_PENGUIN_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_PENGUIN_S_TRAMPLED", -1, -1);
    }

    if (isPointingSe()) {
        MR::startSound(this, "SE_SV_PENGUIN_S_POINTED_2P", -1, -1);
        MR::startDPDHitSound();
    }

    if (_D9) {
        MR::startSound(this, "SE_SV_PENGUIN_S_SPIN_HIT", -1, -1);
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_PENGUIN_S_STAR_PIECE", -1, -1);
    }

    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

bool PenguinRacer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if ((MR::isMsgJetTurtleAttack(msg) || MR::isMsgStarPieceReflect(msg)) && isInRace()) {
        setNerve(&NrvPenguinRacer::PenguinRacerNrvRaceFall::sInstance);
        return true;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return isInRace();
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

void PenguinRacer::control() {
    if (mLodCtrl != nullptr) {
        if (MR::calcCameraDistanceZ(mPosition) < sNearClipping) {
            if (!MR::isHiddenModel(this)) {
                mLodCtrl->invalidate();
                MR::hideModelAndOnCalcAnim(this);
                if (_94 != nullptr) {
                    MR::hideModel(_94);
                }
                if (_98 != nullptr) {
                    MR::hideModel(_98);
                }
            }
        } else {
            if (MR::isHiddenModel(this)) {
                MR::showModel(this);
                if (_94 != nullptr) {
                    MR::showModel(_94);
                }
                if (_98 != nullptr) {
                    MR::showModel(_98);
                }
                mLodCtrl->validate();
            }
        }
    }
    NPCActor::control();
}

void PenguinRacer::exeWait() {
    MR::startLevelSound(this, "SE_SV_LV_PENGUIN_S_CHAT", -1, -1, -1);

    if (!tryReaction()) {
        MR::tryTalkNearPlayerAndStartTalkAction(this);
    }
}

void PenguinRacer::exeRaceWait() {
    if (MR::isFirstStep(this)) {
        if (isInWater(mPosition)) {
            MR::startAction(this, "RacerSwimWaitSurface");
        } else {
            MR::startAction(this, "RacerWait");
        }
        MR::setBckFrameAtRandom(this);
    }
    MR::attenuateVelocity(this, 0.9f);
}

void PenguinRacer::exeRaceWalk() {
    if (MR::isFirstStep(this)) {
        if (!MR::isBckPlaying(this, "RacerLandSwim")) {
            MR::startAction(this, "RacerLandSwim");
            MR::setBckFrameAtRandom(this);
        }
    }

    MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SLIDE", -1, -1, -1);

    if (MR::isNearPlayer(this, sSwimSoundPlayDistance)) {
        MR::startLevelSound(this, "SE_SV_LV_PENGUIN_S_RACE_SWIM", -1, -1, -1);
    }

    updateMove();
}

void PenguinRacer::exeRaceSwim() {
    if (MR::isFirstStep(this)) {
        if (!MR::isActionStart(this, "RacerSwim")) {
            MR::startAction(this, "RacerSwim");
            MR::setBckFrameAtRandom(this);
        }
    }

    if (MR::isNearPlayer(this, sSwimSoundPlayDistance)) {
        MR::startLevelSound(this, "SE_SV_LV_PENGUIN_S_RACE_SWIM", -1, -1, -1);
    }

    updateMove();
}

void PenguinRacer::exeRaceStoW() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "RacerLandSwimSlow");
        MR::startSound(this, "SE_SM_PENGUIN_WATER_OUT", -1, -1);
    }

    updateMove();

    if (!isNerve(&NrvPenguinRacer::PenguinRacerNrvRaceStoW::sInstance)) {
        MR::emitEffect(this, "WaterColumn");
    }
}

void PenguinRacer::exeRaceWtoS() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "RacerSwimSlow");
        MR::startSound(this, "SE_SM_PENGUIN_WATER_IN", -1, -1);
    }

    updateMove();

    if (!isNerve(&NrvPenguinRacer::PenguinRacerNrvRaceWtoS::sInstance)) {
        MR::emitEffect(this, "WaterColumn");
    }
}

void PenguinRacer::exeRaceFall() {
    if (MR::isFirstStep(this)) {
        if (MR::isActionStart(this, "RacerSwim")) {
        }
        MR::startAction(this, "RacerSwimDamage");
        MR::startSound(this, "SE_SV_PENGUIN_S_DAMAGE_SHELL", -1, -1);
    }

    MR::startLevelSound(this, "SE_SM_LV_PENGUIN_S_SWOON", -1, -1, -1);

    mSpeed -= sFallBrake;
    if (mSpeed < 0.0f) {
        mSpeed = 0.0f;
    }
    MR::attenuateVelocity(this, 0.9f);

    updateMove();

    if (getNerveStep() >= sFallFrame) {
        mRailPointNo = MR::getCurrentRailPointNo(this);
        mMoveSpeed = getMoveSpeed();
        MR::startSound(this, "SE_SV_PENGUIN_S_HEAT_UP", -1, -1);
        setNerve(getMoveNerve());
    }
}

void PenguinRacer::exeRaceGoal() {
    mSpeed -= sGoalBrake;
    if (mSpeed < 0.0f) {
        mSpeed = 0.0f;
    }

    MR::attenuateVelocity(this, 0.9f);

    if (mVelocity.length() > 5.0f) {
        updateVelocity();

        if (getMoveNerve() != &NrvPenguinRacer::PenguinRacerNrvRaceSwim::sInstance && !MR::isBindedGround(this)) {
            MR::addVelocityToGravity(this, 1.0f);
        }

        if (getMoveNerve() != &NrvPenguinRacer::PenguinRacerNrvRaceSwim::sInstance) {
            MR::onBind(this);
        }

        MR::decidePose(this, -mGravity, MR::getRailDirection(this), mPosition, 1.0f, 0.1f, 1.0f);
    } else {
        mVelocity.zero();
        setNerve(&NrvPenguinRacer::PenguinRacerNrvRaceTurn::sInstance);
    }
}

void PenguinRacer::exeRaceTurn() {
    if (MR::isFirstStep(this)) {
        if (isInWater(mPosition)) {
            MR::startAction(this, "RacerSwimWaitSurface");
        } else {
            MR::startAction(this, "RacerWait");
        }
    }

    if (getNerveStep() < sTurnFrame) {
        mRotation.y = MR::repeatDegree(mRotation.y + 3.0f);
    }
}

void PenguinRacer::updateMove() {
    updateVelocity();

    if (getMoveNerve() != &NrvPenguinRacer::PenguinRacerNrvRaceSwim::sInstance && !MR::isBindedGround(this)) {
        MR::addVelocityToGravity(this, 1.0f);
    }

    if (getMoveNerve() != &NrvPenguinRacer::PenguinRacerNrvRaceSwim::sInstance) {
        MR::onBind(this);
    }

    updateRail();

    MR::decidePose(this, -mGravity, MR::getRailDirection(this), mPosition, 1.0f, 0.1f, 1.0f);
}

void PenguinRacer::updateVelocity() {
    MR::moveCoordToNearestPos(this, mPosition);
    TVec3f nextPointPos;
    MR::calcRailPointPos(&nextPointPos, this, MR::getNextRailPointNo(this));

    TVec3f toTarget = nextPointPos - mPosition;
    MR::normalizeOrZero(&toTarget);

    mVelocity.add(toTarget.scaleInline(mSpeed));

    TVec3f velV, velH;
    TVec3f grav = mGravity;

    velV.zero();
    velH.zero();

    if (getMoveNerve() != &NrvPenguinRacer::PenguinRacerNrvRaceSwim::sInstance) {
        MR::normalizeOrZero(&grav);
        const TVec3f& v = mVelocity;
        velV.scale(grav.dot(v), grav);
        velH.subInline(v, velV);
    } else {
        velV.zero();
        velH.set(mVelocity);
    }

    f32 speedMaxV = mMoveSpeed;
    f32 speedMaxH = mMoveSpeed;

    if (getMoveNerve() != &NrvPenguinRacer::PenguinRacerNrvRaceSwim::sInstance) {
        speedMaxH = mMoveSpeed;
        speedMaxV = mMoveSpeed * 5.0f;
    }

    if (velV.squared() > speedMaxV * speedMaxV) {
        velV.setLength(speedMaxV);
    }

    if (velH.squared() > speedMaxH * speedMaxH) {
        velH.setLength(speedMaxH);
    }

    mVelocity.set(velV.addOperatorInLine(velH));
}

void PenguinRacer::updateRail() {
    if (MR::getCurrentRailPointNo(this) <= mRailPointNo) {
        return;
    }

    mRailPointNo = MR::getCurrentRailPointNo(this);
    mMoveSpeed = getMoveSpeed();

    if (isNerve(&NrvPenguinRacer::PenguinRacerNrvRaceFall::sInstance)) {
        return;
    }

    const Nerve* moveNerve = getMoveNerve();
    if (!isNerve(moveNerve)) {
        setNerve(moveNerve);
    }
}

const Nerve* PenguinRacer::getMoveNerve() const {
    // Note: 0 means rail section is a "swim" section, non-0 means section is a "walk" section

    s32 curr = 0;
    s32 next = 0;
    MR::getCurrentRailPointArg1NoInit(this, &curr);
    MR::getNextRailPointArg1NoInit(this, &next);

    if (curr != 0 && next != 0) {
        return &NrvPenguinRacer::PenguinRacerNrvRaceWalk::sInstance;
    } else if (curr == 0 && next == 0) {
        return &NrvPenguinRacer::PenguinRacerNrvRaceSwim::sInstance;
    } else if (curr != 0 && next == 0) {
        return &NrvPenguinRacer::PenguinRacerNrvRaceWtoS::sInstance;
    } else {  // (curr == 0 && next != 0)
        return &NrvPenguinRacer::PenguinRacerNrvRaceStoW::sInstance;
    }
}

f32 PenguinRacer::getMoveSpeed() const {
    f32 moveSpeed = mMoveSpeed;
    MR::getCurrentRailPointArg0NoInit(this, &moveSpeed);
    return moveSpeed;
}

bool PenguinRacer::isInRace() const {
    if (isNerve(&NrvPenguinRacer::PenguinRacerNrvRaceWalk::sInstance) || isNerve(&NrvPenguinRacer::PenguinRacerNrvRaceSwim::sInstance) ||
        isNerve(&NrvPenguinRacer::PenguinRacerNrvRaceStoW::sInstance) || isNerve(&NrvPenguinRacer::PenguinRacerNrvRaceWtoS::sInstance)) {
        return true;
    }
    return false;
}

void PenguinRacer::initRacer() {
    AbstractRacer::initRacer();
}

void PenguinRacer::prepRacer(const RaceManager* pRaceManager) {
    MR::invalidateClipping(this);
    mBinder->_1C = 60.0f;

    MR::moveCoordToStartPos(this);
    MR::moveTransToCurrentRailPos(this);
    mRailPointNo = 0;
    mMoveSpeed = getMoveSpeed();
    mSpeed = 1.0f;

    MR::decidePose(this, -mGravity, MR::getRailDirection(this), mPosition, 1.0f, 1.0f, 1.0f);

    if (isInWater(mPosition)) {
        MR::startAction(this, "RacerSwimSurfaceSoon");
    } else {
        TVec3f pos;
        Triangle triangle;
        if (MR::getFirstPolyOnLineToMap(&pos, &triangle, mPosition, mGravity.scaleInline(1000.0f))) {
            mPosition.set(pos);
        }
        MR::startAction(this, "RacerWalkSoon");
    }

    MR::setBckFrameAtRandom(this);
    MR::resetPosition(this);
    MR::requestMovementOn(this);
    if (_94 != nullptr) {
        MR::requestMovementOn(_94);
    }
    if (_98 != nullptr) {
        MR::requestMovementOn(_98);
    }

    setNerve(&NrvPenguinRacer::PenguinRacerNrvRacePrep::sInstance);
}

void PenguinRacer::startRacer() {
    setNerve(getMoveNerve());
}

bool PenguinRacer::updateRacer(const RaceManager* pRaceManager) {
    f32 coord = MR::calcNearestRailCoord(pRaceManager->mPlayerRacer, mPosition);
    f32 length = MR::getRailTotalLength(pRaceManager->mPlayerRacer);
    mPrevPosition.set(mCurrPosition);
    mCurrPosition.set(mPosition);
    if ((f32)__fabs(mRailCoord - coord) < 2000.0f) {
        mRailCoord = coord;
    }

    if ((f32)__fabs(mRailCoord - length) < 100.0f) {
        return pRaceManager->isGoal(this);
    }

    return false;
}

bool PenguinRacer::goalRacer() {
    setNerve(&NrvPenguinRacer::PenguinRacerNrvRaceGoal::sInstance);
    return false;
}

void PenguinRacer::loseRacer() {
    setNerve(&NrvPenguinRacer::PenguinRacerNrvRacePrep::sInstance);
}

void PenguinRacer::resetRacer(const RaceManager* pRaceManager) {
    MR::onBind(this);
    mBinder->_1C = 60.0f;
    MR::setDefaultPose(this);
    mVelocity.zero();
    MR::calcGravity(this);

    if (isInWater(mPosition)) {
        MR::startAction(this, "RacerSwimWaitSoon");
    } else {
        MR::startAction(this, "RacerWaitSoon");
    }

    MR::deleteEffectAll(this);
    setNerve(&NrvPenguinRacer::PenguinRacerNrvWait::sInstance);
}

void PenguinRacer::exitRacer() {
    MR::validateClipping(this);
}
