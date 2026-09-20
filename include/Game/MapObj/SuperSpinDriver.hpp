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
    bool tryBind(HitSensor*, HitSensor*);
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
    bool isNerveEnableBind() const;
    bool isSwingOr2PTrigger() const;
    bool isFarPlayer() const;

    /* 0x8C */ LiveActor* mBindActor;
    /* 0x90 */ SpinDriverShootPath* mShootPath;
    /* 0x94 */ SpinDriverCamera* mSpinDriverCamera;
    /* 0x98 */ SpinDriverOperateRing* mOperateRing;
    /* 0x9C */ SpinDriverPathDrawer* mPathDrawer;
    /* 0xA0 */ ModelObj* mEmptyModel;
    /* 0xA4 */ TQuat4f _A4;
    /* 0xB4 */ TQuat4f _B4;
    /* 0xC4 */ TVec3f _C4;
    /* 0xD0 */ TVec3f _D0;
    /* 0xDC */ TVec3f mShootPathDirection;
    /* 0xE8 */ TVec3f _E8;
    /* 0xF4 */ TVec3f _F4;
    /* 0x100 */ TVec3f _100;
    /* 0x10C */ TVec3f mShootPathPosition;
    /* 0x118 */ TVec3f _118;
    /* 0x124 */ TVec3f _124;
    /* 0x130 */ f32 _130;
    /* 0x134 */ f32 _134;
    /* 0x138 */ f32 _138;
    /* 0x13C */ f32 _13C;
    /* 0x140 */ f32 mFrontAngle;
    /* 0x144 */ f32 _144;
    /* 0x148 */ f32 _148;
    /* 0x14C */ f32 mShadowLength;
    /* 0x150 */ s32 mFlightTime;
    /* 0x154 */ s32 _154;
    /* 0x158 */ s32 _158;
    /* 0x15C */ s32 _15C;
    /* 0x160 */ s32 mDrawPathRangeIdx;
    /* 0x164 */ f32 mPlayerLandRotation;
    /* 0x168 */ u32 _168;
    /* 0x16C */ s32 mAlreadyDoneFlagIdx;
    /* 0x170 */ s32 mColor;
    /* 0x174 */ bool _174;
    // u8 _175;
    // u8 _176;
    // u8 _177;
    /* 0x178 */ s32 _178;
    /* 0x17C */ bool _17C;
    /* 0x17D */ bool _17D;
    /* 0x17E */ bool mIsPullPlayer;
    /* 0x17F */ bool mIsDisableJingle;
};

namespace MR {
    NameObj* createSuperSpinDriverYellow(const char*);
    NameObj* createSuperSpinDriverGreen(const char*);
    NameObj* createSuperSpinDriverPink(const char*);
};  // namespace MR
