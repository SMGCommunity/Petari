#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ActorCameraInfo;

class AnmModelObj : public MapObjActor {
public:
    /// @brief Creates a new `AnmModelObj`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelObj(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control() {
    }
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {
    }
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
    }
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {
    }
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
    }
    virtual bool isOnStartAnmTrigger() const = 0;
    virtual bool isDone() const;
    virtual bool isKilledAtMoveDone() const {
        return false;
    }
    virtual bool isRepeat() const {
        return false;
    }
    virtual void startInner() {
    }
    virtual void moveInner() {
    }
    virtual void stopInner() {
    }

    void exeWait();
    void exeMove();
    void exeDone();

    /* 0xC4 */ TVec3f mJointPos;
};

class AnmModelSwitchMove : public AnmModelObj {
public:
    /// @brief Creates a new `AnmModelSwitchMove`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelSwitchMove(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual bool isOnStartAnmTrigger() const;
};

class AnmModelSwitchMoveEndKill : public AnmModelSwitchMove {
public:
    /// @brief Creates a new `AnmModelSwitchMoveEndKill`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelSwitchMoveEndKill(const char* pName) : AnmModelSwitchMove(pName) {
    }

    virtual bool isKilledAtMoveDone() const;
};

class AnmModelGroundOnMove : public AnmModelObj {
public:
    /// @brief Creates a new `AnmModelGroundOnMove`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelGroundOnMove(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool isOnStartAnmTrigger() const;
    virtual bool isKilledAtMoveDone() const {
        return true;
    }
};

class AnmModelBindMove : public AnmModelObj {
public:
    /// @brief Creates a new `AnmModelBindMove`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelBindMove(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual bool isOnStartAnmTrigger() const;
    virtual bool isKilledAtMoveDone() const {
        return false;
    }
    virtual bool isRepeat() const {
        return true;
    }
};

class AnmModelSwitchMoveEventCamera : public AnmModelSwitchMove {
public:
    /// @brief Creates a new `AnmModelSwitchMoveEventCamera`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelSwitchMoveEventCamera(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual bool isDone() const;
    virtual void startInner();
    virtual void stopInner();

    /* 0xD0 */ ActorCameraInfo* mCameraInfo;
};
