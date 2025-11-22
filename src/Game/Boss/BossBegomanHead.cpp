#include "Game/Boss/BossBegomanHead.hpp"
#include "Game/Util/JointController.hpp"

namespace NrvBossBegomanHead {
    NEW_NERVE(HostTypeNrvDemoWait, BossBegomanHead, DemoWait);
    NEW_NERVE(HostTypeNrvOpeningDemo, BossBegomanHead, OpeningDemo);
    NEW_NERVE(HostTypeNrvOnWait, BossBegomanHead, OnWait);
    NEW_NERVE(HostTypeNrvOffWait, BossBegomanHead, OffWait);
    NEW_NERVE(HostTypeNrvSwitchOn, BossBegomanHead, SwitchOn);
    NEW_NERVE(HostTypeNrvSwitchOff, BossBegomanHead, SwitchOff);
    NEW_NERVE(HostTypeNrvTurn, BossBegomanHead, Turn);
    NEW_NERVE(HostTypeNrvTurnEnd, BossBegomanHead, TurnEnd);
}; // namespace NrvBossBegomanHead

BossBegomanHead::BossBegomanHead(LiveActor* pParent, MtxPtr pMtx)
    : PartsModel(pParent, "スイッチ頭", "BossBegomanHead", pMtx, 0x12, false),
      _9C(0.0f),
      mJointDelegator(nullptr) {
}

void BossBegomanHead::init(const JMapInfoIter& rIter) {
    initNerve(&NrvBossBegomanHead::HostTypeNrvDemoWait::sInstance);

    mJointDelegator = MR::createJointDelegatorWithNullChildFunc(this, &BossBegomanHead::calcJointEdge, "Edge");

    PartsModel::init(rIter);
    MR::initLightCtrl(this);
    appear();
}

bool BossBegomanHead::isSwitchOn() {
    return isNerve(&NrvBossBegomanHead::HostTypeNrvDemoWait::sInstance) || isNerve(&NrvBossBegomanHead::HostTypeNrvOpeningDemo::sInstance) || isNerve(&NrvBossBegomanHead::HostTypeNrvOnWait::sInstance) || isNerve(&NrvBossBegomanHead::HostTypeNrvSwitchOn::sInstance);
}

bool BossBegomanHead::isEdgeOut() {
    return isNerve(&NrvBossBegomanHead::HostTypeNrvOffWait::sInstance) || isNerve(&NrvBossBegomanHead::HostTypeNrvSwitchOff::sInstance) || isNerve(&NrvBossBegomanHead::HostTypeNrvTurn::sInstance) || isNerve(&NrvBossBegomanHead::HostTypeNrvTurnEnd::sInstance);
}

void BossBegomanHead::setOpeningDemo() {
    setNerve(&NrvBossBegomanHead::HostTypeNrvOpeningDemo::sInstance);
}

void BossBegomanHead::trySwitchPushTrample() {
    setNerve(&NrvBossBegomanHead::HostTypeNrvSwitchOn::sInstance);
}

void BossBegomanHead::tryForceRecover() {
    setNerve(&NrvBossBegomanHead::HostTypeNrvSwitchOff::sInstance);
}

void BossBegomanHead::tryTurn() {
    setNerve(&NrvBossBegomanHead::HostTypeNrvTurn::sInstance);
}

void BossBegomanHead::tryTurnEnd() {
    setNerve(&NrvBossBegomanHead::HostTypeNrvTurnEnd::sInstance);
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

    _9C -= 0.23f;
}

void BossBegomanHead::exeSwitchOn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "On");
    }

    MR::setNerveAtBckStopped(this, &NrvBossBegomanHead::HostTypeNrvOnWait::sInstance);
}

void BossBegomanHead::exeSwitchOff() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Off");
    }

    MR::setNerveAtBckStopped(this, &NrvBossBegomanHead::HostTypeNrvOffWait::sInstance);
}

void BossBegomanHead::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Turn");
    }

    _9C -= 0.2f;
}

void BossBegomanHead::exeTurnEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "TurnEnd");
    }

    _9C -= 0.23f;

    MR::setNerveAtBckStopped(this, &NrvBossBegomanHead::HostTypeNrvOffWait::sInstance);
}

void BossBegomanHead::calcAndSetBaseMtx() {
    PartsModel::calcAndSetBaseMtx();
    mJointDelegator->registerCallBack();
}

bool BossBegomanHead::calcJointEdge(TPos3f* pMtx, const JointControllerInfo&) {
    f32    v3 = _9C;
    TVec3f v8;
    v8.x = 0.0;
    v8.y = 1.0;
    v8.z = 0.0;
    TPos3f v9;
    TVec3f v7;
    v7.x = 0.0;
    v7.y = 1.0;
    v7.z = 0.0;
    v9.makeRotateInline(v7, v3);
    pMtx->concat(*pMtx, v9);

    return true;
}

BossBegomanHead::~BossBegomanHead() {
}
