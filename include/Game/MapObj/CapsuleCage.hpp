#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ActorCameraInfo;

class CapsuleCage : public MapObjActor {
public:
    CapsuleCage(const char*);

    /* 0x08 */ virtual ~CapsuleCage();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    /* 0x80 */ virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeStartCamera();
    void exeEndCamera();
    void exeOpen();
    void startOpen();

    ActorCameraInfo* mInfo;  // 0xC4
};
