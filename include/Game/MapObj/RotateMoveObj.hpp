#pragma once

#include "Game/MapObj/MapObjActor.h"

class RotateMoveObj : public MapObjActor {
public:
    inline RotateMoveObj(const char *pName) : MapObjActor(pName) {

    }

    virtual ~RotateMoveObj();
    virtual void init(const JMapInfoIter &);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo &);

    void setStateMove();
    void exeMove();
    void exeStop();
    inline void exeWaitForPlayerOn();
};

namespace NrvRotateMoveObj {
    NERVE_DECL(HostTypeWaitForPlayerOn, RotateMoveObj, RotateMoveObj::exeWaitForPlayerOn);
    NERVE_DECL_NULL(HostTypeWait);
    NERVE_DECL(HostTypeMove, RotateMoveObj, RotateMoveObj::exeMove);
    NERVE_DECL(HostTypeStop, RotateMoveObj, RotateMoveObj::exeStop);
};