#pragma once

#include "Game/Boss/DodoryuStateBase.hpp"

class DodoryuStateLv1 : public DodoryuStateBase {
public:
    DodoryuStateLv1(Dodoryu* pHost, const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x48 */ virtual void control();

    /* 0x78 */ virtual void start();
    /* 0x80 */ virtual void catchAttackSensor(HitSensor*, HitSensor*);
    /* 0x84 */ virtual void catchHipDrop();
    /* 0x88 */ virtual bool catchPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x8C */ virtual void catchStartClipped();

    bool isAttackable() const;
    void executeHideJump();
    void startPop();
    void updatePop();
    bool isHipDropReactionJumpOut() const;
    bool isHipDropReactionPop() const;
    bool isHipDropReactionNothing() const;
    void setRailSpeed(f32);
    void startAnimAppear();
    void startAnimWalk();
    void startAnimHide();
    void startAnimEffectWalkAppear();
    void startAnimEffectWalk();
    void startAnimEffectWalkHide();

    void exeHide();
    void endHide();
    void exeAppearSign();
    void exeAppearStart();
    void endAppearStart();
    void exeAppear();
    void endAppear();
    void exeAppearEnd();
    void exeJumpOut();
    void exeDamage();
    void exeDive();
    void exeSpinOut();

    /* 0x90 */ s32 mHideStep;
    /* 0x94 */ s32 mAppearStep;
    /* 0x98 */ TVec3f _98;
    /* 0xA4 */ bool _A4;
    /* 0xA5 */ bool mIsValidPop;
    /* 0xA8 */ f32 mPopAccel;
    /* 0xAC */ f32 mPopSpeed;
    /* 0xB0 */ f32 mRailSpeed;
    /* 0xB4 */ f32 mRailSpeedMax;
};
