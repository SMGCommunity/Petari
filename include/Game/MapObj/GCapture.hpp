#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CameraTargetMtx;
class GCaptureRibbon;
class GCaptureTargetable;
class SpringValue;

class GCapture : public LiveActor {
public:
    GCapture(const char*);

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
    void endBindByDamage(s32);
    void endBindByFireDamage();
    void calcBindActorPose();
    void updateBindActorMatrix();
    bool canRequestTarget() const NO_INLINE;
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

    void exeCoolDown();
    void exeWait();
    void exeCapture();
    void endCapture();
    void exeRecapture();
    void endRecapture();
    void exeTraction();
    void exeHold();
    void exeBreak();

    /* 0x8C */ TPos3f _8C;
    /* 0xBC */ TQuat4f _BC;
    /* 0xCC */ TVec3f mEffectPos;
    /* 0xD8 */ TVec3f _D8;
    /* 0xE4 */ TVec3f _E4;
    /* 0xF0 */ TVec3f _F0;
    /* 0xFC */ TVec3f _FC;
    /* 0x108 */ LiveActor* _108;
    /* 0x10C */ GCaptureTargetable* mTarget;
    /* 0x110 */ GCaptureTargetable* _110;
    /* 0x114 */ GCaptureRibbon* mCaptureRibbon;
    /* 0x118 */ CameraTargetMtx* mTargetMtx;
    /* 0x11C */ SpringValue* mSpringValue;
    /* 0x120 */ f32 mTractPower;
    /* 0x124 */ f32 _124;
    /* 0x128 */ f32 _128;
    /* 0x12C */ s32 _12C;
    /* 0x130 */ f32 _130;
    /* 0x134 */ s32 mGuidanceTime;
    /* 0x138 */ bool mIsStarPointerPointing;  // useless
    /* 0x139 */ bool _139;                    // useless
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
