#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class KoopaBattleMain;
class KoopaSubSequenceBattle;

class KoopaSequencerVs1 : public KoopaSequencer {
public:
    KoopaSequencerVs1();

    virtual ~KoopaSequencerVs1();
    /* 0x0C */ virtual void init(Koopa* pKoopa, const JMapInfoIter& rIter);
    /* 0x18 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x1C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x20 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeBattleStairs();
    void exeDemoBattleStart();
    void exeDemoJumpToPlanet();
    void exeBattle();
    void exeWaitDemoBattleEnd();
    void exeDemoBattleEnd();
    void exeWaitDemo();

private:
    /* 0x24 */ KoopaSubSequenceBattle* mSubSequenceBattle;
    /* 0x28 */ KoopaBattleMain* mBattleMain;
};
