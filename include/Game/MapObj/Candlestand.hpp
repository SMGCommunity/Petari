#pragma once

#include "Game/Effect/SpinPullParticleCallBack.hpp"
#include "Game/MapObj/MapObjActor.hpp"

class Candlestand : public MapObjActor {
public:
    Candlestand(const char*);

    virtual ~Candlestand();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
