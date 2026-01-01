#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AstroDome : public MapObjActor {
public:
    AstroDome(const char*);

    virtual ~AstroDome();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeDisappear();
    void exeAppear();
};
