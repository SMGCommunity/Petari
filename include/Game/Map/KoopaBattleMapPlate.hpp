#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaBattleMapPlate : public LiveActor {
public:
    KoopaBattleMapPlate(const char* pName);

    virtual ~KoopaBattleMapPlate();
    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAndSetBaseMtx();
    bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

private:
    /* 0x8C */ TPos3f _8C;
};
