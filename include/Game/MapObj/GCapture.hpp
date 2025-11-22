#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class GCaptureRibbon;
class CameraTargetMtx;
class SpringValue;
class GCaptureTargetable;

class GCapture : public LiveActor {
public:
    GCapture(const char*);

    virtual ~GCapture();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool requestBind(HitSensor*);
    bool requestCancelBind();
    bool requestDamageCancel(u32);
    void reset();
    bool tryCapture();
    bool tryCancelCapture();
    bool tryEndTraction();
    bool tryRecapture();
    bool tryRetraction();
    bool tryBreak();
    bool tryFireDamage();
    bool tryRelease();
    void exeCoolDown();
    void exeWait();
    void exeCapture();
    void endCapture();
    void exeRecapture();
    void endRecapture();
    void exeTraction();
    void exeHold();
    void exeBreak();
    void endBindByDamage(s32);
    void endBindByFireDamage();
    void calcBindActorPose();
    void updateBindActorMatrix();
    bool canRequestTarget() const;
    bool canCancelBind() const;
    void upTractPower();
    void downTractPower();
    void fillGapBindTrans();
    void updateRibbon(const TVec3f&, s32);
    void updateRibbonPointEffectMatrix(const TVec3f&);
    void addRotateAccelPointing();
    bool tryAddVelocityReflectJumpCollision();
    bool requestTarget(GCaptureTargetable*);
    bool isRequestedTarget(GCaptureTargetable*);
    void decideTractTarget();
    void releaseTractTarget();
    void updateCameraTargetMatrix();

    TPos3f _8C;
    f32 _BC;
    TVec3f _C0;
    TVec3f _CC;
    TVec3f _D8;
    TVec3f _E4;
    TVec3f _F0;
    TVec3f _FC;
    u32 _108;
    GCaptureTargetable* mTarget;  // 0x10C
    u32 _110;
    GCaptureRibbon* mCaptureRibbon;  // 0x114
    CameraTargetMtx* mTargetMtx;     // 0x118
    SpringValue* mSpringValue;       // 0x11C
    f32 mTractPower;                 // 0x120
    f32 _124;
    f32 _128;
    u32 _122C;
    f32 _130;
    u32 _134;
    u8 _138;
    u8 _139;
};

namespace MR {
    void createGCapture();
    void resetGCapture();
    bool requestGCaptureTarget(GCaptureTargetable*);
    void unrequestGCaptureTarget(GCaptureTargetable*);
    bool isRequestedGCaptureTarget(GCaptureTargetable*);
    void noticeInTouchableRange();
    bool isOnTractTrigger();
    bool isPlayerGCaptured();
};  // namespace MR
