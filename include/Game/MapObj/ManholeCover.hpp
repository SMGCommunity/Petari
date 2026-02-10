#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ManholeCover : public MapObjActor {
public:
    inline ManholeCover(const char* pName) : MapObjActor(pName) {};

    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeOpen();
    void exeRattle();
};
