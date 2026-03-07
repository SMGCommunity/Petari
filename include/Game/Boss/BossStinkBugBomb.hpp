#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BossStinkBugBomb : public LiveActor {
public:
    BossStinkBugBomb(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void start(const TVec3f&, const TVec3f&);
    void exeWait();
    void exeExplosion();

    TQuat4f _8C;
    TVec3f _9C;

};
