#pragma once

#include "Game/Boss/DodoryuStateBase.hpp"

class DodoryuStateLv1 : public DodoryuStateBase {
public:
    DodoryuStateLv1(Dodoryu* pHost, const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    virtual void start();
    virtual void catchAttackSensor(HitSensor*, HitSensor*);
    virtual void catchHipDrop();
    virtual bool catchPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual void catchStartClipped();

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
    void exeAppearSign();
    void exeAppearStart();
    void exeAppear();
    void exeAppearEnd();
    void exeJumpOut();
    void exeDamage();
    void exeDive();
    void exeSpinOut();

    /* 0x90 */ s32 _90;
    /* 0x94 */ s32 _94;
    /* 0x98 */ TVec3f _98;
    /* 0xA4 */ bool _A4;
    /* 0xA5 */ bool _A5;
    /* 0xA8 */ f32 _A8;
    /* 0xAC */ f32 _AC;
    /* 0xB0 */ f32 _B0;
    /* 0xB4 */ f32 _B4;
};
