#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AstroCore : public MapObjActor {
public:
    AstroCore(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeGrow();

    void startDemo();
    void startAnimGrow();
    void setStateBeforeGrow();
};
