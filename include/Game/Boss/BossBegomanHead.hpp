#pragma once

#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/JointController.hpp"

class BossBegomanHead : public PartsModel {
public:
    BossBegomanHead(LiveActor *, MtxPtr);

    virtual ~BossBegomanHead();
    virtual void init(const JMapInfoIter &);
    virtual void calcAndSetBaseMtx();

    bool calcJointEdge(TPos3f *, const JointControllerInfo &);

    bool isSwitchOn();
    bool isEdgeOut();
    void setOpeningDemo();
    void trySwitchPushTrample();
    void tryForceRecover();
    void tryTurn();
    void tryTurnEnd();
    void exeOffWait();
    void exeTurn();
    void exeTurnEnd();
    inline void exeSwitchOff();
    inline void exeSwitchOn();
    inline void exeOnWait();
    inline void exeOpeningDemo();
    inline void exeDemoWait();

    f32 _9C;
    JointControlDelegator<BossBegomanHead>* mJointDeleg;         // 0xA0
};

namespace NrvBossBegomanHead {
    NERVE_DECL(HostTypeNrvDemoWait, BossBegomanHead, BossBegomanHead::exeDemoWait);
    NERVE_DECL(HostTypeNrvOpeningDemo, BossBegomanHead, BossBegomanHead::exeOpeningDemo);
    NERVE_DECL(HostTypeNrvOnWait, BossBegomanHead, BossBegomanHead::exeOnWait);
    NERVE_DECL(HostTypeNrvOffWait, BossBegomanHead, BossBegomanHead::exeOffWait);
    NERVE_DECL(HostTypeNrvSwitchOn, BossBegomanHead, BossBegomanHead::exeSwitchOn);
    NERVE_DECL(HostTypeNrvSwitchOff, BossBegomanHead, BossBegomanHead::exeSwitchOff);
    NERVE_DECL(HostTypeNrvTurn, BossBegomanHead, BossBegomanHead::exeTurn);
    NERVE_DECL(HostTypeNrvTurnEnd, BossBegomanHead, BossBegomanHead::exeTurnEnd);
};