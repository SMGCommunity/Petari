#pragma once

#include "Game/LiveActor/HitSensor.hpp"
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
    void updateVelocityNormal(f32);
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

    inline void setTutorial() {
        mHasTutorial = true;
    }

    inline bool isBindedSphereDash(HitSensor* pSender, HitSensor* pReceiver) const {
        if (pReceiver->isType(ATYPE_SPHERE_DASH)) {
            return pReceiver->receiveMessage(ACTMES_SPHERE_PLAYER_BINDED, pSender);
        }
        return false;
    }

    inline bool isBindedJumpHole(HitSensor* pSender, HitSensor* pReceiver) const {
        if (pReceiver->isType(ATYPE_JUMP_HOLE)) {
            return pReceiver->receiveMessage(ACTMES_SPHERE_PLAYER_BINDED, pSender);
        }
        return false;
    }

    inline bool isBindedBallRail(HitSensor* pSender, HitSensor* pReceiver) const {
        if (pReceiver->isType(ATYPE_BALL_RAIL)) {
            return pReceiver->receiveMessage(ACTMES_SPHERE_PLAYER_BINDED, pSender);
        }
        return false;
    }

private:
    /* 0x08C */ SphereAccelSensorController* mAccelSensorCtrl;
    /* 0x090 */ TamakoroTutorial* mTutorial;
    /* 0x094 */ JointController* mJointCtrl;
    /* 0x098 */ TQuat4f mBallRotateQuat;
    /* 0x0A8 */ TQuat4f mMarioRotateQuat;
    /* 0x0B8 */ TVec3f mRingUp;
    /* 0x0C4 */ TVec3f _C4;  // unused
    /* 0x0D0 */ TVec3f mKickVel;
    /* 0x0DC */ TVec3f mMarioBindRequestPos;
    /* 0x0E8 */ TVec3f mMarioUp;
    /* 0x0F4 */ TVec3f mMarioPos;
    /* 0x100 */ TVec3f mMoment;
    /* 0x10C */ TVec3f mMarioFront;
    /* 0x118 */ TVec3f mDirectionToMario;
    /* 0x124 */ TVec3f mAccelDir;
    /* 0x130 */ f32 mAccelRate;
    /* 0x134 */ f32 mMarioRotateYAngle;
    /* 0x138 */ f32 mMarioOffset;
    /* 0x13C */ f32 mMarioOffsetVelocity;
    /* 0x140 */ s32 mAirTime;
    /* 0x144 */ AudTamakoroBgmCtrl* mBgmCtrl;
    /* 0x148 */ bool mControlDisabled;
    /* 0x149 */ bool mHasTutorial;
};

namespace MR {
    NameObj* createTamakoroWithTutorial(const char*);
};
