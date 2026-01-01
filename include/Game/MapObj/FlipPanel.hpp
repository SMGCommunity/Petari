#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Util/JointController.hpp"

class FlipPanel : public MapObjActor {
public:
    FlipPanel(const char*);

    virtual ~FlipPanel();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void endClipped();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeFrontLand();
    void exeBackLand();
    void exeWait();
    void exeEndPrepare();
    void exeEnd();
    bool calcJointMove(TPos3f*, const JointControllerInfo&);
    bool checkPlayerOnTop();

    JointControlDelegator< FlipPanel >* mDelegator;  // 0xC4
    MsgSharedGroup* mFlipPanelGroup;                 // 0xC8
    bool _CC;
    u8 _CD;
    s32 _D0;
    bool mIsReverse;  // 0xD4
};

class FlipPanelObserver : public LiveActor {
public:
    FlipPanelObserver(const char*);

    virtual ~FlipPanelObserver();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeComplete();
    void exeDemoWait();

    MsgSharedGroup* _8C;
    s32 _90;
    s32 mDemoDelay;    // 0x94
    s32 mPowerStarId;  // 0x98
    u8 _9C;
};
