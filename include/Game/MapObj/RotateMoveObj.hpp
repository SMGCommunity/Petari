#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class RotateMoveObj : public MapObjActor {
public:
    inline RotateMoveObj(const char* pName) : MapObjActor(pName) {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void setStateMove();
    void exeMove();
    void exeStop();
    void exeWait();
    void exeWaitForPlayerOn();
};
