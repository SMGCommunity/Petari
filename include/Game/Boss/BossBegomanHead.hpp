#pragma once

#include "Game/LiveActor/PartsModel.hpp"

template <typename T>
class JointControlDelegator;
class JointControllerInfo;

class BossBegomanHead : public PartsModel {
public:
    BossBegomanHead(LiveActor*, MtxPtr);

    virtual ~BossBegomanHead();
    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();

    bool isSwitchOn();
    bool isEdgeOut();
    void setOpeningDemo();
    void trySwitchPushTrample();
    void tryForceRecover();
    void tryTurn();
    void tryTurnEnd();
    void exeDemoWait();
    void exeOpeningDemo();
    void exeOnWait();
    void exeOffWait();
    void exeSwitchOn();
    void exeSwitchOff();
    void exeTurn();
    void exeTurnEnd();
    bool calcJointEdge(TPos3f*, const JointControllerInfo&);

private:
    /* 0x9C */ f32                                     _9C;
    /* 0xA0 */ JointControlDelegator<BossBegomanHead>* mJointDelegator;
};
