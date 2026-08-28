#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

template < typename T >
class JointControlDelegator;
class JointControllerInfo;
class MsgSharedGroup;

class FlipPanel : public MapObjActor {
public:
    FlipPanel(const char*);

    /* 0x08 */ virtual ~FlipPanel();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x44 */ virtual void endClipped();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    /* 0x08 */ virtual ~FlipPanelObserver();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeComplete();
    void exeDemoWait();

    MsgSharedGroup* _8C;
    s32 _90;
    s32 mDemoDelay;    // 0x94
    s32 mPowerStarId;  // 0x98
    u8 _9C;
};
