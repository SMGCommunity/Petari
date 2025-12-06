#pragma once

#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/Util/MessageUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class JMapInfoIter;
class LiveActor;
class TalkMessageCtrl;
class TalkMessageFuncBase;

namespace MR {
    class ActorTalkParam {
    public:
        void setNoTurnAction(const char*);
        void setSingleAction(const char*);

        u8 _0;
        u8 _1;
        u8 _2;
        u8 _3;
        f32 _4;
        f32 _8;
        f32 _C;
        f32 _10;
        const char* _14;
        const char* _18;
        const char* _1C;
        const char* _20;
    };

    void registerBranchFunc(TalkMessageCtrl*, const TalkMessageFuncBase&);
    void registerEventFunc(TalkMessageCtrl*, const TalkMessageFuncBase&);
    void registerAnimeFunc(TalkMessageCtrl*, const TalkMessageFuncBase&);
    void registerKillFunc(TalkMessageCtrl*, const TalkMessageFuncBase&);
    void setMessageArg(TalkMessageCtrl*, int);
    void setMessageArg(TalkMessageCtrl*, const wchar_t*);
    TalkMessageCtrl* createTalkCtrl(LiveActor*, const JMapInfoIter&, const char*, const TVec3f&, MtxPtr);
    TalkMessageCtrl* createTalkCtrlDirect(LiveActor*, const JMapInfoIter&, const char*, const TVec3f&, MtxPtr);
    TalkMessageCtrl* createTalkCtrlDirectOnRootNodeAutomatic(LiveActor*, const JMapInfoIter&, const char*, const TVec3f&, MtxPtr);
    bool tryTalkNearPlayer(TalkMessageCtrl*);
    bool tryTalkNearPlayerAtEnd(TalkMessageCtrl*);
    bool tryTalkForce(TalkMessageCtrl*);
    bool tryTalkForceAtEnd(TalkMessageCtrl*);
    bool tryTalkForceWithoutDemo(TalkMessageCtrl*);
    bool tryTalkForceWithoutDemoMarioPuppetable(TalkMessageCtrl*);
    bool tryTalkForceWithoutDemoAtEnd(TalkMessageCtrl*);
    bool tryTalkForceWithoutDemoMarioPuppetableAtEnd(TalkMessageCtrl*);
    bool tryTalkTimeKeepDemo(TalkMessageCtrl*);
    bool tryTalkTimeKeepDemoMarioPuppetable(TalkMessageCtrl*);
    bool tryTalkTimeKeepDemoWithoutPauseMarioPuppetable(TalkMessageCtrl*);
    bool tryTalkRequest(TalkMessageCtrl*);
    bool tryTalkSelectLeft(TalkMessageCtrl*);
    bool tryTalkSelectRight(TalkMessageCtrl*);
    const MtxPtr getMessageBalloonFollowMatrix(const TalkMessageCtrl*);
    const TVec3f& getMessageBalloonFollowOffset(const TalkMessageCtrl*);
    void setMessageBalloonFollowOffset(TalkMessageCtrl*, const TVec3f&);
    bool isNearPlayer(const TalkMessageCtrl*, f32);
    bool inMessageArea(const TalkMessageCtrl*);
    bool isTalkNone(const TalkMessageCtrl*);
    bool isTalkEntry(const TalkMessageCtrl*);
    bool isTalkTalking(const TalkMessageCtrl*);
    bool isTalkEnableEnd(const TalkMessageCtrl*);
    void clearTalkState(TalkMessageCtrl*);
    void resetNode(TalkMessageCtrl*);
    void readMessage(TalkMessageCtrl*);
    void forwardNode(TalkMessageCtrl*);
    void resetAndForwardNode(TalkMessageCtrl*, s32);
    void forwardNodeNextBranchLeft(TalkMessageCtrl*);
    void forwardNodeNextBranchRight(TalkMessageCtrl*);
    void forwardNodeCurrentBranchLeft(TalkMessageCtrl*);
    void forwardNodeCurrentBranchRight(TalkMessageCtrl*);
    void tryForwardNode(TalkMessageCtrl*);
    bool isExistNextNode(const TalkMessageCtrl*);
    bool isShortTalk(const TalkMessageCtrl*);
    void setDistanceToTalk(TalkMessageCtrl*, f32);
    void onRootNodeAutomatic(TalkMessageCtrl*);
    void offRootNodeAutomatic(TalkMessageCtrl*);
    void onReadNodeAutomatic(TalkMessageCtrl*);
    void offReadNodeAutomatic(TalkMessageCtrl*);
    void onStartOnlyFront(TalkMessageCtrl*);
    bool isTalkStart(const TalkMessageCtrl*);
    bool isTalkEnd(const TalkMessageCtrl*);

    inline void setMessageArgToCurrentGalaxyName(TalkMessageCtrl* pCtrl, const char* pName) {
        MR::setMessageArg(pCtrl, MR::getGalaxyNameOnCurrentLanguage(pName));
    }
};  // namespace MR
