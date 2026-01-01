#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class CapsuleCage : public MapObjActor {
public:
    CapsuleCage(const char*);

    virtual ~CapsuleCage();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void connectToScene(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeStartCamera();
    void exeEndCamera();
    void exeOpen();
    void startOpen();

    ActorCameraInfo* mInfo;  // 0xC4
};
