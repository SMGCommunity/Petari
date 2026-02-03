#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ManholeCover : public MapObjActor {
public:
    ManholeCover(const char*);

    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeOpen();
    void exeRattle();
};
