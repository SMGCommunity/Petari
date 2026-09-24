#pragma once

#include "Game/Boss/DinoPackunTailNode.hpp"

class DinoPackunBall : public DinoPackunTailNode {
public:
    DinoPackunBall(const char*, DinoPackun*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual void requestLockPosition();
    virtual void requestUnLockPosition();
    virtual void addNodeVelocity(const TVec3f&);
    virtual JointController* createJointControllerOwn(LiveActor*, const char*);

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

    /* 0x0D4 */ TPos3f _D4;
    /* 0x104 */ TQuat4f _104;
    /* 0x114 */ HitSensor* mWeakSensor;
    /* 0x118 */ TVec3f _118;
    /* 0x124 */ u8 _124;
    /* 0x125 */ u8 _125;
    /* 0x126 */ u8 _126;
    /* 0x128 */ s32 _128;
};
