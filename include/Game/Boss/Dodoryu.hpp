#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class CameraTargetMtx;
class DodoryuBank;
class DodoryuHill;
class DodoryuLeadHill;
class DodoryuRabbit;
class DodoryuStateBase;

namespace DodoryuSub {
    class MoveStateHolder;
};  // namespace DodoryuSub

class Dodoryu : public LiveActor {
public:
    /// @brief Creates a new `Dodoryu`.
    /// @param pName A pointer to the null-terminated name of the object.
    Dodoryu(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void snapToGround();
    bool snapToWall();
    void setMtx(const TPos3f&);
    void notifyOnSwitchA();
    void nextState();
    void pauseOff();
    void startPlayerCenterCamera();
    void endPlayerCenterCamera();
    void getPlayerResetMtx(TPos3f*, u32) const;
    bool checkWallWithVelocity();
    void reactJumpOutCommon() const;
    void startSpinOutCamera();
    void endSpinOutCamera();
    bool keepOffFromClosedArea(TVec3f*);
    void tryRumblePad();
    void invalidateEyeSensor();
    void validateEyeSensor();
    void invalidateStarPieceSensor();
    void validateStarPieceSensor();
    bool isHeadNeedle(HitSensor*, HitSensor*) const;
    void shiftMoveStateRail(f32);
    void shiftMoveStateChase(f32, f32, f32, f32);
    void shiftMoveStateEscape(f32, s32, s32, f32, f32);
    void shiftMoveStateJumpOut(f32, f32);
    void shiftMoveStateNull();
    void setMoveRailSpeed(f32);
    void startHill();
    void stopHill();
    void stopHillForce();
    void killAllHillPieces();
    void setHillAppearNumHalf();
    void setHillAppearNumMax();
    void startLeadHillBck(const char*);
    void killLeadHill();
    void appearBank();
    void leaveRabbit();
    void resetRabbit();
    void calcRabbitDir(TVec3f*) const;
    void displayRabbitMessage();
    void initHitSensor();
    void initSwitch(const JMapInfoIter&);
    void createMogucchiHill();
    void createDodoryuBank();
    void turnUpVecTo(const TVec3f&);
    void checkHipDrop();
    void initState();
    void updateRumblePad();
    void updateCameraTarget();

    /* 0x08C */ TPos3f mBaseMtx;
    /* 0x0BC */ TVec3f _BC;
    /* 0x0C8 */ u8 _C8;
    /* 0x0CC */ u32 _CC;
    /* 0x0D0 */ MR::Vector< MR::FixedArray< DodoryuStateBase*, 16 > > mState;
    /* 0x114 */ DodoryuSub::MoveStateHolder* mMoveStateHolder;
    /* 0x118 */ DodoryuHill* mHill;
    /* 0x11C */ DodoryuLeadHill* mLeadHill;
    /* 0x120 */ DodoryuBank* mBank;
    /* 0x124 */ DodoryuRabbit* mRabbit;
    /* 0x128 */ TVec3f _128;
    /* 0x134 */ TVec3f _134;
    /* 0x140 */ AreaObj* _140;
    /* 0x144 */ u8 _144;
    /* 0x148 */ CameraTargetMtx* _148;
    /* 0x14C */ CameraTargetMtx* _14C;
    /* 0x150 */ u8 _150;
    /* 0x154 */ u32 _154;
    /* 0x158 */ AnimScaleController* mAnimScaleCtrl;
};
