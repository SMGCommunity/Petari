#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Kabokuri : public LiveActor {
public:
    Kabokuri(const char*);
    virtual ~Kabokuri();

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void initSensor();
    void updatePose();
    void addVelocityBase();
    void addVelocityToRailPoint(f32);
    void requestTrampled();
    void requestStarPieceHitted();
    void requestHipDropped();
    void requestHitAttacked(HitSensor*, HitSensor*);
    void tryPointBind();
    void exeWait();
    void exeWalk();
    void exeDropFire();
    void exeAttackSuccess();
    void exeTrampled();
    void exeStarPieceHitted();
    void exeHipDropped();
    void exeHitAttacked();
    void exeBreak();
    void exeBindStarPointer();
    bool isEnableAttack() const;
    bool isEnablePointBind() const;
    bool isEnableTrampled() const;
    bool isEnablePush() const;

private:
    u8 mPad[(0xC0) - sizeof(LiveActor)];
};
