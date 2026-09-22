#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class LiveActorGroup;
class PlayerPoseSetterInWater;
class RaceManagerLayout;
class TalkMessageCtrl;
struct JointControllerInfo;

template < class T >
class JointControlDelegator;

class Syati : public LiveActor {
public:
    Syati(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeFadeoutBeforeTalk();
    void exeWaitBlank();
    void exeFadeinBeforeTalk();
    void exeTalkStartMission();
    void exeReadyToStart();
    void exeCountDown();
    void exeSwim();
    void exeEmitRing();
    void exeWaitStarAppeared();
    void exeReachToEnd();
    void exeWaitAllRingDisappear();
    void exeTalkRetryMission();
    void exeForceKill();
    void exeHideOnShore();
    void exeWaitOnShore();
    void exeWaitTalkNormal();
    void exeTalkNormal();
    void exeStart();
    void initRings(const JMapInfoIter& rIter);
    void initPose();
    void initTalking(const JMapInfoIter& rIter);
    void updateSwimCommon();
    void updatePoseByRail();
    void updateNumRingPassed();
    void updateBlink();
    bool isReadyToEmitRing() const;
    void syncNumRingLeftToActiveRings();
    void resetScore();
    void killAllRings();
    void emitRing();
    void setupBalloonFollowMtx(const TVec3f& rVec);
    bool calcHeadJoint(TPos3f* pPos, const JointControllerInfo& rInfo);

    /* 0x8C */ TQuat4f _8C;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ f32 mClippingRange;
    /* 0xAC */ TVec3f mClippingVec;
    /* 0xB8 */ TalkMessageCtrl* mTalkMessageCtrl;
    /* 0xBC */ ActorCameraInfo* mActorCameraInfo;
    /* 0xC0 */ s32 mHideOnShoreMode;
    /* 0xC4 */ PlayerPoseSetterInWater* mPlayerPoseSetterInWater;
    /* 0xC8 */ s32 mBlinkTimer;
    /* 0xCC */ TVec3f mMarioMoveLocalOffsetVec;
    /* 0xD8 */ TPos3f mBalloonFollowMtx;
    /* 0x108 */ JointControlDelegator< Syati >* mJointControlDelegator;
    /* 0x10C */ TMtx34f _10C;
    /* 0x13C */ RaceManagerLayout* mRaceManagerLayout;
    /* 0x140 */ LiveActorGroup* mPrizeRingGroup;
    /* 0x144 */ s32 mNumRings;
    /* 0x148 */ s32 mStarSpawnType;
    /* 0x14C */ s32 mSwimMode;
    /* 0x150 */ s32 mPrizeRingLife;
    /* 0x154 */ s32 mPrizeRingCount;
    /* 0x158 */ s32 mCurrentRailPointNo;
};

class PlayerPoseSetterInWater {
public:
    inline PlayerPoseSetterInWater(const TVec3f& rVec, Syati* pSyati);
    void update();

    /* 0x00 */ TVec3f _0;
    /* 0x0C */ TQuat4f _C;
    /* 0x1C */ s32 _1C;
    /* 0x20 */ Vec* _20;
};
