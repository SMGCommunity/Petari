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

    /* 0x08 */ virtual ~BossKameckSequencer() {
    }

    /* 0x0C */ virtual void start() {
    }

    /* 0x10 */ virtual void init(BossKameck*, const JMapInfoIter&);
    /* 0x14 */ virtual void update();
    /* 0x18 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x1C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x20 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x24 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x28 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    /* 0x08 */ BossKameck* mBossKameck;
    /* 0x0C */ BossKameckAction* _C;
    /* 0x10 */ BossKameckBattleDemo* mBattleDemo;
};