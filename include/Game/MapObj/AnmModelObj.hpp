#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ActorCameraInfo;

class AnmModelObj : public MapObjActor {
public:
    /// @brief Creates a new `AnmModelObj`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelObj(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control() {
    }
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {
    }
    /* 0x80 */ virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
    }
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {
    }
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {
    }
    /* 0x90 */ virtual bool isOnStartAnmTrigger() const = 0;
    /* 0x94 */ virtual bool isDone() const;
    /* 0x98 */ virtual bool isKilledAtMoveDone() const {
        return false;
    }
    /* 0x9C */ virtual bool isRepeat() const {
        return false;
    }
    /* 0xA0 */ virtual void startInner() {
    }
    /* 0xA4 */ virtual void moveInner() {
    }
    /* 0xA8 */ virtual void stopInner() {
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

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x90 */ virtual bool isOnStartAnmTrigger() const;
};

class AnmModelSwitchMoveEndKill : public AnmModelSwitchMove {
public:
    /// @brief Creates a new `AnmModelSwitchMoveEndKill`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelSwitchMoveEndKill(const char* pName) : AnmModelSwitchMove(pName) {
    }

    /* 0x98 */ virtual bool isKilledAtMoveDone() const;
};

class AnmModelGroundOnMove : public AnmModelObj {
public:
    /// @brief Creates a new `AnmModelGroundOnMove`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelGroundOnMove(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x90 */ virtual bool isOnStartAnmTrigger() const;
    /* 0x98 */ virtual bool isKilledAtMoveDone() const {
        return true;
    }
};

class AnmModelBindMove : public AnmModelObj {
public:
    /// @brief Creates a new `AnmModelBindMove`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelBindMove(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x90 */ virtual bool isOnStartAnmTrigger() const;
    /* 0x98 */ virtual bool isKilledAtMoveDone() const {
        return false;
    }
    /* 0x9C */ virtual bool isRepeat() const {
        return true;
    }
};

class AnmModelSwitchMoveEventCamera : public AnmModelSwitchMove {
public:
    /// @brief Creates a new `AnmModelSwitchMoveEventCamera`.
    /// @param pName A pointer to the null-terminated name of the object.
    AnmModelSwitchMoveEventCamera(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x94 */ virtual bool isDone() const;
    /* 0xA0 */ virtual void startInner();
    /* 0xA8 */ virtual void stopInner();

    /* 0xD0 */ ActorCameraInfo* mCameraInfo;
};
