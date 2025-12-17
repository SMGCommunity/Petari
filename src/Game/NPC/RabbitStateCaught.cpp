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
#include <JSystem/JMath/JMath.hpp>

namespace {
    static const f32 sGravityAccel = 1.0f;
    static const s32 sMarioPoseBlendTime = 5;
    static const s32 sCaughtLandStartTime = 20;
    static const f32 sCaughtJumpPower = 20.0f;
};  // namespace

namespace NrvRabbitStateCaught {
    NEW_NERVE(RabbitStateCaughtNrvTryStartDemo, RabbitStateCaught, TryStartDemo);
    NEW_NERVE(RabbitStateCaughtNrvCaught, RabbitStateCaught, Caught);
    NEW_NERVE(RabbitStateCaughtNrvCaughtLand, RabbitStateCaught, CaughtLand);
    NEW_NERVE(RabbitStateCaughtNrvCaughtEvent, RabbitStateCaught, CaughtEvent);
};  // namespace NrvRabbitStateCaught

RabbitStateCaught::RabbitStateCaught(LiveActor* pHost, TalkMessageCtrl* pTalkMessageCtrl)
    : ActorStateBaseInterface("うさぎ捕まり状態"), mHost(pHost), mCaughtStartMarioRot(0.0f, 0.0f, 0.0f, 1.0f), mCaughtStartMarioPos(0.0f, 0.0f, 0.0f),
      mTalkMessageCtrl(pTalkMessageCtrl), mPowerStarModel(nullptr), mUsePowerStarModel(true) {
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

void RabbitStateCaught::control() {}

void RabbitStateCaught::exeTryStartDemo() {}

void RabbitStateCaught::exeCaught() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Caught");
        MR::startBckPlayer("Catch", static_cast< const char* >(nullptr));
        MR::zeroVelocity(mHost);
        MR::startSound(mHost, "SE_SM_RABBIT_CAUGHT", -1, -1);
        setCaughtStartMarioPose();
    }

    if (!MR::isBindedGround(mHost)) {
        MR::addVelocityToGravity(mHost, sGravityAccel);
    }

    MR::attenuateVelocity(mHost, 0.99f);
    blendBaseMatrixToMario(MR::calcNerveRate(this, sMarioPoseBlendTime));

    if (MR::isStep(this, sMarioPoseBlendTime)) {
        MR::setVelocityJump(mHost, sCaughtJumpPower);
    }

    if (MR::isGreaterEqualStep(this, sCaughtLandStartTime) && MR::isBindedGround(mHost)) {
        MR::zeroVelocity(mHost);
        setNerve(&NrvRabbitStateCaught::RabbitStateCaughtNrvCaughtLand::sInstance);
    }
}

void RabbitStateCaught::exeCaughtLand() {
    TMtx34f baseMtx;

    if (MR::isFirstStep(this)) {
        JMath::gekko_ps_copy12(&baseMtx, mHost->getBaseMtx());
        MR::setPlayerBaseMtx(baseMtx);
        JMath::gekko_ps_copy12(&mCaughtLandMarioBaseMtx, &baseMtx);

        if (mPowerStarModel != nullptr) {
            mPowerStarModel->appear();
            MR::requestMovementOn(mPowerStarModel);
            MR::startAction(mPowerStarModel, "TrickRabbitLand");
        }

        MR::startAction(mHost, "CaughtLand");
        MR::startBckPlayer("CatchLand", static_cast< const char* >(nullptr));
    }

    MR::startLevelSound(mHost, "SE_SM_LV_RABBIT_STRUGGLE", -1, -1, -1);

    if (MR::isBckStopped(mHost)) {
        if (mTalkMessageCtrl == nullptr) {
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

    MR::startLevelSound(mHost, "SE_SM_LV_RABBIT_STRUGGLE", -1, -1, -1);

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mTalkMessageCtrl)) {
        if (mPowerStarModel != nullptr) {
            mPowerStarModel->kill();
        }

        kill();
    }
}

void RabbitStateCaught::setCaughtStartMarioPose() {
    TPos3f baseMtx;

    JMath::gekko_ps_copy12(&baseMtx, MR::getPlayerDemoActor()->getBaseMtx());
    baseMtx.getQuat(mCaughtStartMarioRot);
    baseMtx.getTransInline(mCaughtStartMarioPos);
}

void RabbitStateCaught::blendBaseMatrixToMario(f32 blendRate) const {
    TMtx34f baseMtx;
    TMtx34f finalBaseMtx;

    if (blendRate >= 1.0f) {
        JMath::gekko_ps_copy12(&finalBaseMtx, mHost->getBaseMtx());
        MR::setPlayerBaseMtx(finalBaseMtx);
    } else {
        baseMtx.identity();
        MR::blendMtx(MR::getPlayerBaseMtx(), mHost->getBaseMtx(), blendRate, baseMtx);
        MR::setPlayerBaseMtx(baseMtx);
    }
}
