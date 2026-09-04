#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class IceStep : public LiveActor {
public:
    IceStep(const char*);

    virtual ~IceStep();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void setOn(u32, const TVec3f&, const TVec3f&);
    void destroy();
    void doHit();

    void exeWait();
    void exeHit();

    /* 0x8C */ Mtx _8C;
    /* 0xBC */ u16 mTimer;
};
