#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class HeavensDoorDemoObj : public MapObjActor {
public:
    HeavensDoorDemoObj(const char*);

    /* 0x08 */ virtual ~HeavensDoorDemoObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);

    void exeAppear();
    void exeWait();
    void exeVanish();
    void startInsideCageDemo();

    bool _C4;
};
