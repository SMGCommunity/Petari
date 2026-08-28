#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class StarPieceSpot : public LiveActor {
public:
    StarPieceSpot(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void emit();
};
