#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class KoopaBattleMain;
class KoopaSubSequenceBattle;

class KoopaSequencerVs2 : public KoopaSequencer {
public:
    KoopaSequencerVs2();

    virtual ~KoopaSequencerVs2();
    virtual void init(Koopa* pKoopa, const JMapInfoIter& rIter);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWaitDemoBattleStart();
    void exeDemoBattleStart();
    void exeDemoJumpToPlanet();
    void exeBattle();
    void exeWaitDemoDown();
    void exeDemoDown();
    void exeWaitDemo();

    /* 0x24 */ KoopaSubSequenceBattle* mSubSequenceBattle;
    /* 0x28 */ KoopaBattleMain* mBattleMain;
};
