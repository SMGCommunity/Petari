#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class IceStep : public LiveActor {
public:
    IceStep(const char*);

    /* 0x08 */ virtual ~IceStep();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeHit();

    void setOn(u32, const TVec3f&, const TVec3f&);
    void destroy();
    void doHit();

    Mtx _8C;
    u16 mTimer;  // 0xBC
};
