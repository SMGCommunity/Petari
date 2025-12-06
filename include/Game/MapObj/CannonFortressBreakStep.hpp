#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class CannonFortressBreakStep : public MapObjActor {
public:
    CannonFortressBreakStep(const char*);

    virtual ~CannonFortressBreakStep();
    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeFall();
    void exeFallStart();
    void exeBreak();
    void startFall();
};
