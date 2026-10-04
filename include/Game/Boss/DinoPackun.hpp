#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class AnimScaleController;
class CameraTargetMtx;
class DinoPackunBall;
class DinoPackunDemoPosition;
class DinoPackunEggShell;
class DinoPackunSequencer;
class DinoPackunTail;
class JointControllerInfo;
class FootPrint;
class PartsModel;

template < typename T >
class JointControlDelegator;

class DinoPackun : public LiveActor {
public:
    DinoPackun(const char*);

    virtual ~DinoPackun();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initTail();
    void initFootPrint();
    void initDemoPosition(const JMapInfoIter&);
    void initEggShell();
    void initBall();
    void initCamera(const JMapInfoIter&);
    void initScaleJointController();
    void startHitReaction();
    bool isHitReaction(s32) const;
    bool hitScaleJoint(TPos3f*, const JointControllerInfo&);
    DinoPackunEggShell* getEggShell();
    PartsModel* getEggBrokenModel();
    PartsModel* getBallModel();
    void attackSensorTail(HitSensor*, HitSensor*);
    bool receiveMsgPlayerAttackTail(u32, HitSensor*, HitSensor*);
    void startSequence();
    void updatePose();
    void updateFootPrintNerve(s32, s32);
    void updateCameraInfo();
    void updateNormalVelocity();
    void updateRunVelocity();
    void appearStarPiece(s32);
    bool isSensorEgg(const HitSensor*) const;
    void resetPosition();
    void adjustTailRootPosition(const TVec3f&, f32);
    void activateParts();
    void onMovementParts();
    void onAimTailBall(s32);
    void offAimTailBall(s32);
    void startDemo();
    void startDemoAndReset();
    void endDemo(const char*);
    void startDamageCamera();
    void endDamageCamera();

    /* 0x08C */ DinoPackunTail* mTail;
    /* 0x090 */ DinoPackunBall* mBall;
    /* 0x094 */ FootPrint* mFootPrint;
    /* 0x098 */ DinoPackunEggShell* mShell;
    /* 0x09C */ DinoPackunDemoPosition* mDemoPos;
    /* 0x0A0 */ PartsModel* mShellBreakModel;
    /* 0x0A4 */ PartsModel* mTailBall;
    /* 0x0A8 */ CameraTargetMtx* mCamTargetMtx;
    /* 0x0AC */ ActorCameraInfo* mCameraInfo;
    /* 0x0B0 */ DinoPackunSequencer* mSequence;
    /* 0x0B4 */ JointControlDelegator< DinoPackun >* _B4;
    /* 0x0B8 */ AnimScaleController* _B8;
    /* 0x0BC */ TQuat4f _BC;
    /* 0x0CC */ TVec4f _CC;
    /* 0x0DC */ TVec3f _DC;
    /* 0x0E8 */ TVec3f _E8;
    /* 0x0F4 */ s32 _F4;
    /* 0x0F8 */ TVec3f mCameraVec;
    /* 0x104 */ f32 _104;
    /* 0x108 */ f32 _108;
    /* 0x10C */ s32 _10C;
    /* 0x110 */ u8 _110;
};

namespace MR {
    NameObj* createDinoPackunVs1(const char*);
    NameObj* createDinoPackunVs2(const char*);
};  // namespace MR
