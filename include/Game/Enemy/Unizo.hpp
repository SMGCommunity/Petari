#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class WalkerStateBindStarPointer;

class Unizo : public LiveActor {
public:
    Unizo(const char*);
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void appear();
    virtual void kill();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual ~Unizo();
    virtual MtxPtr getBaseMtx() const;

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
    /* 0x08C */ s32 _8C; // Okay I have no idea
    /* 0x090 */ s32 _90;
    /* 0x094 */ u32 mPad[3];
    /* 0x0A0 */ TPos3f _A0;
    /* 0x0D0 */ TMtx34f _D0;
    /* 0x100 */ u32 _100;
    /* 0x104 */ s32 _104; // Not sure
    /* 0x108 */ u32 mPad2[3];
    /* 0x114 */ LiveActor* _114; // It's a LiveActor, idk which one
    /* 0x118 */ AnimScaleController* _118;
    /* 0x11C */ WalkerStateBindStarPointer* _11C;
    /* 0x120 */ s32 _120;
    /* 0x124 */ u32 mPad3[4];   // Total: 0x134
};
