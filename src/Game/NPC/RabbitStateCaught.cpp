#include "Game/NPC/RabbitStateCaught.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

void RabbitStateCaught_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    // static const f32 sGravityAccel = _;
    // static const f32 sBodyRadius = _;
    static const s32 sMarioPoseBlendTime = 5;
    static const s32 sCaughtLandStartTime = 20;
    // static const s32 sCaughtEventTime = _;
    static const f32 sCaughtJumpPower = 20.0f;
    static const f32 sCaughtGravityAccel = 1.0f;
    static const f32 sCaughtAirFreq = 0.99f;
};  // namespace

namespace NrvRabbitStateCaught {
    NEW_NERVE(RabbitStateCaughtNrvTryStartDemo, RabbitStateCaught, TryStartDemo);
    NEW_NERVE(RabbitStateCaughtNrvCaught, RabbitStateCaught, Caught);
    NEW_NERVE(RabbitStateCaughtNrvCaughtLand, RabbitStateCaught, CaughtLand);
    NEW_NERVE(RabbitStateCaughtNrvCaughtEvent, RabbitStateCaught, CaughtEvent);
};  // namespace NrvRabbitStateCaught

RabbitStateCaught::RabbitStateCaught(LiveActor* pHost, TalkMessageCtrl* pTalkCtrl)
    : ActorStateBase("うさぎ捕まり状態", pHost), mCaughtStartMarioQuat(0.0f, 0.0f, 0.0f, 1.0f), mCaughtStartMarioTrans(0.0f, 0.0f, 0.0f),
      mTalkCtrl(pTalkCtrl), mPowerStarModel(), mUsePowerStarModel(true) {
    mCaughtLandMarioBaseMtx.identity();
}

void RabbitStateCaught::unusePowerStarModel() {
    mUsePowerStarModel = false;
}

void RabbitStateCaught::init() {
    initNerve(&NrvRabbitStateCaught::RabbitStateCaughtNrvCaught::sInstance);

    if (mUsePowerStarModel) {
        mPowerStarModel = MR::createPowerStarDemoModel(mHost, "パワースターデモモデル", mCaughtLandMarioBaseMtx);
        mPowerStarModel->initWithoutIter();
        MR::invalidateClipping(mPowerStarModel);
        mPowerStarModel->makeActorDead();
    }
}

void RabbitStateCaught::appear() {
    mIsDead = false;

    MR::requestStartDemoMarioPuppetable(this, mHost, "捕まり", &NrvRabbitStateCaught::RabbitStateCaughtNrvCaught::sInstance,
                                        &NrvRabbitStateCaught::RabbitStateCaughtNrvTryStartDemo::sInstance);
}

void RabbitStateCaught::control() {
}

void RabbitStateCaught::exeTryStartDemo() {
}

void RabbitStateCaught::exeCaught() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Caught");
        MR::startBckPlayer("Catch", static_cast< const char* >(nullptr));
        MR::zeroVelocity(mHost);
        MR::startSound(mHost, "SE_SM_RABBIT_CAUGHT");
        setCaughtStartMarioPose();
    }

    if (!MR::isBindedGround(mHost)) {
        MR::addVelocityToGravity(mHost, ::sCaughtGravityAccel);
    }

    MR::attenuateVelocity(mHost, ::sCaughtAirFreq);
    blendBaseMatrixToMario(MR::calcNerveRate(this, ::sMarioPoseBlendTime));

    if (MR::isStep(this, ::sMarioPoseBlendTime)) {
        MR::setVelocityJump(mHost, ::sCaughtJumpPower);
    }

    if (MR::isGreaterEqualStep(this, ::sCaughtLandStartTime) && MR::isBindedGround(mHost)) {
        MR::zeroVelocity(mHost);
        setNerve(&NrvRabbitStateCaught::RabbitStateCaughtNrvCaughtLand::sInstance);
    }
}

void RabbitStateCaught::exeCaughtLand() {
    TMtx34f baseMtx;

    if (MR::isFirstStep(this)) {
        baseMtx.set(mHost->getBaseMtx());
        MR::setPlayerBaseMtx(baseMtx);
        mCaughtLandMarioBaseMtx.set(baseMtx);

        if (mPowerStarModel != nullptr) {
            mPowerStarModel->appear();
            MR::requestMovementOn(mPowerStarModel);
            MR::startAction(mPowerStarModel, "TrickRabbitLand");
        }

        MR::startAction(mHost, "CaughtLand");
        MR::startBckPlayer("CatchLand", static_cast< const char* >(nullptr));
    }

    MR::startLevelSound(mHost, "SE_SM_LV_RABBIT_STRUGGLE");

    if (MR::isBckStopped(mHost)) {
        if (mTalkCtrl == nullptr) {
            kill();
        } else {
            setNerve(&NrvRabbitStateCaught::RabbitStateCaughtNrvCaughtEvent::sInstance);
        }
    }
}

void RabbitStateCaught::exeCaughtEvent() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "CaughtWait");
        MR::startBckPlayer("CatchWait", static_cast< const char* >(nullptr));

        if (mPowerStarModel != nullptr) {
            MR::requestMovementOn(mPowerStarModel);
            MR::startAction(mPowerStarModel, "TrickRabbitWait");
        }
    }

    MR::startLevelSound(mHost, "SE_SM_LV_RABBIT_STRUGGLE");

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mTalkCtrl)) {
        if (mPowerStarModel != nullptr) {
            mPowerStarModel->kill();
        }

        kill();
    }
}

void RabbitStateCaught::setCaughtStartMarioPose() {
    TPos3f baseMtx(MR::getPlayerDemoActor()->getBaseMtx());
    baseMtx.getQuat(mCaughtStartMarioQuat);
    baseMtx.getTrans(mCaughtStartMarioTrans);
}

void RabbitStateCaught::blendBaseMatrixToMario(f32 blendRate) const {
    TMtx34f baseMtx;
    TMtx34f finalBaseMtx;

    if (blendRate >= 1.0f) {
        finalBaseMtx.set(mHost->getBaseMtx());
        MR::setPlayerBaseMtx(finalBaseMtx);
    } else {
        baseMtx.identity();
        MR::blendMtx(MR::getPlayerBaseMtx(), mHost->getBaseMtx(), blendRate, baseMtx);
        MR::setPlayerBaseMtx(baseMtx);
    }
}
