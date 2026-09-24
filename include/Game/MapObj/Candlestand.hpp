#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SpinPullParticleCallBack;

class Candlestand : public MapObjActor {
public:
    Candlestand(const char* pName);

    virtual ~Candlestand();
    virtual void init(const JMapInfoIter& rIter);
    virtual void makeActorAppeared();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void emitEffectFire();
    void deleteEffectFire();
    void emitEffectExtinguishFire();
    void appearItem();
    void exeWaitFire();
    void exeFire();
    void exeExtinguish();
    void exeFlicker();
    inline void exeAttack();
    inline void exeBurn();

    /* 0xC4 */ s32 mItem;
    /* 0xC8 */ bool mHasItemAppear;
    /* 0xCC */ SpinPullParticleCallBack* mSpinPtclCb;
};
