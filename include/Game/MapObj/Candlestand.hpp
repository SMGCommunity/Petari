#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Effect/SpinPullParticleCallBack.hpp"

class Candlestand : public MapObjActor {
public:

    virtual ~Candlestand();
    virtual void init(const JMapInfoIter &);
    virtual void makeActorAppeared();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void emitEffectFire();
    void deleteEffectFire();
    void emitEffectExtinguishFire();
    void appearItem();
    void exeFire();
    void exeExtinguish();
    void exeFlicker();
    inline void exeAttack();
    inline void exeBurn();

    s32 mItem;                                      // _C4
    bool mHasItemAppear;                            // _C8
    SpinPullParticleCallBack* mSpinPtclCb;          // _CC
};