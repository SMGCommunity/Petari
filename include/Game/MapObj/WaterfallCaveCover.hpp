#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class WaterfallCaveCover : public MapObjActor {
public:
    WaterfallCaveCover(const char* pName) : MapObjActor(pName) {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeBreak();
};
