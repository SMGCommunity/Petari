#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ActorCameraInfo;
class LiveActor;
class TalkMessageFuncBase;
class TalkMessageInfo;
class TalkNodeCtrl;

class CustomTagArg {
public:
    enum TagType { Type_Int = 0, Type_Char = 1, Type_Uninitialized = 2 };

    inline CustomTagArg(int a1, TagType a2) : mIntArg(a1), mArgType(a2) {
    }
    inline CustomTagArg(const wchar_t* a1, TagType a2) : mCharArg(a1), mArgType(a2) {
    }

    inline void operator=(const CustomTagArg& rhs) {
        mCharArg = rhs.mCharArg;
        mArgType = rhs.mArgType;
    }

    union {
        /* 0x0 */ int mIntArg;
        /* 0x0 */ const wchar_t* mCharArg;
    };

    /* 0x4 */ TagType mArgType;
};

class TalkMessageCtrl : public NameObj {
public:
    TalkMessageCtrl(LiveActor*, const TVec3f&, MtxPtr);

    virtual ~TalkMessageCtrl();

    void createMessage(const JMapInfoIter&, const char*);
    void createMessageDirect(const JMapInfoIter&, const char*);
    u32 getMessageID() const;
    bool requestTalk();
    bool requestTalkForce();
    bool startTalk();
    bool startTalkForce();
    bool startTalkForcePuppetable();
    bool startTalkForceWithoutDemo();
    bool startTalkForceWithoutDemoPuppetable();
    bool endTalk();
    void updateBalloonPos();
    bool isNearPlayer(const TalkMessageCtrl*);
    bool isNearPlayer(f32) const;
    void rootNodePre(bool);
    void rootNodePst();
    bool isCurrentNodeContinue() const;
    bool rootNodeEve();
    void rootNodeSel(bool);
    void registerBranchFunc(const TalkMessageFuncBase&);
    void registerEventFunc(const TalkMessageFuncBase&);
    void registerAnimeFunc(const TalkMessageFuncBase&);
    void registerKillFunc(const TalkMessageFuncBase&);
    void readMessage();
    bool isSelectYesNo() const;

    void setMessageArg(const CustomTagArg& rArg) NO_INLINE {
        mTagArg = rArg;
    }

    void setMessageBallonFollowOffs(const TVec3f& rVec) {
        mMsgBalloonFollowOffs = rVec;
    }

    bool inMessageArea() const;
    void startCamera(s32);
    const char* getBranchID() const;

    /* 0x0C */ LiveActor* mHostActor;
    /* 0x10 */ TalkNodeCtrl* mNodeCtrl;
    /* 0x14 */ s32 mZoneID;
    /* 0x18 */ u32 _18;
    /* 0x1C */ TVec3f _1C;
    /* 0x28 */ MtxPtr mMtx;
    /* 0x2C */ TVec3f mMsgBalloonFollowOffs;
    /* 0x38 */ f32 mTalkDistance;
    /* 0x3C */ u32 _3C;
    /* 0x40 */ u32 mAlreadyDoneFlags;
    /* 0x44 */ bool mIsOnRootNodeAuto;
    /* 0x45 */ bool mIsOnReadNodeAuto;
    /* 0x46 */ bool mIsStartOnlyFront;
    /* 0x48 */ ActorCameraInfo* mCameraInfo;
    /* 0x4C */ TalkMessageFuncBase* mBranchFunc;
    /* 0x50 */ TalkMessageFuncBase* mEventFunc;
    /* 0x54 */ TalkMessageFuncBase* mAnimeFunc;
    /* 0x58 */ TalkMessageFuncBase* mKillFunc;
    /* 0x5C */ CustomTagArg mTagArg;
};

class TalkFunction {
public:
    static bool isShortTalk(const TalkMessageCtrl*);
    static bool isComposeTalk(const TalkMessageCtrl*);
    static bool isSelectTalk(const TalkMessageCtrl*);
    static bool isEventNode(const TalkMessageCtrl*);

    static bool requestTalkSystem(TalkMessageCtrl*, bool);
    static bool startTalkSystem(TalkMessageCtrl*, bool, bool, bool);
    static bool endTalkSystem(TalkMessageCtrl*);
    static bool isTalkSystemStart(const TalkMessageCtrl*);
    static bool isTalkSystemEnd(const TalkMessageCtrl*);
    static bool getBranchAstroGalaxyResult(u16);
    static void registerTalkSystem(TalkMessageCtrl*);

    static TalkMessageInfo* getMessageInfo(const TalkMessageCtrl*);
    static const wchar_t* getSubMessage(const TalkMessageCtrl*);
    static const wchar_t* getMessage(const TalkMessageCtrl*);
    static void onTalkStateEntry(TalkMessageCtrl*);
    static void onTalkStateNone(TalkMessageCtrl*);
    static void onTalkStateEnableStart(TalkMessageCtrl*);
    static void onTalkStateTalking(TalkMessageCtrl*);
    static void onTalkStateEnableEnd(TalkMessageCtrl*);
};
