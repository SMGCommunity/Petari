#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/NPC/TalkMessageFunc.h"

class TalkNodeCtrl;
class ActorCameraInfo;

class CustomTagArg {
public:
    inline CustomTagArg(int a1, u32 a2) : mIntArg(a1), _4(a2) { }
    inline CustomTagArg(const wchar_t *a1, u32 a2) : mCharArg(a1), _4(a2) { }

    inline void operator=(const CustomTagArg &rhs) {
        mCharArg = rhs.mCharArg;
        _4 = rhs._4;
    }

    union {
        int mIntArg;
        const wchar_t* mCharArg;
    };

    u32 _4;
};


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

    void setMessageArg(const CustomTagArg &rArg) {
        mTagArg = rArg;
    }

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
    CustomTagArg mTagArg;               // _5C
};

class TalkFunction {
public:
    static bool isShortTalk(const TalkMessageCtrl *);
    static bool isComposeTalk(const TalkMessageCtrl *);
    static bool isSelectTalk(const TalkMessageCtrl *);
    bool isEventNode(const TalkMessageCtrl *);

    static bool requestTalkSystem(TalkMessageCtrl *, bool);
    static void startTalkSystem(TalkMessageCtrl *, bool, bool, bool);
    static void endTalkSystem(TalkMessageCtrl *);
    static bool isTalkSystemStart(const TalkMessageCtrl *);
    static bool isTalkSystemEnd(const TalkMessageCtrl *);
    static bool getBranchAstroGalaxyResult(u16);
    static void registerTalkSystem(TalkMessageCtrl *);
};