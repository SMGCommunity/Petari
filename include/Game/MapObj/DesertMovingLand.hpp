#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class DesertMovingLand : public MapObjActor {
public:
    DesertMovingLand(const char* pName);
    /* 0x08 */ virtual ~DesertMovingLand();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x48 */ virtual void control();
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo& rIter);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
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
