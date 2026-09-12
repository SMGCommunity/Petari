#include "Game/MapObj/PomponPlant.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace {
    const Vec sSensorOffset = {55.0f, 170.0f, -100.0f};
    const f32 sSensorRadius = 70.0f;
    const f32 sItemSpeed = 20.0f;
    const f32 sPlayerVelFactor = 0.7f;
}  // namespace

namespace NrvPomponPlant {
    NEW_NERVE(HostTypeWait, PomponPlant, Wait);
    NEW_NERVE(HostTypeSwingDpd, PomponPlant, SwingDpd);
    NEW_NERVE(HostTypeSwingPlayer, PomponPlant, SwingPlayer);
}  // namespace NrvPomponPlant

PomponPlant::PomponPlant(const char* pName) : LiveActor(pName), mItemType(-1), mItemCount(1), mCanGenItem(true) {
}

void PomponPlant::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* modelName = nullptr;
    MR::getObjectName(&modelName, rIter);
    initModelManagerWithAnm(modelName, nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, sSensorRadius, TVec3f(sSensorOffset));
    MR::initStarPointerTarget(this, sSensorRadius, TVec3f(sSensorOffset));
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::getJMapInfoArg0NoInit(rIter, &mItemType);
    MR::getJMapInfoArg1NoInit(rIter, &mItemCount);

    if (mItemType == -1) {
        MR::declareCoin(this, mItemCount);
    } else if (mItemType == 0) {
        MR::declareStarPiece(this, mItemCount);
    }

    initNerve(GET_NERVE(PomponPlant, HostTypeWait));
    makeActorAppeared();
}

void PomponPlant::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && !isNerve(GET_NERVE(PomponPlant, HostTypeSwingPlayer))) {
        if (MR::sendMsgTouchJump(pReceiver, pSender)) {
            setNerve(GET_NERVE(PomponPlant, HostTypeSwingPlayer));
        }
    }
}

bool PomponPlant::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        setNerve(GET_NERVE(PomponPlant, HostTypeSwingPlayer));
        return true;
    }

    return false;
}

bool PomponPlant::tryGenItem() {
    if (!mCanGenItem) {
        return false;
    }

    TVec3f gravity;
    MR::calcGravityVector(this, &gravity, nullptr, 0);
    TVec3f playerMove(*MR::getPlayerLastMove());
    MR::vecKillElement(playerMove, -gravity, &playerMove);
    TVec3f velocity(-gravity * sItemSpeed + playerMove * sPlayerVelFactor);

    if (mItemType == 0) {
        MR::appearStarPiece(this, getSensor("body")->mPosition, mItemCount, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    } else {
        MR::appearCoinPop(this, getSensor("body")->mPosition, mItemCount);
    }

    mCanGenItem = false;
    return true;
}

void PomponPlant::exeWait() {
    if (MR::tryStarPointerCheckWithoutRumble(this, false)) {
        setNerve(GET_NERVE(PomponPlant, HostTypeSwingDpd));
    }
}

void PomponPlant::exeSwingDpd() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_POMPON_PLANT_DPD_HIT");
        MR::startBck(this, "DpdHit", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PomponPlant, HostTypeWait));
    }
}

void PomponPlant::exeSwingPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_POMPON_PLANT_HIT");
        MR::startBck(this, "Hit", nullptr);
        tryGenItem();
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(PomponPlant, HostTypeWait));
    }
}

PomponPlant::~PomponPlant() {
}
