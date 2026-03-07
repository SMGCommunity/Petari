#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/SpinDriverCamera.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"

class SpinDriver : public LiveActor {
public:
    SpinDriver(const char*);

    virtual ~SpinDriver();
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
    void cancelCamera();
    void endCamera();
    bool canStartBind() const NO_INLINE;
    bool canBind(HitSensor*) const;

    LiveActor* _8C;
    SpinDriverShootPath* mShootPath;      // 0x90
    SpinDriverCamera* mSpinDriverCamera;  // 0x94
    TQuat4f _98;
    TQuat4f _A8;
    TVec3f _B8;
    TVec3f _C4;
    TVec3f _D0;
    TVec3f _DC;
    TVec3f _E8;
    TVec3f _F4;
    f32 _100;
    f32 _104;
    f32 _108;
    TVec3f _10C;
    f32 _118;
    f32 _11C;
    f32 _120;
    s32 _124;
    u32 _128;
    u32 _12C;
    u32 _130;
    f32 _134;
    u8 _138;
    s32 _13C;
    bool _140;
    u8 _141;
    bool _142;
};
