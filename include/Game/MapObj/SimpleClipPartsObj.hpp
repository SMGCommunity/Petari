#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SimpleClipPartsObj : public MapObjActor {
public:
    SimpleClipPartsObj(const char*);

    virtual ~SimpleClipPartsObj();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void connectToScene(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
};
