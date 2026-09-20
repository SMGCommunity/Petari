#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ParabolicPath;
class ValueControl;
class Pukupuku;
class WalkerStateBindStarPointer;
class AnimScaleController;

class PukupukuStateLanding : public ActorStateBase< Pukupuku > {
public:
    PukupukuStateLanding(Pukupuku* pParent);

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

    /* 0x10 */ ParabolicPath* mPath;
    /* 0x14 */ ValueControl* mValueCtrl;
    /* 0x18 */ u8 _18;
};

class Pukupuku : public LiveActor {
public:
    Pukupuku(const char* pName);

    virtual ~Pukupuku();
    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void updatePoseByRailIgnoreUpScale();
    void rotatePoseByLocalZ();
    void calcGroundHitMtx(TPos3f* pMtx) const;
    bool isReadyToJumpFromLand() const NO_INLINE;
    bool isDamaged() const;
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

    inline void getCurrentRailPointPos(TVec3f* pOut) {
        MR::calcRailPointPos(pOut, this, MR::getCurrentRailPointNo(this));
    }

    inline void getNextRailPointPos(TVec3f* pOut) {
        MR::calcRailPointPos(pOut, this, MR::getNextRailPointNo(this));
    }

    inline void exeMoveWaterAfterJumpAfterPointing() {
        updateMoveWaterAfterJump();
    }

    /* 0x8C */ PukupukuStateLanding* mStateLanding;
    /* 0x90 */ TVec3f _90;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ TQuat4f _A8;
    /* 0xB8 */ AnimScaleController* mScaleCtrl;
    /* 0xBC */ WalkerStateBindStarPointer* mStarPointer;
    /* 0xC0 */ const char* _C0;
    /* 0xC4 */ Nerve* _C4;
    /* 0xC8 */ f32 _C8;
    /* 0xCC */ f32 _CC;
    /* 0xD0 */ u8 _D0;
    /* 0xD1 */ u8 _D1;
    /* 0xD2 */ u8 _D2;
    /* 0xD3 */ u8 _D3;
};
