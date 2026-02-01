#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class BossStinkBug;
class HitSensor;
class SpringValue;

class BossStinkBugActionBase : public ActorStateBase< BossStinkBug > {
public:
    BossStinkBugActionBase(const char*, BossStinkBug*);

    ~BossStinkBugActionBase();


    virtual bool isValidFollowId(s32 id) const{
        return false;
    };
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver) {};
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    };
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    };
    virtual bool isAngry() const {
        return false;
    };

    bool updateGroundWalk(f32);
    bool updateFlyRoll();
    bool updateFlyRollWait(s32);
    bool updateFlyRollEnd();
    bool updateFlyDash(f32);
    void throwBomb(s32);
    void resetThrowBombTiming();
    f32 calcShakeOffSignSpeed(s32) const;
    f32 calcShakeOffSpeed() const;
    f32 calcShakeOffEndSpeed() const;
    s32 calcKeepRollTime(s32) const;
    s32 calcFlySoundVol() const;
    s32 calcIIRCutOff() const;
    void updateSound();
    void updateSoundFly();
    bool updateGroundRegainRail(f32);
    bool updateGroundFind();
    bool updateGroundShakeOff();
    bool updateGroundRun(f32);
    bool updateGroundAttackSuccess();
    bool updateGroundCoolDown();
    bool updateGroundTurn();
    bool updateGroundDamage();
    bool updateFly();
    bool updateFlyShakeOffSign(s32);
    bool updateFlyShakeOff();
    bool updateFlyShakeOffWait(s32);
    bool updateFlyShakeOffEnd();
    bool updateFlyRollSign(s32);
    bool updateFlyTurnSign();
    bool updateFlyTurn();
    bool updateFall();
    bool updateFallDamage();
    bool updateToGround();
    bool updateLand();
    bool updateToFly();
    void startSoundFly(s32);

    /* 0x10 */ SpringValue* mSpringValue;
    TVec3f _14;
    TVec3f _20;
    f32 _2C;
    f32 _30;
    f32 _34;
    f32 _38;
    f32 _3C;
    f32 _40;
    f32 _44;
    f32 _48;
    /* 0x4C */ u32 mThrowBombTimer;
    bool _50;
    f32 _54;
    f32 _58;
    s32 _5C;
    bool _60;
    bool _61;
};
