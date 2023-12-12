#pragma once

#include "Game/MapObj/MapObjActor.h"
#include "Game/Util/JointController.h"

namespace {
    static u32 sBloomSyncStep;
};

class FlipPanel : public MapObjActor {
public:
    FlipPanel(const char *);

    virtual ~FlipPanel();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void endClipped();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeFrontLand();
    void exeBackLand();
    void exeWait();
    void exeEnd();
    bool calcJointMove(TPos3f *, const JointControllerInfo &);
    bool checkPlayerOnTop();

    JointControlDelegator<FlipPanel>* mDelegator;   // _C4
    MsgSharedGroup* mFlipPanelGroup;                // _C8
    bool _CC;
    u8 _CD;
    s32 _D0;
    bool mIsReverse;    // _D4
};

class FlipPanelObserver : public LiveActor {
public:
    FlipPanelObserver(const char *);

    virtual ~FlipPanelObserver();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeWait();
    void exeComplete();
    void exeDemoWait();

    MsgSharedGroup* _8C;
    s32 _90;
    s32 mDemoDelay;         // _94
    s32 mPowerStarId;       // _98
    u8 _9C;
};

namespace NrvFlipPanel {
    NERVE(FlipPanelNrvFront);
    NERVE(FlipPanelNrvBack);
    NERVE(FlipPanelNrvFrontLand);
    NERVE(FlipPanelNrvBackLand);
    NERVE(FlipPanelNrvEnd);
    NERVE(FlipPanelNrvEndPrepare);
};

namespace NrvFlipPanelObserver {
    NERVE(FlipPanelObserverNrvWait);
    NERVE(FlipPanelObserverNrvDemoWait);
    NERVE(FlipPanelObserverNrvComplete);
};