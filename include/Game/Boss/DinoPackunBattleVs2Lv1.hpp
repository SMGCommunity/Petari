#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunStateDamage;
class DinoPackunTrackFireHolder;
class DinoPackunFireHolder;
class DinoPackunStateAwake;

class DinoPackunBattleVs2Lv1 : public DinoPackunAction {
public:
    DinoPackunBattleVs2Lv1(DinoPackun*);

    virtual ~DinoPackunBattleVs2Lv1();
    virtual void init();
    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void setTrackFire(DinoPackunTrackFireHolder*);
    void setFireBall(DinoPackunFireHolder*);
    void setMiddleBattle();
    void setLastBattle();
    bool tryAwake();
    void exeWalk();
    void endWalk();
    void exeTurn();
    void exeFindCrazy();
    void exeCrazy();
    void endCrazy();
    void exeChase();
    void endChase();
    void exeAttackHit();
    void updateOnOffFireTrail(s32, s32, s32);
    bool emitFireMouth();
    bool emitFireTail();
    inline void exeDamage();

    DinoPackunStateDamage* mStateDamage;          // 0x14
    DinoPackunStateAwake* mStateAwake;            // 0x18
    DinoPackunTrackFireHolder* mTrackFireHolder;  // 0x1C
    DinoPackunFireHolder* mFireHolder;            // 0x20
};
