#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class CameraTargetMtx;
class FixedPosition;
class GhostPacket;
class HitSensor;
class JMapInfoIter;
class JetTurtleShadow;
class NameObjArchiveListCollector;
class RaceManagerLayout;
class XanimePlayer;

f32 getShiftRatio(s8 shiftValue);

namespace MR {
    void convToFloat(TVec3s&, s8, TVec3f*);
    void convToFloat(TVec3Sc&, s8, TVec3f*);
    void convToFloat(s16, s8, f32*);
    void convToFloat(s8, s8, f32*);
};  // namespace MR

class GhostPlayer : public LiveActor {
public:
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    GhostPlayer(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void warpPosition(const char*);
    void exePreStartDemo0();
    bool isRequestSkipDemo() const;
    void exePreStartDemo1();
    void exePreStartDemo2();
    void exeWinDemo();
    void exeLostDemo();
    void initAnimation();
    void setAnimation(const char*);
    void setAnimationWeight(const f32*);
    u32 receiveGhostPacket(GhostPacket*);

    /* 0x08C */ u32 _8C;
    /* 0x090 */ u32 _90;
    /* 0x094 */ ActorCameraInfo* mCameraInfo;
    /* 0x098 */ XanimePlayer* mXanimePlayer;
    /* 0x09C */ XanimePlayer* mXanimePlayerUpper;
    /* 0x0A0 */ Mtx mTargetRotationMtx;
    /* 0x0D0 */ f32 mAnimTrackWeights[4];
    /* 0x0E0 */ TVec3f mStartPos;
    /* 0x0EC */ u32 _EC[3];  // unused
    /* 0x0F8 */ TVec3f mTargetRotation;
    /* 0x104 */ TVec3f mInitialDirection;
    /* 0x110 */ bool mIsHidden;
    /* 0x111 */ bool mWaitingToStart;
    /* 0x112 */ bool _112;
    /* 0x113 */ bool mKilledByStar;
    /* 0x114 */ bool mIsDemoCameraActive;
    /* 0x115 */ bool mHasJetTurtle;
    /* 0x118 */ LiveActor* mPowerStarTarget;
    /* 0x11C */ JetTurtleShadow* mJetTurtleShadow;
    /* 0x120 */ FixedPosition* mHandRPos;
    /* 0x124 */ u16 mAppearStarPieceCooldown;
    /* 0x126 */ u16 mPlayerTrampleCooldown;
    /* 0x128 */ GhostPacket* mCurrentPacket;
    /* 0x12C */ RaceManagerLayout* mRaceManagerLayout;
    /* 0x130 */ CameraTargetMtx* mCameraTargetMtx;
};
