#include "Game/MapObj/HipDropRock.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void HipDropRock_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const s32 sStepToSwitchOn = 30;
    static const s32 sDefaultStarPeaceNum = 6;
};  // namespace

namespace NrvHipDropRock {
    NEW_NERVE(HipDropRockNrvWait, HipDropRock, Wait);
    NEW_NERVE(HipDropRockNrvBreak, HipDropRock, Break);
    NEW_NERVE(HipDropRockNrvWreck, HipDropRock, Wreck);
};  // namespace NrvHipDropRock

HipDropRock::HipDropRock(const char* pName) : LiveActor(pName), mBreakModel(), mItemType(-1), mStarPieceNum(-1), mObjArg7() {
    mBreakModelMtx.identity();
}

void HipDropRock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("HipDropRock", nullptr, false);
    initBreakModel();
    MR::connectToSceneMapObjStrongLight(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "HipDropRock", getSensor(nullptr), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    HipDropRock::initItem(rIter);
    MR::getJMapInfoArg7NoInit(rIter, &mObjArg7);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mBreakModel, rIter);
    }

    initNerve(GET_NERVE(HipDropRock, HipDropRockNrvWait));
    makeActorAppeared();
}

void HipDropRock::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }
}

void HipDropRock::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ROCK_BREAK");
        MR::invalidateCollisionParts(this);
        MR::invalidateClipping(this);

        if (mObjArg7) {
            MR::hideModel(this);
        } else {
            MR::startBva(this, "BreakLevel");
        }

        mBreakModel->makeActorAppeared();
        MR::invalidateClipping(mBreakModel);
        MR::startBck(mBreakModel, "HipDropRockBreak");

        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }

        HipDropRock::appearItem();
    }

    if (MR::isStep(this, ::sStepToSwitchOn) && MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (MR::isBckStopped(mBreakModel) && MR::isGreaterStep(this, ::sStepToSwitchOn)) {
        mBreakModel->kill();

        if (mObjArg7) {
            kill();
        } else {
            setNerve(GET_NERVE(HipDropRock, HipDropRockNrvWreck));
        }
    }
}

void HipDropRock::exeWreck() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }
}

bool HipDropRock::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(HipDropRock, HipDropRockNrvBreak))) {
        return false;
    }

    if (MR::isMsgPlayerHipDropFloor(msg)) {
        setNerve(GET_NERVE(HipDropRock, HipDropRockNrvBreak));

        return true;
    }

    return false;
}

void HipDropRock::initBreakModel() {
    mBreakModelMtx.set(getBaseMtx());
    TVec3f v1;
    TVec3f v2;
    v1.x = 0.0f;
    v1.y = 1.0f;
    v1.z = 0.0f;
    v2.scale(MR::getRandom(0.0f, 180.0f), v1);
    TPos3f mtx;
    MR::makeMtxMoment(&mtx, v2);
    mBreakModelMtx.concat(mtx);

    mBreakModel = MR::createModelObjMapObj("ヒビ石壊れモデル", "HipDropRockBreak", mBreakModelMtx);
    mBreakModel->initWithoutIter();
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
}

void HipDropRock::initItem(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &mItemType);
    MR::getJMapInfoArg1NoInit(rIter, &mStarPieceNum);

    if (mItemType == 1 && mStarPieceNum == -1) {
        mStarPieceNum = ::sDefaultStarPeaceNum;
    }

    if (mItemType == 0) {
        MR::declareCoin(this, 1);
    } else if (mItemType == 1) {
        MR::declareStarPiece(this, mStarPieceNum);
    }
}

void HipDropRock::appearItem() {
    if (mItemType == -1) {
        return;
    }

    if (mItemType == 0) {
        MR::appearCoinPop(this, mPosition, 1);
    } else if (mItemType == 1) {
        MR::appearStarPiece(this, mPosition, mStarPieceNum, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    }
}
