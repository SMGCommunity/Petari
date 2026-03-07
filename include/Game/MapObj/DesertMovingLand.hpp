#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class DesertMovingLand : public MapObjActor {
public:
    DesertMovingLand(const char* pName);
    virtual ~DesertMovingLand();
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void connectToScene(const MapObjActorInitInfo& rIter);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
    void startDemo();
    void updateDemoPlayerPos();
    void exeWait();
    void exeMoveDown();
    void exeMoveUp();
    void exeMoveSign();
    void exeStop();

    TVec3f _C4;
    TVec3f _D0;
    s32 _DC;
    s32 _E0;
    TMtx34f _E4;
};
