#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class HitSensor;

class KoopaSequencerVs1 : public KoopaSequencer {
public:
    KoopaSequencerVs1();

    virtual ~KoopaSequencerVs1();
    virtual void init(Koopa*, const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual void receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual void receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual void receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeBattleStairs();
    void exeDemoBattleStart();
    void exeDemoJumpToPlanet();
    void exeBattle();
    void exeWaitDemoBattleEnd();
    void exeDemoBattleEnd();
    void exeWaitDemo();

private:
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
};
