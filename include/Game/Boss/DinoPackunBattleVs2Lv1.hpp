#pragma once

#include "Game/Boss/DinoPackunAction.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class DinoPackunStateDamage;
class DinoPackunTrackFireHolder;
class DinoPackunFireHolder;
class DinoPackunStateAwake;
class DinoPackunStateFire;

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
    void endWalk() NO_INLINE;
    void exeTurn();
    void exeFindCrazy();
    void exeCrazy();
    void endCrazy() NO_INLINE;
    void exeChase();
    void endChase() NO_INLINE;
    void exeAttackHit();
    void updateOnOffFireTail(s32, s32, s32);
    bool emitFireMouth();
    bool emitFireTail();
    inline void exeDamage();
    void exeAwake();
    void endAwake();

    DinoPackunStateDamage* mStateDamage;          // 0x14
    DinoPackunStateAwake* mStateAwake;            // 0x18
    DinoPackunTrackFireHolder* mTrackFireHolder;  // 0x1C
    DinoPackunFireHolder* mFireHolder;            // 0x20
    DinoPackunStateFire* mStateFire;              // 0x24
    TVec3f _28;
    s32 _34;
    u8 _38;
    u8 _39;
    u8 _3A;
    u8 _3B;
    u8 _3C;
    u8 _3D;
};
