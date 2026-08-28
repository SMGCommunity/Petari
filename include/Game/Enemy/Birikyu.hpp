#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Birikyu : public LiveActor {
public:
    Birikyu(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x24 */ virtual void appear();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual f32 getHitRadius() const;
    /* 0x78 */ virtual char* getCenterJointName() const;

    void initFromJmpArgs(const JMapInfoIter&);
    void initRail(const JMapInfoIter&);
    void initCollision();
    void initShadow();
    bool tryStopPointing();
    void goMove();
    void exeMove();
    void exeMoveCircle();
    void exeWaitAtEdge();
    void exeAttack();
    void exeAttackWait();
    void exeStopPointing();

    const char* _8C;
    TVec3f _90;
    TVec3f _9C;
    bool _A8;
    bool _A9;
    TVec3f _AC;
    TVec3f _B8;
    f32 _C4;
    f32 _C8;
};

class BirikyuWithFace : public Birikyu {
public:
    BirikyuWithFace(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    virtual f32 getHitRadius() const;
    /* 0x78 */ virtual char* getCenterJointName() const;

    bool _CC;
};
