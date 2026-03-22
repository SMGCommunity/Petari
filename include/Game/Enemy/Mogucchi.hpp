#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MogucchiHill;
class ModelObj;

class Mogucchi : public LiveActor {
public:
    Mogucchi(const char*);
    virtual ~Mogucchi();

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeStroll();
    void endStroll();
    void exeAppearDown();
    void exeDive();
    void exeScatter();
    void exeDie();
    void exeDown();
    void initSensor();
    void createMogucchiHill();
    void createHole();
    void updatePosition();
    void updateReferenceMtx();
    bool checkHipDrop() const;
    void updateStrollSpeed();
    void reflectStarPointer2P();
    void makeEulerRotation();
    bool calcAttackDir(TVec3f*, const TVec3f&, const TVec3f&) const;
    bool receiveAttackBySpinSensor(u32, HitSensor* pSender, HitSensor* pReceiver);
    bool receiveAttackByBodySensor(u32, HitSensor* pSender, HitSensor* pReceiver);

    /* 0x8C */ MogucchiHill* mHill;
    /* 0x90 */ ModelObj* mHole;
    TMtx34f _94;
    TVec3f _C4;
    bool _D0;
    f32 _D4;
    TVec3f _D8;
    f32 _E4;
    /* 0xE8 */ f32 mStrollSpeed;
    /* 0xEC */ f32 mMaxStrollSpeed;
    /* 0xF0 */ bool mIsStoppedByP2;

private:
    u8 mPad[(0xF4) - sizeof(LiveActor)];
};
