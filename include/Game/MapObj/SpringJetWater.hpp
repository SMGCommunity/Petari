#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpringJetWater : public LiveActor {
public:
    SpringJetWater(const char*);

    virtual ~SpringJetWater();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void exeJet();
    void exeEnd();

    const char* mObjName;  // 0x8C
    TVec3f _90;
};
