#pragma once

#include "Game/Demo/DemoCtrlBase.hpp"
#include "Game/MapObj/MapObjActor.hpp"

class RailMoveObj : public MapObjActor {
public:
    RailMoveObj(const char*);

    virtual ~RailMoveObj() {}

    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
    virtual void setupInitInfo(const JMapInfoIter&, MapObjActorInitInfo*);
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
    RailMoveObjPress(const char*);

    virtual ~RailMoveObjPress();
    virtual void init(const JMapInfoIter&);
};

class RailMoveObjBreakAtEnd : public RailMoveObj {
public:
    RailMoveObjBreakAtEnd(const char*);

    virtual ~RailMoveObjBreakAtEnd();
    virtual void doAtEndPoint();
};

class RailMoveIndirectObj : public RailMoveObj {
public:
    inline RailMoveIndirectObj(const char* pName) : RailMoveObj(pName) {}

    virtual ~RailMoveIndirectObj();

    virtual void connectToScene(const MapObjActorInitInfo&);
};

class RailMoveObjSwitchShadow : public RailMoveObj {
public:
    inline RailMoveObjSwitchShadow(const char* pName) : RailMoveObj(pName) {}

    virtual ~RailMoveObjSwitchShadow();
    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void startOnShadow();
};

class RailDemoMoveObj : public RailMoveObj {
public:
    RailDemoMoveObj(const char*);

    virtual ~RailDemoMoveObj();
    virtual void init(const JMapInfoIter&);
    virtual bool tryStartMove();
    virtual void startMoveInner();
    virtual void move();
    virtual bool endMove();

    DemoCtrlBase* mDemoControl;  // 0xD4
};

class RailRotateMoveObj : public RailMoveObj {
public:
    RailRotateMoveObj(const char*);

    virtual ~RailRotateMoveObj();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void setupInitInfo(const JMapInfoIter&, MapObjActorInitInfo*);

    bool tryStartRotateAtPoint();
    bool tryStartRotateBetweenPoints();
};
