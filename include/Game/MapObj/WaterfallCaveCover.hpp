#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class WaterfallCaveCover : public MapObjActor {
public:
    WaterfallCaveCover(const char* pName) : MapObjActor(pName) {
    }

    virtual void init(const JMapInfoIter& rIter);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeBreak();
};
