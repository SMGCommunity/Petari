#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class PartsModel;
class JointRumbler;

class HammerHeadPackun : public LiveActor {
public:
    HammerHeadPackun(const char*);

    virtual ~HammerHeadPackun();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeSearch();
    void exeAttack();
    void exeRest();
    void exeRecover();
    void exeDamage();
    void exeRise();
    void exeHit();
    void exeCrow();
    void exeSmashKill();
    void exeBlowKill();
    void exeNumb();
    void endNumb();
    void initPosture();
    void initSensor();
    void initShadow();
    void initLeaf(const JMapInfoIter&);
    bool isTargetInRange() const;
    bool isInFieldOfView(const TVec3f&) const;
    bool isAttackSensorValid() const;
    bool isChance() const;
    bool calcPlayerDir(TVec3f*) const;
    void turnTo(const TVec3f&, f32);
    void turnToAttackVec(s32);
    void verticalizeFrontVec();
    bool tryShiftNumb();
    bool receiveMsgPlayerAttackChance(u32, HitSensor*, HitSensor*);

    inline bool checkBlowAttack() const;

    /* 0x8C */ PartsModel* mLeafModel;
    /* 0x90 */ s32 mImmunityTime;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ TVec3f _A0;
    /* 0xAC */ TVec3f _AC;
    /* 0xB8 */ JointRumbler* mJointRumbler;
    /* 0xBC */ s32 mNumbTime;

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
