#pragma once

#include "Game/LiveActor/LiveActor.hpp"
class SpinDriverCamera;
class SpinDriverShootPath;

class SpinDriver : public LiveActor {
public:
    SpinDriver(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initParamFromJMapInfo(const JMapInfoIter&);
    void initShootPath(const JMapInfoIter&);
    void initEventCamera(const JMapInfoIter&);
    bool tryStartShoot();
    bool tryEndCapture();
    bool tryShoot();
    bool tryEndShoot();
    bool tryForceCancel();
    bool tryEndCoolDown();
    bool trySwitchOff();

    void exeTryDemo();
    void exeNonActive();
    void exeAppear();
    void exeWait();
    void exeCapture();
    void exeShootStart();
    void exeShoot();
    void exeCoolDown();

    bool startBind(HitSensor*);
    void cancelBind();
    void updateBindPosition();
    void moveBindPosToCenter();
    void updateBindActorMatrix(f32);
    void calcBindActorMatrix();
    void calcParabolicBindPose();
    void turnBindHead(const TVec3f&, f32);
    void calcShootMotionTime();
    void startCamera();
    void updateCamera();
    void cancelCamera() NO_INLINE;
    void endCamera() NO_INLINE;
    bool canStartBind() const NO_INLINE;
    bool canBind(HitSensor*) const;

    /* 0x08C */ LiveActor* _8C;
    /* 0x090 */ SpinDriverShootPath* mShootPath;
    /* 0x094 */ SpinDriverCamera* mSpinDriverCamera;
    /* 0x098 */ TQuat4f _98;
    /* 0x0A8 */ TQuat4f _A8;
    /* 0x0B8 */ TVec3f _B8;
    /* 0x0C4 */ TVec3f _C4;
    /* 0x0D0 */ TVec3f _D0;
    /* 0x0DC */ TVec3f _DC;
    /* 0x0E8 */ TVec3f _E8;
    /* 0x0F4 */ TVec3f _F4;
    /* 0x100 */ f32 _100;
    /* 0x104 */ f32 _104;
    /* 0x108 */ f32 _108;
    /* 0x10C */ TVec3f _10C;
    /* 0x118 */ f32 _118;
    /* 0x11C */ f32 _11C;
    /* 0x120 */ f32 _120;
    /* 0x124 */ s32 _124;
    /* 0x128 */ s32 _128;
    /* 0x12C */ s32 _12C;
    /* 0x130 */ s32 _130;
    /* 0x134 */ f32 _134;
    /* 0x138 */ bool _138;
    /* 0x13C */ s32 _13C;
    /* 0x140 */ bool _140;
    /* 0x141 */ bool _141;
    /* 0x142 */ bool _142;
};
