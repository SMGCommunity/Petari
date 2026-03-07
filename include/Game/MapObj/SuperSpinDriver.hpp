#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/MapObj/SpinDriverCamera.hpp"
#include "Game/MapObj/SpinDriverOperateRing.hpp"
#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"

class SuperSpinDriver : public LiveActor {
public:
    SuperSpinDriver(const char*, s32);

    virtual ~SuperSpinDriver();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initParamFromJMapInfo(const JMapInfoIter&);
    void initGravityAxis();
    void initSensor();
    void initEmptyModel();
    void initEventCamera(const JMapInfoIter&);
    void initShootPath(const JMapInfoIter&);
    void initShootPathDraw(const JMapInfoIter&);
    void initColor();
    void initOperateRing();
    void initAppearState(const JMapInfoIter&);
    bool canBind(HitSensor*, HitSensor*) const;
    bool tryBind(HitSensor*, HitSensor*) const;
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

    void exeTryDemo();
    void exeEmptyNonActive();
    void exeEmptyAppear();
    void exeEmptyWait();
    void exeNonActive();
    void exeAppear();
    void exeWait();
    void exeCapture();
    void exeShootStart();
    void exeShoot();
    void exeCoolDown();
    void endShoot();

    void updateShootMotion();
    void cancelBind();
    void endBind();
    void updateBindActorMatrix();
    void updateBindActorPoseToShoot(f32);
    void turnBindHead(const TVec3f&, f32);
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
    SpinDriverShootPath* mShootPath;      // 0x90
    SpinDriverCamera* mSpinDriverCamera;  // 0x94
    SpinDriverOperateRing* mOperateRing;  // 0x98
    SpinDriverPathDrawer* mPathDrawer;    // 0x9C
    ModelObj* mEmptyModel;                // 0xA0
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
    s32 mColor;  // 0x170
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
    NameObj* createSuperSpinDriverYellow(const char*);
    NameObj* createSuperSpinDriverGreen(const char*);
    NameObj* createSuperSpinDriverPink(const char*);
};  // namespace MR
