#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;

class Unizo : public LiveActor {
public:
    Unizo(const char*);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void appear();
    virtual void kill();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual MtxPtr getBaseMtx() const;
    virtual ~Unizo();

    void initType(const JMapInfoIter&);
    void updateRotate();
    void updateVelocity();
    void updateSurfaceEffect();
    bool tryPointBind();
    bool isBreakNow() const;
    bool isEnablePointBind() const;
    void udpateInfluence(); // Nice one Nintendo
    void doBreak();
    void doAttack(HitSensor*);
    void appearBreakModel();
    void deleteEffect();

    void exeJump();
    void exeWait();
    void exeChase();
    void exeAttack();
    void exeCollidePlayer();
    void exeCollideEnemy();
    void exeBreak();
    void exeFireDown();
    void exeJumpDown();
    void exePointing();
    void endPointing();

private:
    s32 _8C; // Okay I have no idea
    s32 _90;
    u32 mPad[3];
    TPos3f _A0;
    TMtx34f _D0;
    u32 _100;
    s32 _104; // Not sure
    u32 mPad2[3];
    LiveActor* _114; // It's a LiveActor, idk which one
    AnimScaleController* _118;
    WalkerStateBindStarPointer* _11C;
    s32 _120;
    u32 mPad3[4];   // Total: 0x134
};
