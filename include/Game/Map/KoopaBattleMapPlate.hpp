#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class KoopaBattleMapPlate : public LiveActor {
public:
    KoopaBattleMapPlate(const char* pName);

    /* 0x08 */ virtual ~KoopaBattleMapPlate();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

private:
    /* 0x8C */ TPos3f _8C;
};
