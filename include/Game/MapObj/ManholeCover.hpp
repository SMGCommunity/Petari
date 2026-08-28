#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ManholeCover : public MapObjActor {
public:
    ManholeCover(const char* pName) : MapObjActor(pName) {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeOpen();
    void exeRattle();
};
