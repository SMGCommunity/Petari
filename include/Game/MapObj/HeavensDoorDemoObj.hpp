#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class HeavensDoorDemoObj : public MapObjActor {
public:
    HeavensDoorDemoObj(const char*);

    virtual ~HeavensDoorDemoObj();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void connectToScene(const MapObjActorInitInfo&);

    void exeAppear();
    void exeWait();
    void exeVanish();
    void startInsideCageDemo();

    bool _C4;
};
