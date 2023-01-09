#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/NPC/TalkMessageFunc.h"

class LiveActor;
class TalkNodeCtrl;
class ActorCameraInfo;

class TalkMessageCtrl : public NameObj {
public:
    TalkMessageCtrl(LiveActor *, const TVec3f &, MtxPtr);

    virtual ~TalkMessageCtrl();

    void createMessage(const JMapInfoIter &, const char *);
    void createMessageDirect(const JMapInfoIter &, const char *);
    u32 getMessageID() const;
    bool requestTalk();
    bool requestTalkForce();
    void startTalk();
    void startTalkForce();
    void startTalkForcePuppetable();
    void startTalkForceWithoutDemo();
    void startTalkForceWithoutDemoPuppetable();
    void endTalk();
    void updateBalloonPos();
    bool isNearPlayer(const TalkMessageCtrl *);
    bool isNearPlayer(f32) const;
    void rootNodePre(bool);
    void rootNodePst();
    bool isCurrentNodeContinue() const;
    void rootNodeEve();
    void rootNodeSel(bool);
    void registerBranchFunc(const TalkMessageFuncBase &);
    void registerEventFunc(const TalkMessageFuncBase &);
    void registerAnimeFunc(const TalkMessageFuncBase &);
    void registerKillFunc(const TalkMessageFuncBase &);
    void readMessage();
    bool isSelectYesNo() const;

    bool inMessageArea() const;
    void startCamera(s32);
    const char* getBranchID() const;

    LiveActor* mHostActor;              // _C
    TalkNodeCtrl* mNodeCtrl;            // _10
    s32 mZoneID;                        // _14
    u32 _18;
    TVec3f _1C;
    MtxPtr mMtx;                        // _28
    TVec3f _2C;
    f32 mTalkDistance;                  // _38
    u32 _3C;
    u32 mAlreadyDoneFlags;              // _40
    bool mIsOnRootNodeAuto;             // _44
    bool mIsOnReadNodeAuto;             // _45
    bool mIsStartOnlyFront;             // _46
    ActorCameraInfo* mCameraInfo;       // _48
    TalkMessageFuncBase* mBranchFunc;   // _4C
    TalkMessageFuncBase* mEventFunc;    // _50
    TalkMessageFuncBase* mAnimeFunc;    // _54
    TalkMessageFuncBase* mKillFunc;     // _58
    u32 _5C;
    u32 _60;
};

class TalkFunction {
public:
    static bool isShortTalk(const TalkMessageCtrl *);
    static bool isComposeTalk(const TalkMessageCtrl *);
    static bool isSelectTalk(const TalkMessageCtrl *);
    bool isEventNode(const TalkMessageCtrl *);
};