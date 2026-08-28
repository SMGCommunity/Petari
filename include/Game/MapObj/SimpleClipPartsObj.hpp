#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SimpleClipPartsObj : public MapObjActor {
public:
    SimpleClipPartsObj(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    /* 0x80 */ virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    /* 0x88 */ virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
};
