#pragma once

#include "Game/System/NerveExecutor.hpp"

class HitSensor;
class JMapInfoIter;
class Koopa;
class KoopaBattleStairsBase;
class KoopaDemoJumpToPlanet;
class KoopaDemoPowerUp;

class KoopaSequencer : public NerveExecutor {
public:
    KoopaSequencer(const char* pName, s32 vs);

    /* 0x08 */ virtual ~KoopaSequencer() {};
    /* 0x0C */ virtual void init(Koopa* pKoopa, const JMapInfoIter& rIter);
    /* 0x10 */ virtual void update();
    /* 0x14 */ virtual void calcAndSetBaseMtx();
    /* 0x18 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x1C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x20 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void startBattleLevel2();
    void startBattleLevel3();

    /* 0x08 */ Koopa* mKoopa;
    /* 0x0C */ KoopaDemoPowerUp* mPowerUp;
    /* 0x10 */ KoopaBattleStairsBase* mBattleStairs;
    /* 0x14 */ KoopaDemoJumpToPlanet* mJumpToPlanet;
    /* 0x18 */ u32 mLv;
    /* 0x1C */ u32 mVs;
    /* 0x20 */ bool mIsAngry;
};
