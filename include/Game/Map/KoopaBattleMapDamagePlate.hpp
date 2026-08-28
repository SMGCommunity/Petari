#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry.hpp>

class KoopaBattleMapDamagePlate : public LiveActor {
public:
    KoopaBattleMapDamagePlate(const char*);

    /* 0x08 */ virtual ~KoopaBattleMapDamagePlate();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeBreak();
    void exeRecover();

    /* 0x8C */ TPos3f mBaseMtx;
};
