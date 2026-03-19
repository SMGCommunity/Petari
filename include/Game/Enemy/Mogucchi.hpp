#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class MogucchiHill;

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

    MogucchiHill* mHill;
    LiveActor* _90;
    Mtx _94;
    TVec3f _C4;
    bool _D0;
    f32 _D4;
    TVec3f _D8;
    f32 _E4;
    f32 _E8;
    f32 _EC;
    bool _F0;

private:
    u8 mPad[(0xF4) - sizeof(LiveActor)];
};
