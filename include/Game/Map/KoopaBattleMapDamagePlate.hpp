#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry.hpp>

class KoopaBattleMapDamagePlate : public LiveActor {
public:
    KoopaBattleMapDamagePlate(const char*);

    virtual ~KoopaBattleMapDamagePlate();
    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeBreak();
    void exeRecover();

    TPos3f _8C;
};
