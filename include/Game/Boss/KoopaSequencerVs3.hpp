#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class KoopaBattleMain;
class KoopaBattleVs3Lv1;
class KoopaBattleVs3Lv2;
class KoopaDemoFallToPlanetVs3Lv2;
class KoopaDemoFallToPlanetVs3Lv3;

class KoopaSequencerVs3 : public KoopaSequencer {
public:
    KoopaSequencerVs3();

    virtual ~KoopaSequencerVs3();
    virtual void init(Koopa* pKoopa, const JMapInfoIter& rIter);
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWaitPlayer();
    void exeBattleStairs();
    void exeDemoBattleStart();
    void exeDemoJumpToPlanet();
    void exeBattleVs3Lv1();
    void exeFallToPlanetLv2();
    void exeBattleVs3Lv2();
    void exeFallToPlanetLv3();
    void exeBattleVs3Lv3();
    void exeDemoPowerUpFinal();
    void exeBattleFinal();
    void exeWaitDemoDown();
    void exeDemoDown();
    void exeDead();
    void exeWaitDemo();

    /* 0x24 */ KoopaBattleVs3Lv1* mBattleLv1;
    /* 0x28 */ KoopaBattleVs3Lv2* mBattleLv2;
    /* 0x2C */ KoopaBattleMain* mBattleMain;
    /* 0x30 */ KoopaDemoFallToPlanetVs3Lv2* mDemoFallToPlanetLv2;
    /* 0x34 */ KoopaDemoFallToPlanetVs3Lv3* mDemoFallToPlanetLv3;
};
