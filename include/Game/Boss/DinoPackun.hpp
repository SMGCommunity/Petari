#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JointController.hpp"

class DinoPackunTail;
class DinoPackunBall;
class FootPrint;
class DinoPackunDemoPosition;
class DinoPackunEggShell;
class PartsModel;
class AnimScaleController;
class DinoPackunSequencer;

class DinoPackun : public LiveActor {
public:
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

    DinoPackunTail* mTail;             // 0x8C
    DinoPackunBall* mBall;             // 0x90
    FootPrint* mFootPrint;             // 0x94
    DinoPackunEggShell* mShell;        // 0x98
    DinoPackunDemoPosition* mDemoPos;  // 0x9C
    PartsModel* mShellBreakModel;      // 0xA0
    PartsModel* mTailBall;             // 0xA4
    CameraTargetMtx* mCamTargetMtx;    // 0xA8
    ActorCameraInfo* mCameraInfo;      // 0xAC
    DinoPackunSequencer* mSequence;    // 0xB0
    JointControlDelegator< DinoPackun >* _B4;
    AnimScaleController* _B8;
    TQuat4f _BC;
    TVec4f _CC;
    TVec3f _DC;
    TVec3f _E8;
    s32 _F4;
    TVec3f mCameraVec;  // 0xF8
    f32 _104;
    f32 _108;
    s32 _10C;
    u8 _110;
};
