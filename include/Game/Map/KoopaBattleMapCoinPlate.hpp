#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class KoopaBattleMapCoinPlate : public LiveActor {
public:
    KoopaBattleMapCoinPlate(const char*);

    virtual ~KoopaBattleMapCoinPlate();
    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

private:
    /* 0x8C */ TPos3f mBaseMtx;
};
