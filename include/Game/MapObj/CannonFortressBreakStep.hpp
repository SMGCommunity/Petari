#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class CannonFortressBreakStep : public MapObjActor {
public:
    CannonFortressBreakStep(const char*);

    /* 0x08 */ virtual ~CannonFortressBreakStep();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeFall();
    void exeFallStart();
    void exeBreak();
    void startFall();
};
