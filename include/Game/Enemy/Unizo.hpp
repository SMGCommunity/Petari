#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class WalkerStateBindStarPointer;

class Unizo : public LiveActor {
public:
    Unizo(const char*);
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x08 */ virtual ~Unizo();
    /* 0x38 */ virtual MtxPtr getBaseMtx() const;

    void initType(const JMapInfoIter&);
    void updateRotate();
    void updateVelocity();
    void updateSurfaceEffect();
    bool tryPointBind();
    bool isBreakNow() const;
    bool isEnablePointBind() const;
    void udpateInfluence();
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
    /* 0x08C */ s32 _8C;  // Okay I have no idea
    /* 0x090 */ s32 _90;
    /* 0x094 */ u32 mPad[3];
    /* 0x0A0 */ TPos3f _A0;
    /* 0x0D0 */ TMtx34f _D0;
    /* 0x100 */ u32 _100;
    /* 0x104 */ s32 _104;  // Not sure
    /* 0x108 */ u32 mPad2[3];
    /* 0x114 */ LiveActor* _114;  // It's a LiveActor, idk which one
    /* 0x118 */ AnimScaleController* _118;
    /* 0x11C */ WalkerStateBindStarPointer* _11C;
    /* 0x120 */ s32 _120;
    /* 0x124 */ u32 mPad3[4];  // Total: 0x134
};
