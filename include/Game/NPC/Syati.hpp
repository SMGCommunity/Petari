#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include <JSystem/JGeometry.hpp>
#include "Game/Util/JointController.hpp"
#include "Game/Screen/RaceManagerLayout.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/MapObj/PrizeRing.hpp"

class PlayerPoseSetterInWater : public TVec3f {
public:
    inline PlayerPoseSetterInWater();
    void update();

    TQuat4f _C;
    s32 _1C;
    Vec* _20;
};
class Syati : public LiveActor {
public:
    Syati(const char*);
    virtual ~Syati();
    
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *,HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32 ,HitSensor *,HitSensor *);
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
    void exeHideOnShore();
    void exeWaitOnShore();
    void exeWaitTalkNormal();
    void exeTalkNormal();
    inline void exeKill();
    void initRings(JMapInfoIter const &);
    void initPose();
    void initTalking(JMapInfoIter const &);
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
    bool calcHeadJoint(TPos3f *,const JointControllerInfo &);

    private:
    TQuat4f _8C;
    TVec3f _9C;
    f32 _A8; // some radius
    TVec3f _AC;
    TalkMessageCtrl* _B8;
    ActorCameraInfo* _BC;
    s32 _C0;
    PlayerPoseSetterInWater* _C4;
    s32 _C8;
    TVec3f _CC;
    TPos3f _D8;
    JointControlDelegator<Syati>* _108;
    TMtx34f _10C;
    RaceManagerLayout* _13C;
    LiveActorGroup* _140;
    s32 _144;
    s32 _148;
    s32 _14C; //Obj_arg2
    s32 _150;
    s32 _154;
    s32 _158;

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
    NERVE_DECL_EXE(SyatiForceKill, Syati, Kill);
    NERVE_DECL_EXE(SyatiHideOnShore, Syati, HideOnShore);
    NERVE_DECL_EXE(SyatiWaitOnShore, Syati, WaitOnShore);
    NERVE_DECL_EXE(SyatiWaitTalkNormal, Syati, WaitTalkNormal);
    NERVE_DECL_EXE(SyatiTalkNormal, Syati, TalkNormal);
    NERVE_DECL_NULL(SyatiWaitDemoStart);
};
