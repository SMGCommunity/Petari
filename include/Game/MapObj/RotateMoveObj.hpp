#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class RotateMoveObj : public MapObjActor {
public:
    inline RotateMoveObj(const char* pName) : MapObjActor(pName) {}

    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void setStateMove();
    void exeMove();
    void exeStop();
    void exeWait();
    void exeWaitForPlayerOn();
};
