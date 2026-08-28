#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AstroDome : public MapObjActor {
public:
    AstroDome(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x48 */ virtual void control();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeDisappear();
    void exeAppear();
};
