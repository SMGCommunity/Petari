#pragma once

#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/RaceManager.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/Util/JointController.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include <JSystem/JGeometry.hpp>

class PlayerPoseSetterInWater;

class Syati : public LiveActor {
public:
    Syati(const char*);
    virtual ~Syati();

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
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
    void initRings(JMapInfoIter const&);
    void initPose();
    void initTalking(JMapInfoIter const&);
    void updateSwimCommon();
    void updatePoseByRail();
    void updateNumRingPassed();
    void updateBlink();
    bool isReadyToEmitRing() const;
    void syncNumRingLeftToActiveRings();
    void resetScore();
    void killAllRings();
    void emitRing();
    void setupBalloonFollowMtx(const TVec3f&);
    bool calcHeadJoint(TPos3f*, const JointControllerInfo&);

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
    /* 0x144 */ s32 mNumRings; // Obj_arg0
    /* 0x148 */ s32 mStarSpawnType;
    /* 0x14C */ s32 mSwimMode;
    /* 0x150 */ s32 mPrizeRingLife; // Obj_arg3
    /* 0x154 */ s32 mPrizeRingCount;
    /* 0x158 */ s32 mCurrentRailPointNo;
};

class PlayerPoseSetterInWater {
public:
    inline PlayerPoseSetterInWater(const TVec3f& rVec, Syati* pSyati);
    void update();

    TVec3f _0;
    TQuat4f _C;
    s32 _1C;
    Vec* _20;
};

namespace NrvSyati {
    NERVE_DECL_EXE(SyatiWait, Syati, Wait);
    NERVE_DECL_EXE(SyatiFadeoutStartEvent, Syati, FadeoutBeforeTalk);
    NERVE_DECL_EXE(SyatiWaitBlankStartEvent, Syati, WaitBlank);
    NERVE_DECL_EXE(SyatiFadeinStartEvent, Syati, FadeinBeforeTalk);
    NERVE_DECL_EXE(SyatiTalkStartMission, Syati, TalkStartMission);
    NERVE_DECL_EXE(SyatiReadyToStart, Syati, ReadyToStart);
    NERVE_DECL_EXE(SyatiCountDown, Syati, CountDown);
    NERVE_DECL_EXE(SyatiSwim, Syati, Swim);
    NERVE_DECL_EXE(SyatiEmitRing, Syati, EmitRing);
    NERVE_DECL_EXE(SyatiWaitStarAppeared, Syati, WaitStarAppeared);
    NERVE_DECL_EXE(SyatiReachToEnd, Syati, ReachToEnd);
    NERVE_DECL_EXE(SyatiWaitAllRingDisappear, Syati, WaitAllRingDisappear);
    NERVE_DECL_EXE(SyatiFadeoutRetryEvent, Syati, FadeoutBeforeTalk);
    NERVE_DECL_EXE(SyatiWaitBlankRetryEvent, Syati, WaitBlank);
    NERVE_DECL_EXE(SyatiFadeinRetryEvent, Syati, FadeinBeforeTalk);
    NERVE_DECL_EXE(SyatiTalkRetryMission, Syati, TalkRetryMission);
    NERVE_DECL_EXE(SyatiForceKill, Syati, ForceKill);
    NERVE_DECL_EXE(SyatiHideOnShore, Syati, HideOnShore);
    NERVE_DECL_EXE(SyatiWaitOnShore, Syati, WaitOnShore);
    NERVE_DECL_EXE(SyatiWaitTalkNormal, Syati, WaitTalkNormal);
    NERVE_DECL_EXE(SyatiTalkNormal, Syati, TalkNormal);
    NERVE_DECL_NULL(SyatiWaitDemoStart);
};  // namespace NrvSyati
