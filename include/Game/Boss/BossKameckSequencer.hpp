#pragma once

#include "Game/System/NerveExecutor.hpp"

class BossKameck;
class BossKameckAction;
class BossKameckBattleDemo;
class HitSensor;
class JMapInfoIter;

class BossKameckSequencer : public NerveExecutor {
public:
    BossKameckSequencer(const char*);

    virtual ~BossKameckSequencer() {
    }

    virtual void start() {
    }

    virtual void init(BossKameck*, const JMapInfoIter&);
    virtual void update();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    /* 0x08 */ BossKameck* mBossKameck;
    /* 0x0C */ BossKameckAction* _C;
    /* 0x10 */ BossKameckBattleDemo* mBattleDemo;
};