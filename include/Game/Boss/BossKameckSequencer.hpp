#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class BossKameck;
class BossKameckBattleDemo;

class BossKameckSequencer : public NerveExecutor {
public:
    BossKameckSequencer(const char *);

    virtual ~BossKameckSequencer();

    virtual void start() {

    }

    virtual void init(BossKameck *, const JMapInfoIter &);
    virtual void update();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgPush(HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    u32 _8;
    u32 _C;
    BossKameckBattleDemo* mBattleDemo;
};