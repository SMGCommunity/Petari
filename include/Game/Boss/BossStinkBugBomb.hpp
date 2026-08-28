#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BossStinkBugBomb : public LiveActor {
public:
    BossStinkBugBomb(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void start(const TVec3f&, const TVec3f&);
    void exeWait();
    void exeExplosion();

    TQuat4f _8C;
    TVec3f _9C;
};
