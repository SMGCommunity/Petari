#pragma once

#include "Game/Boss/BossKameckAction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class BossKameck;
class BossKameckBattleDemo;

class BossKameckSequencer : public NerveExecutor {
public:
    BossKameckSequencer(const char*);

    inline virtual ~BossKameckSequencer() {}

    virtual void start() {}

    virtual void init(BossKameck*, const JMapInfoIter&);
    virtual void update();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    BossKameck* mBossKameck;            // 0x8
    BossKameckAction* _C;               // not really sure what this is.
    BossKameckBattleDemo* mBattleDemo;  // 0x10
};