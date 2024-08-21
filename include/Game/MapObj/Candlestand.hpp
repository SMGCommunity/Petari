#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/Effect/SpinPullParticleCallBack.hpp"

class Candlestand : public MapObjActor {
public:
    Candlestand(const char *);

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

    s32 mItem;                                      // 0xC4
    bool mHasItemAppear;                            // 0xC8
    SpinPullParticleCallBack* mSpinPtclCb;          // 0xCC
};

namespace NrvCandlestand {
    NERVE_DECL_NULL(HostTypeWaitFire);
    NERVE_DECL(HostTypeFire, Candlestand, Candlestand::exeFire);
    NERVE_DECL(HostTypeBurn, Candlestand, Candlestand::exeBurn);
    NERVE_DECL(HostTypeAttack, Candlestand, Candlestand::exeAttack);
    NERVE_DECL(HostTypeExtinguish, Candlestand, Candlestand::exeExtinguish);
    NERVE_DECL(HostTypeFlicker, Candlestand, Candlestand::exeFlicker);
};