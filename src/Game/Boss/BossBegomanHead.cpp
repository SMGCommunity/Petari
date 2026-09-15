#include "Game/Boss/BossBegomanHead.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LiveActorUtil.hpp"

void BossBegomanHead_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 hSpikeRotate = 0.23f;
    static const f32 hSpikeRotateTurn = 0.2f;
};  // namespace

namespace NrvBossBegomanHead {
    NEW_NERVE(HostTypeNrvDemoWait, BossBegomanHead, DemoWait);
    NEW_NERVE(HostTypeNrvOpeningDemo, BossBegomanHead, OpeningDemo);
    NEW_NERVE(HostTypeNrvOnWait, BossBegomanHead, OnWait);
    NEW_NERVE(HostTypeNrvOffWait, BossBegomanHead, OffWait);
    NEW_NERVE(HostTypeNrvSwitchOn, BossBegomanHead, SwitchOn);
    NEW_NERVE(HostTypeNrvSwitchOff, BossBegomanHead, SwitchOff);
    NEW_NERVE(HostTypeNrvTurn, BossBegomanHead, Turn);
    NEW_NERVE(HostTypeNrvTurnEnd, BossBegomanHead, TurnEnd);
};  // namespace NrvBossBegomanHead

BossBegomanHead::BossBegomanHead(LiveActor* pParent, MtxPtr pMtx)
    : PartsModel(pParent, "スイッチ頭", "BossBegomanHead", pMtx, MR::DrawBufferType_Enemy, false), mAngle(), mJointDelegator() {
}

void BossBegomanHead::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE(BossBegomanHead, HostTypeNrvDemoWait));

    mJointDelegator = MR::createJointDelegatorWithNullChildFunc(this, &BossBegomanHead::calcJointEdge, "Edge");

    PartsModel::init(rIter);
    MR::initLightCtrl(this);
    appear();
}

bool BossBegomanHead::isSwitchOn() {
    return isNerve(GET_NERVE(BossBegomanHead, HostTypeNrvDemoWait)) || isNerve(GET_NERVE(BossBegomanHead, HostTypeNrvOpeningDemo)) ||
           isNerve(GET_NERVE(BossBegomanHead, HostTypeNrvOnWait)) || isNerve(GET_NERVE(BossBegomanHead, HostTypeNrvSwitchOn));
}

bool BossBegomanHead::isEdgeOut() {
    return isNerve(GET_NERVE(BossBegomanHead, HostTypeNrvOffWait)) || isNerve(GET_NERVE(BossBegomanHead, HostTypeNrvSwitchOff)) ||
           isNerve(GET_NERVE(BossBegomanHead, HostTypeNrvTurn)) || isNerve(GET_NERVE(BossBegomanHead, HostTypeNrvTurnEnd));
}

void BossBegomanHead::setOpeningDemo() {
    setNerve(GET_NERVE(BossBegomanHead, HostTypeNrvOpeningDemo));
}

void BossBegomanHead::trySwitchPushTrample() {
    setNerve(GET_NERVE(BossBegomanHead, HostTypeNrvSwitchOn));
}

void BossBegomanHead::tryForceRecover() {
    setNerve(GET_NERVE(BossBegomanHead, HostTypeNrvSwitchOff));
}

void BossBegomanHead::tryTurn() {
    setNerve(GET_NERVE(BossBegomanHead, HostTypeNrvTurn));
}

void BossBegomanHead::tryTurnEnd() {
    setNerve(GET_NERVE(BossBegomanHead, HostTypeNrvTurnEnd));
}

void BossBegomanHead::exeDemoWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "DemoWait");
    }
}

void BossBegomanHead::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "OpeningDemo");
    }
}

void BossBegomanHead::exeOnWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "OnWait");
    }
}

void BossBegomanHead::exeOffWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "OffWait");
    }

    mAngle -= ::hSpikeRotate;
}

void BossBegomanHead::exeSwitchOn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "On");
    }

    MR::setNerveAtBckStopped(this, GET_NERVE(BossBegomanHead, HostTypeNrvOnWait));
}

void BossBegomanHead::exeSwitchOff() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Off");
    }

    MR::setNerveAtBckStopped(this, GET_NERVE(BossBegomanHead, HostTypeNrvOffWait));
}

void BossBegomanHead::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Turn");
    }

    mAngle -= ::hSpikeRotateTurn;
}

void BossBegomanHead::exeTurnEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "TurnEnd");
    }

    mAngle -= ::hSpikeRotate;

    MR::setNerveAtBckStopped(this, GET_NERVE(BossBegomanHead, HostTypeNrvOffWait));
}

void BossBegomanHead::calcAndSetBaseMtx() {
    PartsModel::calcAndSetBaseMtx();
    mJointDelegator->registerCallBack();
}

bool BossBegomanHead::calcJointEdge(TPos3f* pMtx, const JointControllerInfo&) {
    TPos3f v9;
    v9.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), mAngle);
    pMtx->concat(*pMtx, v9);

    return true;
}
