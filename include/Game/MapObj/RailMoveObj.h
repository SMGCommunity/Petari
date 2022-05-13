#pragma once

#include "Game/MapObj/MapObjActor.h"
#include "Game/Demo/DemoCtrlBase.h"

namespace {
    const char* cMoveBckName = "Move";
};

class RailMoveObj : public MapObjActor {
public:
    RailMoveObj(const char *);

    virtual ~RailMoveObj() {

    }

    virtual void init(const JMapInfoIter &);
    virtual void startClipped();
    virtual void endClipped();
    virtual u32 receiveOtherMsg(u32, HitSensor *, HitSensor *);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo &);
    virtual void setupInitInfo(const JMapInfoIter &, MapObjActorInitInfo *);
    virtual bool tryStartMove();
    virtual void startMoveInner();
    virtual void move();
    virtual void doAtEndPoint();
    virtual bool endMove();
    virtual TVec3f* getSoundCalcPos();

    bool isMoving() const;
    void exeWait();
    void exeMove();

    TVec3f _C4;
    u8 _D0;
};

class RailMoveObjPress : public RailMoveObj {
public:
    RailMoveObjPress(const char *);

    virtual ~RailMoveObjPress();
    virtual void init(const JMapInfoIter &);
};

class RailMoveObjBreakAtEnd : public RailMoveObj {
public:
    RailMoveObjBreakAtEnd(const char *);

    virtual ~RailMoveObjBreakAtEnd();
    virtual void doAtEndPoint();
};

class RailMoveIndirectObj : public RailMoveObj {
public:
    virtual ~RailMoveIndirectObj();

    virtual void connectToScene(const MapObjActorInitInfo &);
};

class RailMoveObjSwitchShadow : public RailMoveObj {
public:
    inline RailMoveObjSwitchShadow(const char *pName) : RailMoveObj(pName) {

    }

    virtual ~RailMoveObjSwitchShadow();
    virtual void init(const JMapInfoIter &);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo &);

    void startOnShadow();
};

class RailDemoMoveObj : public RailMoveObj {
public:
    RailDemoMoveObj(const char *);

    virtual ~RailDemoMoveObj();
    virtual void init(const JMapInfoIter &);
    virtual bool tryStartMove();
    virtual void startMoveInner();
    virtual void move();
    virtual bool endMove();

    DemoCtrlBase* mDemoControl; // _D4
};

class RailRotateMoveObj : public RailMoveObj {
public:
    RailRotateMoveObj(const char *);

    virtual ~RailRotateMoveObj();
    virtual u32 receiveOtherMsg(u32, HitSensor *, HitSensor *);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void setupInitInfo(const JMapInfoIter &, MapObjActorInitInfo *);

    bool tryStartRotateAtPoint();
    bool tryStartRotateBetweenPoints();
};

namespace NrvRailMoveObj {
    NERVE(HostTypeWait);
    NERVE(HostTypeWaitForPlayerOn);
    NERVE(HostTypeMove);
    NERVE(HostTypeDone);
};