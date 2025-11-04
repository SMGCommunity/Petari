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
    bool isEnablePushPlayer() const;
    bool isEnableEnemyAttack() const;
    bool isEnableBallBind() const;
    bool isEnebleHitCollisionMessage() const;
    bool isUseMarioOffset() const;
    bool isGroundMomentBall() const;
    bool isRideRail() const;
    bool isNeedTutorial() const;
    void startRotateLevelSound();

private:
    /* 0x08C */ SphereAccelSensorController* _8C;
    /* 0x090 */ TamakoroTutorial* mTutorial;
    /* 0x094 */ JointController* _94;
    /* 0x098 */ TQuat4f _98;
    /* 0x0A8 */ TQuat4f _A8;
    /* 0x0B8 */ TVec3f _B8;
    /* 0x0C4 */ u8 _C4[0xC];
    /* 0x0D0 */ TVec3f _D0;
    /* 0x0DC */ TVec3f _DC;
    /* 0x0E8 */ TVec3f _E8;
    /* 0x0F4 */ TVec3f _F4;
    /* 0x100 */ TVec3f _100;
    /* 0x10C */ TVec3f _10C;
    /* 0x118 */ TVec3f _118;
    /* 0x124 */ TVec3f _124;
    /* 0x130 */ f32 _130;
    /* 0x134 */ f32 _134;
    /* 0x138 */ f32 _138;
    /* 0x13C */ f32 _13C;
    /* 0x140 */ u32 _140;
    /* 0x144 */ AudTamakoroBgmCtrl* _144;
    /* 0x148 */ bool _148;
    /* 0x149 */ bool _149;
};

namespace MR {
    void createTamakoroWithTutorial(const char*);
};
