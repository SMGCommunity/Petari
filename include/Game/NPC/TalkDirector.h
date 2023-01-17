#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/NPC/TalkMessageInfo.h"
#include "Game/NPC/TalkMessageCtrl.h"
#include "Game/Screen/LayoutActor.h"
#include <revolution.h>

class ActorCameraInfo;
class TalkMessageCtrl;
class TalkBalloonHolder;
class TalkStateHolder;
class TalkPeekZ;
class TalkState;

class TalkDirector : public LayoutActor {
public:

    virtual ~TalkDirector();
    virtual void init(const JMapInfoIter &);
    virtual void movement();

    bool request(TalkMessageCtrl *, bool);
    bool test(TalkMessageCtrl *, bool, bool);
    bool start(TalkMessageCtrl *, bool, bool, bool);
    void updateMessage();
    void prepTalk(TalkMessageCtrl *, bool, bool, bool);
    void termTalk();
    void initState(TalkMessageCtrl *);
    bool isInvalidTalk() const;
    void appearYesNoSelector(const TalkMessageCtrl *) const;
    u32 getDemoType(const TalkMessageCtrl *, bool) const;
    u8 getBranchResult(u16);
    void initBranchResult();
    void pauseOff();
    void balloonOff();
    bool isSystemTalking() const;
    bool isNormalTalking();
    LiveActor* getTalkingActor() const;
    void exePrep();
    void exeTalk();
    void exeSlct();
    void exeNext();
    void exeTerm();

    MR::AssignableArray<TalkMessageCtrl *> mMsgControls;     // _20
    TalkPeekZ* mPeekZ;                                      // _2C
    TalkBalloonHolder* mBalloonHolder;                      // _30
    TalkStateHolder* mStateHolder;                          // _34
    TalkMessageCtrl* mMsgCtrl;                              // _38
    u32 _3C;
    u32 _40;
    bool _44;
    bool _45;
    bool _46;
    bool _47;
    TalkState* mTalkState;                                  // _48
    bool _4C;
    bool _4D;
    bool _4E;
    bool mIsInvalidClipping;                                // _4F
    bool _50;
    u32 mDemoType;                                          // _54
    bool _58;
    bool _59;
    bool _5A;
    bool _5B;
    TalkMessageInfo mMessageInfo;                           // _5C
    ActorCameraInfo* mCameraInfo;                           // _68
    LiveActor* mHostActor;                                  // _6C
    bool mIsKinopioExplorerRescued;                         // _70
    bool mIsKinopioExplorerOrganize;                        // _71
    bool mIsKinopioExplorerTalkGetGrandStar2;               // _72
    bool mIsKinopioExplorerTrickComet;                      // _73
    bool mIsKinopioExplorerTalkGetGrandStar3;               // _74
    bool mIsKinopioExplorerStartMessenger;                  // _75
    bool mIsKinopioExplorerTalkGetGrandStar4;               // _76
    bool mIsKinopioExplorerTalkGetGrandStar5;               // _77
    bool mIsKinopioExplorerTalkGetGrandStar6;               // _78
    bool mIsKinopioExplorerTalkGoFinalBattle;               // _79
    bool mIsEndLuigiHideAndSeek;                            // _7A
    bool mIsKinopioExplorerCompleteTrickComet;              // _7B
    bool _7C;
    bool _7D;
    bool mHasOneGreenStar;                                  // _7E
    bool mHasTwoGreenStars;                                 // _7F
    bool mHasThreeGreenStars;                               // _80
    bool mIsUnlockedRedDriver;                              // _81
    bool _82;
    bool mIsActiveLuigiHideAndSeek;                         // _83
    bool mIsGalaxyPurpleCometLaunch;                        // _84
    bool misRosettaTalkTorchLecture;                        // _85
    bool mIsRosettaTalkTrickComet;                          // _86
    bool mIsRosettaTalkKoopa;                               // _87
    bool mIsRosettaTalkCountDownStart;                      // _88
    bool mIsRosettaTalkAstroDemoRecover;                    // _89
    bool mIsRosettaTalkTorchProgress;                       // _8A
    bool mIsOnGameEventFlagViewNormalEnding;                // _8B
};


namespace MR {
    bool isActiveTalkBalloonShort();
};