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
    void updateOnOffFireTail(s32, s32, s32);
    bool emitFireMouth();
    bool emitFireTail();

    void exeTurn();
    void exeWalk();
    void endWalk() NO_INLINE;
    void exeFindCrazy();
    void exeCrazy();
    void endCrazy() NO_INLINE;
    void exeChase();
    void endChase() NO_INLINE;
    void exeAwake();
    void endAwake();
    void exeAttackHit();
    void exeDamage();

    /* 0x14 */ DinoPackunStateDamage* mStateDamage;
    /* 0x18 */ DinoPackunStateAwake* mStateAwake;
    /* 0x1C */ DinoPackunTrackFireHolder* mTrackFireHolder;
    /* 0x20 */ DinoPackunFireHolder* mFireHolder;
    /* 0x24 */ DinoPackunStateFire* mStateFire;
    /* 0x28 */ TVec3f _28;
    /* 0x34 */ s32 _34;
    /* 0x38 */ u8 _38;
    /* 0x39 */ u8 _39;
    /* 0x3A */ u8 _3A;
    /* 0x3B */ u8 _3B;
    /* 0x3C */ u8 _3C;
    /* 0x3D */ u8 _3D;
};
