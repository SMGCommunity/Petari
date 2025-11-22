#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AudTamakoroBgmCtrl;
class JointController;
class JointControllerInfo;
class SphereAccelSensorController;
class TamakoroTutorial;

class Tamakoro : public LiveActor {
public:
    /// @brief Creates a new `Tamakoro`.
    /// @param pName A pointer to the null-terminated name of the object.
    Tamakoro(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSensor();
    void initJointControl();
    bool ringMtxCallBack(TPos3f*, const JointControllerInfo&);
    void updateBindActorMatrix();
    void updateRingUpVec();
    bool requestBind(HitSensor*);
    bool requestEndBind();
    void startRide();
    bool requestTutorialEnd();
    bool tryFall();
    bool tryJump();
    bool tryBumpWall();
    bool tryLand();
    void reactionCollision();
    void exeStandByTutorial();
    void exeStandByBind();
    void exeBindStart();
    void exeBindStartLand();
    void exeTutorial();
    void exeWait();
    void exeFall();
    void exeJump();
    void exeBumpWall();
    void exeLand();
    void exeJumpHole();
    void exeJumpHoleSetUp();
    void exeJumpHoleLaunch();
    void endJumpHoleLaunch();
    void exeDashRail();
    void exeDashRailEnd();
    void exeRideRail();
    void exeRideRailFastStart();
    void exeRideRailFast();
    void exeRideRailFastEnd();
    void exeBindEnd();
    void addVelocityOperate();
    f32 updateRideRail();
    void updateMoment();
    void updateAirTime();
    void updateMarioPose(f32);
    void updateMarioOffset();
    void updateMoveBckBlend(f32);
    void updateSquatBckBlend(f32);
    bool isEnablePushPlayer() const NO_INLINE;
    bool isEnableEnemyAttack() const NO_INLINE;
    bool isEnableBallBind() const NO_INLINE;
    bool isEnebleHitCollisionMessage() const NO_INLINE;
    bool isUseMarioOffset() const NO_INLINE;
    bool isGroundMomentBall() const NO_INLINE;
    bool isRideRail() const NO_INLINE;
    bool isNeedTutorial() const NO_INLINE;
    void startRotateLevelSound();

    inline void setTutorial() { mHasTutorial = true; }

private:
    /* 0x08C */ SphereAccelSensorController* mAccelSensorCtrl;
    /* 0x090 */ TamakoroTutorial* mTutorial;
    /* 0x094 */ JointController* mJointCtrl;
    /* 0x098 */ TQuat4f mBaseQuat;
    /* 0x0A8 */ TQuat4f mRotateQuat;
    /* 0x0B8 */ TVec3f mRingUp;
    /* 0x0C4 */ u8 _C4[0xC];
    /* 0x0D0 */ TVec3f mKickVel;
    /* 0x0DC */ TVec3f mMarioBindRequestPos;
    /* 0x0E8 */ TVec3f mMarioRotateUp;
    /* 0x0F4 */ TVec3f mMarioPos;
    /* 0x100 */ TVec3f mMoment;
    /* 0x10C */ TVec3f mMarioRotateFront;
    /* 0x118 */ TVec3f mDirectionToMario;
    /* 0x124 */ TVec3f mMoveVec;
    /* 0x130 */ f32 mMoveSpeed;
    /* 0x134 */ f32 mMarioRotateYAngle;
    /* 0x138 */ f32 mMarioOffset;
    /* 0x13C */ f32 mMarioOffsetVelocity;
    /* 0x140 */ s32 mAirTime;
    /* 0x144 */ AudTamakoroBgmCtrl* mBgmCtrl;
    /* 0x148 */ bool mControlDisabled;
    /* 0x149 */ bool mHasTutorial;
};

namespace MR {
    Tamakoro* createTamakoroWithTutorial(const char*);
};
