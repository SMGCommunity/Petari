#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class Kuribo;
class ModelObj;
class WalkerStateBindStarPointer;

/// @brief Jack O'Goomba
class Kabokuri : public LiveActor {
public:
    Kabokuri(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void initSensor();
    void updatePose();
    void addVelocityBase();
    void addVelocityToRailPoint(f32);
    bool requestTrampled();
    bool requestStarPieceHitted();
    bool requestHipDropped();
    bool requestHitAttacked(HitSensor*, HitSensor*);
    bool tryPointBind();
    void exeWait();
    void exeWalk();
    void exeDropFire();
    void exeAttacksuccess();
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
    void endBindStarPointer() const;

    /* 0x8C */ Kuribo* mKuribo;
    /* 0x90 */ ModelObj* mBreakModel;
    /* 0x94 */ AnimScaleController* mAnimeScale;
    /* 0x98 */ WalkerStateBindStarPointer* mStateBindStartPointer;
    /* 0x9C */ TQuat4f mRotationQuat;
    /* 0xAC */ TVec3f mFrontVec;
    /* 0xB8 */ s32 _B8;
    /* 0xBC */ bool mIsFloating;

    /// @brief Drops fire on every point of the rail reached, except the first one.
    /* 0xBD */ bool mIsValidDropFire;
};
