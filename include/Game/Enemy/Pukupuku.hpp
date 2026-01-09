#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class ParabolicPath;
class ValueControl;
class Pukupuku;
class WalkerStateBindStarPointer;
class AnimScaleController;

class PukupukuStateLanding : public ActorStateBase< Pukupuku > {
public:
    PukupukuStateLanding(Pukupuku*);

    virtual ~PukupukuStateLanding();

    void exeLandingMoveLand();
    void exeLandingJumpFromWater();
    void exeLandingJumpFromLand();
    void updatePoseByJumpPath(f32);
    void setNerveAfterJumpAccordingToNextPoint();
    void setupJumping(f32, f32);
    void updateJumping();
    void emitWaterColumIfNeed(bool, bool);
    void emitGroundHitEffect();

    Pukupuku* mParent;         // 0xC
    ParabolicPath* mPath;      // 0x10
    ValueControl* mValueCtrl;  // 0x14
    u8 _18;
};

class Pukupuku : public LiveActor {
public:
    Pukupuku(const char*);

    virtual ~Pukupuku();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void updatePoseByRailIgnoreUpScale();
    void rotatePoseByLocalZ();
    void calcGroundHitMtx(TPos3f*) const;
    bool isReadyToJumpFromLand() const NO_INLINE;
    void exeWait();
    void exeMoveWater();
    void exeMoveWaterAfterJump();
    void exeLanding();
    void exeTrampled();
    void exeBlownOff();
    void exeBindStarPointer();
    void updatePoseByRail();
    void updateMoveWaterAfterJump();
    bool tryBindStarPointer();
    void exeMoveWaterCommon();
    f32 getBlownOffSpeedRate() const;

    inline void startAnim(const char* pBck, const char* pBtp) {
        MR::startBck(this, pBck, nullptr);
        MR::startBtp(this, pBtp);
    }

    inline void getCurrentRailPointPos(TVec3f* pOut) { MR::calcRailPointPos(pOut, this, MR::getCurrentRailPointNo(this)); }

    inline void getNextRailPointPos(TVec3f* pOut) { MR::calcRailPointPos(pOut, this, MR::getNextRailPointNo(this)); }

    inline void exeMoveWaterAfterJumpAfterPointing() { updateMoveWaterAfterJump(); }

    PukupukuStateLanding* mStateLanding;  // 0x8C
    TVec3f _90;
    TVec3f _9C;
    TQuat4f _A8;
    AnimScaleController* mScaleCtrl;           // 0xB8
    WalkerStateBindStarPointer* mStarPointer;  // 0xBC
    const char* _C0;
    Nerve* _C4;
    f32 _C8;
    f32 _CC;
    u8 _D0;
    u8 _D1;
    u8 _D2;
    u8 _D3;
};
