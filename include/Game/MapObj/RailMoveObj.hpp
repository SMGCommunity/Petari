#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class DemoCtrlBase;

class RailMoveObj : public MapObjActor {
public:
    RailMoveObj(const char*);

    /* 0x08 */ virtual ~RailMoveObj() {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
    /* 0x90 */ virtual void setupInitInfo(const JMapInfoIter&, MapObjActorInitInfo*);
    /* 0x94 */ virtual bool tryStartMove();
    /* 0x98 */ virtual void startMoveInner();
    /* 0x9C */ virtual void move();
    /* 0xA0 */ virtual void doAtEndPoint();
    /* 0xA4 */ virtual bool endMove();
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

    /* 0x08 */ virtual ~RailMoveObjPress();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};

class RailMoveObjBreakAtEnd : public RailMoveObj {
public:
    RailMoveObjBreakAtEnd(const char*);

    /* 0x08 */ virtual ~RailMoveObjBreakAtEnd();
    /* 0xA0 */ virtual void doAtEndPoint();
};

class RailMoveIndirectObj : public RailMoveObj {
public:
    inline RailMoveIndirectObj(const char* pName) : RailMoveObj(pName) {
    }

    /* 0x08 */ virtual ~RailMoveIndirectObj();

    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
};

class RailMoveObjSwitchShadow : public RailMoveObj {
public:
    inline RailMoveObjSwitchShadow(const char* pName) : RailMoveObj(pName) {
    }

    /* 0x08 */ virtual ~RailMoveObjSwitchShadow();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void startOnShadow();
};

class RailDemoMoveObj : public RailMoveObj {
public:
    RailDemoMoveObj(const char*);

    /* 0x08 */ virtual ~RailDemoMoveObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x94 */ virtual bool tryStartMove();
    /* 0x98 */ virtual void startMoveInner();
    /* 0x9C */ virtual void move();
    /* 0xA4 */ virtual bool endMove();

    DemoCtrlBase* mDemoControl;  // 0xD4
};

class RailRotateMoveObj : public RailMoveObj {
public:
    RailRotateMoveObj(const char*);

    /* 0x08 */ virtual ~RailRotateMoveObj();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x90 */ virtual void setupInitInfo(const JMapInfoIter&, MapObjActorInitInfo*);

    bool tryStartRotateAtPoint();
    bool tryStartRotateBetweenPoints();
};
