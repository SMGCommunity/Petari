#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class HitSensor;

class KoopaSequencerVs1 : public KoopaSequencer {
public:
    KoopaSequencerVs1();
    
    virtual ~KoopaSequencerVs1();
    virtual void init(Koopa*, const JMapInfoIter&);

    void exeBattleStairs();
    void exeDemoBattleStart();
    void exeDemoJumpToPlanet();
    void exeBattle();
    void exeWaitDemoBattleEnd();
    void exeDemoBattleEnd();
    void exeWaitDemo();

    void attackSensor(HitSensor*, HitSensor*);
    void receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    void receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    void receiveOtherMsg(u32, HitSensor*, HitSensor*);

private:
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
};
