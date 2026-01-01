#pragma once

#include "Game/Boss/Dodoryu.hpp"
#include "Game/Boss/DodoryuStateBase.hpp"

class DodoryuChaseParam;

class DodoryuStateLv2 : public DodoryuStateBase {
public:
    DodoryuStateLv2(Dodoryu*, DodoryuChaseParam*, const char*);
    virtual ~DodoryuStateLv2();

    virtual void init(const JMapInfoIter&);
    virtual void control();

    virtual void start();
    virtual void catchAttackSensor(HitSensor*, HitSensor*);
    virtual void catchHipDrop();
    virtual bool catchPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual void catchStartClipped();
    virtual void catchSendPlayerFlying(HitSensor*, HitSensor*);

    void exeStart();
    void exeReadyChase();
    void endReadyChase();
    void exeChaseHide();
    void endChaseHide();
    void exeReadyAppear();
    void endReadyAppear();
    void exeAppearSign();
    void exeChaseAppearStart();
    void endChaseAppearStart();
    void exeChaseAppear();
    void endChaseAppear();
    void exeChaseAppearEnd();
    void endChaseAppearEnd();
    void exeJumpOut();
    void exeEscape();
    void endEscape();
    void exeEscapeJump();
    void endEscapeJump();
    void exeRecover();
    void exeDive();
    void endDive();
    void exeKnockDown();
    void exeLaugh();
    void exeLaughEnd();
    void endLaughEnd();
    void exeFindPos();
    void exeHideAttack();
    void exeChaseMoreStart();
    void exeChaseMore();
    void endChaseMore();
    void exeChaseMoreEnd();

    void knockOver();
    bool isEnableToCatchHipDrop() const;
    bool isDown() const;
    bool isAttackableNerve() const;
    bool tryShiftToChaseAppear();
    bool tryShiftToChaseAppearEnd();
    bool isEscapeEnd() const;
    void updateReadyChaseShake();
    void updateChaseHideShake();
    void updateHideShake();
    void updateDiveShake();
    void calcRandomVelocity(s32);
    void keepVerticalizedVelocity();
    void attackStrongToDir(HitSensor*, HitSensor*);
    void hideJump();
    bool tryToShiftAppearSign();
    void determinChaseMoreAccelCounter();
    void determinChaseMoreBrakeCounter();
    void turnToward(const TVec3f&, f32);
    void addVelocity(bool);
    void calcLimitedRotateMtx(TPos3f*, const TVec3f&, const TVec3f&, f32);
    bool isReflectSpinAttack() const;
    void shiftChaseUnderground();
    void shiftChaseExposingHead();
    void shiftChaseExposingBust();
    void shiftChaseInAnger();
    void shiftHideAttack();
    void startAnim(const DodoryuAnimSet&);
    void startBtk(const DodoryuAnimSet&);
    void startBrk(const DodoryuAnimSet&);
    void startBva(const DodoryuAnimSet&);
    void startLeadHillBck(const DodoryuAnimSet&);

    void* _90;
    TVec3f _94;
    TVec3f _A0;
    f32 _AC;
    f32 _B0;
    f32 _B4;
    s32 _B8;
    s32 _BC;
    s32 mAppearCount;             // 0xC0
    s32 _C4;
    s32 _C8;
    s32 _CC;
    f32 _D0;
    f32 _D4;
    f32 _D8;
    s32 mFindPosCounter;          // 0xDC
    s32 _E0;
    bool _E4;
    DodoryuChaseParam* mChaseParam;  // 0xE8
    bool mPlayerStaggering;          // 0xEC
};
