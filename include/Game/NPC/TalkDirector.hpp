#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include <revolution.h>

class ActorCameraInfo;
class TalkMessageCtrl;
class TalkBalloonHolder;
class TalkStateHolder;
class TalkPeekZ;
class TalkState;

class TalkDirector : public LayoutActor {
public:
    TalkDirector(const char*);

    virtual ~TalkDirector();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    bool request(TalkMessageCtrl*, bool);
    bool test(TalkMessageCtrl*, bool, bool);
    bool start(TalkMessageCtrl*, bool, bool, bool);
    void updateMessage();
    void prepTalk(TalkMessageCtrl*, bool, bool, bool);
    void termTalk();
    TalkState* initState(TalkMessageCtrl*);
    bool isInvalidTalk() const;
    void appearYesNoSelector(const TalkMessageCtrl*) const;
    s32 getDemoType(const TalkMessageCtrl*, bool) const;
    bool getBranchResult(u16);
    void initBranchResult();
    void pauseOff();
    void balloonOff();
    bool isSystemTalking() const;
    bool isNormalTalking() const;
    LiveActor* getTalkingActor() const;
    void exePrep();
    void exeTalk();
    void exeSlct();
    void exeNext();
    void exeTerm();
    void exeWait();

    MR::Vector< MR::AssignableArray< TalkMessageCtrl* > > mMsgControls;  // 0x20
    TalkPeekZ* mPeekZ;                                                   // 0x2C
    TalkBalloonHolder* mBalloonHolder;                                   // 0x30
    TalkStateHolder* mStateHolder;                                       // 0x34
    TalkMessageCtrl* mMsgCtrl;                                           // 0x38
    TalkMessageCtrl* _3C;
    TalkMessageCtrl* _40;
    TalkMessageCtrl* _44;
    TalkState* mTalkState;  // 0x48
    bool _4C;
    bool _4D;
    bool _4E;
    bool mIsInvalidClipping;  // 0x4F
    bool _50;
    s32 mDemoType;  // 0x54
    bool _58;
    bool _59;
    bool _5A;
    bool _5B;
    TalkMessageInfo mMessageInfo;               // 0x5C
    ActorCameraInfo* mCameraInfo;               // 0x68
    LiveActor* mHostActor;                      // 0x6C
    bool mIsKinopioExplorerRescued;             // 0x70
    bool mIsKinopioExplorerOrganize;            // 0x71
    bool mIsKinopioExplorerTalkGetGrandStar2;   // 0x72
    bool mIsKinopioExplorerTrickComet;          // 0x73
    bool mIsKinopioExplorerTalkGetGrandStar3;   // 0x74
    bool mIsKinopioExplorerStartMessenger;      // 0x75
    bool mIsKinopioExplorerTalkGetGrandStar4;   // 0x76
    bool mIsKinopioExplorerTalkGetGrandStar5;   // 0x77
    bool mIsKinopioExplorerTalkGetGrandStar6;   // 0x78
    bool mIsKinopioExplorerTalkGoFinalBattle;   // 0x79
    bool mIsEndLuigiHideAndSeek;                // 0x7A
    bool mIsKinopioExplorerCompleteTrickComet;  // 0x7B
    bool _7C;
    bool _7D;
    bool mHasOneGreenStar;      // 0x7E
    bool mHasTwoGreenStars;     // 0x7F
    bool mHasThreeGreenStars;   // 0x80
    bool mIsUnlockedRedDriver;  // 0x81
    bool _82;
    bool mIsActiveLuigiHideAndSeek;           // 0x83
    bool mIsGalaxyPurpleCometLaunch;          // 0x84
    bool misRosettaTalkTorchLecture;          // 0x85
    bool mIsRosettaTalkTrickComet;            // 0x86
    bool mIsRosettaTalkKoopa;                 // 0x87
    bool mIsRosettaTalkCountDownStart;        // 0x88
    bool mIsRosettaTalkAstroDemoRecover;      // 0x89
    bool mIsRosettaTalkTorchProgress;         // 0x8A
    bool mIsOnGameEventFlagViewNormalEnding;  // 0x8B
};

class TalkPeekZ {
public:
    TalkPeekZ();
    void setDrawSyncToken();
    void drawSyncCallback(u16);
};

namespace MR {
    void pauseOffTalkDirector();
    void balloonOffTalkDirector();
    void invalidateTalkDirector();
    void setTalkDirectorDrawSyncToken();
    bool isActiveTalkBalloonShort();
    void endNPCTalkCamera(bool, s32);
};  // namespace MR
