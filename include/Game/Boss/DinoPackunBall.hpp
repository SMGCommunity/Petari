#pragma once

#include "Game/Boss/DinoPackunTailNode.hpp"

class DinoPackunBall : public DinoPackunTailNode {
public:
    DinoPackunBall(const char*, DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunBall();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x7C */ virtual void requestLockPosition();
    /* 0x80 */ virtual void requestUnLockPosition();
    /* 0x84 */ virtual void addNodeVelocity(const TVec3f&);
    /* 0x88 */ virtual JointController* createJointControllerOwn(LiveActor*, const char*);

    void setWeakSensor(HitSensor*);
    void addDodgeTargetVelocity();
    bool requestPunch(HitSensor*, HitSensor*);
    void setDamageFire();
    void setDamageNone();
    bool tryRebound();
    void exeWait();
    void exeShoot();
    void exeCharge();
    void exeReverse();
    void exeRebound();
    void exeLock();

    TPos3f _D4;
    TQuat4f _104;
    HitSensor* mWeakSensor;  // 0x114
    TVec3f _118;
    u8 _124;
    u8 _125;
    u8 _126;
    s32 _128;
};
