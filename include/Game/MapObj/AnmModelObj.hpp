#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AnmModelObj : public MapObjActor {
public:
    AnmModelObj(const char*);

    virtual ~AnmModelObj();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
    virtual bool isOnStartAnmTrigger() const = 0;
    virtual bool isDone() const;
    virtual bool isKilledAtMoveDone() const;
    virtual bool isRepeat() const;
    virtual void startInner();
    virtual inline void moveInner();
    virtual void stopInner();

    inline void exeWait();
    void exeMove();
    void exeDone();

    TVec3f mJointPos;  // 0xC4
};

class AnmModelSwitchMove : public AnmModelObj {
public:
    AnmModelSwitchMove(const char*);

    virtual ~AnmModelSwitchMove();
    virtual void init(const JMapInfoIter&);
    virtual bool isOnStartAnmTrigger() const;
};

class AnmModelSwitchMoveEndKill : public AnmModelSwitchMove {
public:
    AnmModelSwitchMoveEndKill(const char* pName) : AnmModelSwitchMove(pName){};

    virtual ~AnmModelSwitchMoveEndKill();
    virtual bool isKilledAtMoveDone() const;
};

class AnmModelGroundOnMove : public AnmModelObj {
public:
    AnmModelGroundOnMove(const char*);

    virtual ~AnmModelGroundOnMove();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool isOnStartAnmTrigger() const;
    virtual bool isKilledAtMoveDone() const;
};

class AnmModelBindMove : public AnmModelObj {
public:
    AnmModelBindMove(const char*);

    virtual ~AnmModelBindMove();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual bool isOnStartAnmTrigger() const;
    virtual bool isKilledAtMoveDone() const;
    virtual bool isRepeat() const;
};

class AnmModelSwitchMoveEventCamera : public AnmModelSwitchMove {
public:
    AnmModelSwitchMoveEventCamera(const char*);

    virtual ~AnmModelSwitchMoveEventCamera();
    virtual void init(const JMapInfoIter&);
    virtual bool isDone() const;
    virtual void startInner();
    virtual void stopInner();

    ActorCameraInfo* mCameraInfo;  // 0xD0
};
