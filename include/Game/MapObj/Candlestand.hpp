#pragma once

#include "Game/Effect/SpinPullParticleCallBack.hpp"
#include "Game/MapObj/MapObjActor.hpp"

class Candlestand : public MapObjActor {
public:
    Candlestand(const char*);

    /* 0x08 */ virtual ~Candlestand();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    s32 mItem;                              // 0xC4
    bool mHasItemAppear;                    // 0xC8
    SpinPullParticleCallBack* mSpinPtclCb;  // 0xCC
};
