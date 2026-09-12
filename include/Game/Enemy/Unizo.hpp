#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class ModelObj;
class WalkerStateBindStarPointer;

class Unizo : public LiveActor {
public:
    enum Type { TypeSea, TypeLand, TypeShoal };

    Unizo(const char*);

    virtual ~Unizo();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual MtxPtr getBaseMtx() const;
    virtual void startClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void initType(const JMapInfoIter&);
    void updateRotate();
    void updateVelocity();
    void udpateInfluence();
    void udpateBlink();
    void updateSurfaceEffect();
    void deleteEffect();
    void doJump();
    void doAttack(HitSensor*);
    void doJumpDown();
    void doFireDown(const TVec3f&);
    void doBreak();
    void doSpin();
    bool isBreakGround();
    bool isBreakNow() const;
    bool tryPointBind();
    bool isEnablePointBind() const;
    void appearBreakModel();
    void startWallHitSound();

    void exeWait();
    void exeJump();
    void exeChase();
    void exeAttack();
    void exeCollidePlayer();
    void exeCollideEnemy();
    void exeBreak();
    void exeFireDown();
    void exeJumpDown();
    void exePointing();
    void endPointing();

    /* 0x08C */ s32 mType;
    /* 0x090 */ TQuat4f mQuat;
    /* 0x0A0 */ TPos3f mBaseMtx;
    /* 0x0D0 */ TPos3f mRippleMtx;
    /* 0x100 */ s32 mFrame;
    /* 0x104 */ s32 mBlinkCount;
    /* 0x108 */ s32 mBlinkFrame;
    /* 0x10C */ s32 mSpinAnimTimer;
    /* 0x110 */ f32 mRadiusScale;
    /* 0x114 */ ModelObj* mBreakModel;
    /* 0x118 */ AnimScaleController* mAnimScaleController;
    /* 0x11C */ WalkerStateBindStarPointer* mBindStarPointerState;
    /* 0x120 */ s32 mJumpCount;
    /* 0x124 */ s32 _124;
    /* 0x128 */ f32 mGravityRate;
    /* 0x12C */ f32 mRollHeight;
    /* 0x130 */ s32 mRollSoundTimer;
};
