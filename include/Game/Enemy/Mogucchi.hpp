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
    void calcAttackDir(TVec3f*, const TVec3f&, const TVec3f&) const;
    bool receiveAttackBySpinSensor(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    bool receiveAttackByBodySensor(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    void calcScatterVec(const TVec3f&, const TVec3f&);

    /* 0x8C */ MogucchiHill* mHill;
    /* 0x90 */ ModelObj* mHole;
    /* 0x94 */ TPos3f mNewHolePos;
    /* 0xC4 */ TVec3f mRailGravity;
    /* 0xD0 */ bool mGrounded;
    /* 0xD4 */ f32 mScatterPropulsionSpeed;
    /* 0xD8 */ TVec3f mScatterNormal;
    f32 _E4;
    /* 0xE8 */ f32 mStrollSpeed;
    /* 0xEC */ f32 mMaxStrollSpeed;
    /* 0xF0 */ bool mIsStoppedByP2;

private:
    u8 mPad[(0xF4) - sizeof(LiveActor)];
};
