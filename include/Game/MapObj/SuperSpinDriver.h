#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ModelObj.h"
#include "Game/MapObj/SpinDriverCamera.h"
#include "Game/MapObj/SpinDriverOperateRing.h"
#include "Game/MapObj/SpinDriverShootPath.h"
#include "Game/MapObj/SpinDriverPathDrawer.h"

namespace {
    static f32 sCanBindTime = 90.0f;
};

class SuperSpinDriver : public LiveActor {
public:
    SuperSpinDriver(const char *, s32);

    virtual ~SuperSpinDriver();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initParamFromJMapInfo(const JMapInfoIter &);
    void initGravityAxis();
    void initSensor();
    void initEmptyModel();
    void initEventCamera(const JMapInfoIter &);
    void initShootPath(const JMapInfoIter &);
    void initShootPathDraw(const JMapInfoIter &);
    void initColor();
    void initOperateRing();
    void initAppearState(const JMapInfoIter &);
    bool canBind(HitSensor *, HitSensor *) const;
    bool tryBind(HitSensor *, HitSensor *) const;
    bool tryEndCapture();
    bool tryForceCancel();
    bool tryShootStart();
    bool tryShoot();
    bool tryEndShoot();
    bool tryEndCoolDown();
    bool trySwitchOff();
    void requestAppear();
    void requestEmptyAppear();
    void requestActive();
    void requestHide();
    void requestShow();

    void exeEmptyNonActive();
    void exeEmptyAppear();
    void exeEmptyWait();
    void exeNonActive();
    void exeAppear();
    void exeWait();
    void exeCapture();
    void exeShootStart();
    void exeShoot();
    inline void exeCoolDown();
    inline void exeShootOnEnd();

    void updateShootMotion();
    void cancelBind();
    void endBind();
    void updateBindActorMatrix();
    void updateBindActorPoseToShoot(f32);
    void turnBindHead(const TVec3f &, f32);
    void moveBindPosToCenter();
    void startPathDraw();
    void endPathDraw();
    void updatePathDraw(f32);
    void updateOperateRate();
    void updateBindPosition(f32);
    void calcShootMotionTime();
    void addSwingSignRotateY();
    void onUse();
    void offUse();
    bool isNeedEmptyModel() const;
    bool isRightToUse() const;

    LiveActor* _8C;
    SpinDriverShootPath* mShootPath;        // _90
    SpinDriverCamera* mSpinDriverCamera;    // _94
    SpinDriverOperateRing* mOperateRing;    // _98
    SpinDriverPathDrawer* mPathDrawer;      // _9C
    ModelObj* mEmptyModel;                  // _A0
    TQuat4f _A4;
    TQuat4f _B4;
    TVec3f _C4;
    TVec3f _D0;
    TVec3f _DC;
    TVec3f _E8;
    TVec3f _F4;
    TVec3f _100;
    TVec3f _10C;
    TVec3f _118;
    TVec3f _124;
    f32 _130;
    f32 _134;
    f32 _138;
    f32 _13C;
    f32 _140;
    f32 _144;
    f32 _148;
    f32 _14C;
    u32 _150;
    u32 _154;
    u32 _158;
    u32 _15C;
    u32 _160;
    f32 _164;
    u32 _168;
    u32 _16C;
    s32 mColor;                             // _170
    u8 _174;
    u8 _175;
    u8 _176;
    u8 _177;
    u32 _178;
    u8 _17C;
    u8 _17D;
    u8 _17E;
    u8 _17F;
};

namespace MR {
    NameObj* createSuperSpinDriverYellow(const char *);
    NameObj* createSuperSpinDriverGreen(const char *);
    NameObj* createSuperSpinDriverPink(const char *);
};

namespace NrvSuperSpinDriver {
    NERVE_DECL_NULL(SuperSpinDriverNrvTryDemo);
    NERVE_DECL(SuperSpinDriverNrvEmptyNonActive, SuperSpinDriver, SuperSpinDriver::exeEmptyNonActive);
    NERVE_DECL(SuperSpinDriverNrvEmptyAppear, SuperSpinDriver, SuperSpinDriver::exeEmptyAppear);
    NERVE_DECL(SuperSpinDriverNrvEmptyWait, SuperSpinDriver, SuperSpinDriver::exeEmptyWait);
    NERVE_DECL(SuperSpinDriverNrvNonActive, SuperSpinDriver, SuperSpinDriver::exeNonActive);
    NERVE_DECL(SuperSpinDriverNrvAppear, SuperSpinDriver, SuperSpinDriver::exeAppear);
    NERVE_DECL(SuperSpinDriverNrvWait, SuperSpinDriver, SuperSpinDriver::exeWait);
    NERVE_DECL(SuperSpinDriverNrvCapture, SuperSpinDriver, SuperSpinDriver::exeCapture);
    NERVE_DECL(SuperSpinDriverNrvShootStart, SuperSpinDriver, SuperSpinDriver::exeShootStart);
    NERVE_DECL_ONEND(SuperSpinDriverNrvShoot, SuperSpinDriver, SuperSpinDriver::exeShoot, SuperSpinDriver::exeShootOnEnd);
    NERVE_DECL(SuperSpinDriverNrvCoolDown, SuperSpinDriver, SuperSpinDriver::exeCoolDown);
};