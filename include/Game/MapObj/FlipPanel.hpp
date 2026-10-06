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

    /* 0xC4 */ JointControlDelegator< FlipPanel >* mDelegator;
    /* 0xC8 */ MsgSharedGroup* mFlipPanelGroup;
    /* 0xCC */ bool _CC;
    /* 0xCD */ u8 _CD;
    /* 0xD0 */ s32 _D0;
    /* 0xD4 */ bool mIsReverse;
};

class FlipPanelObserver : public LiveActor {
public:
    FlipPanelObserver(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeComplete();
    void exeDemoWait();

    /* 0x8C */ MsgSharedGroup* _8C;
    /* 0x90 */ s32 _90;
    /* 0x94 */ s32 mDemoDelay;
    /* 0x98 */ s32 mPowerStarId;
    /* 0x9C */ u8 _9C;
};
