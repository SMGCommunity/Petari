#pragma once

#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"
#include <JSystem/JGeometry.hpp>

class TalkMessageCtrl;
class LiveActor;

namespace MR {

    void registerBranchFunc(TalkMessageCtrl *, const TalkMessageFuncBase &);
    void registerEventFunc(TalkMessageCtrl *, const TalkMessageFuncBase &);
    void registerAnimeFunc(TalkMessageCtrl *, const TalkMessageFuncBase &);
    void registerKillFunc(TalkMessageCtrl *, const TalkMessageFuncBase &);
    void setMessageArg(TalkMessageCtrl *, int);
    void setMessageArg(TalkMessageCtrl *, const wchar_t *);

    void resetAndForwardNode(TalkMessageCtrl *, s32);

    bool tryTalkForceWithoutDemoMarioPuppetableAtEnd(TalkMessageCtrl *);

    void setDistanceToTalk(TalkMessageCtrl *, f32);

    void onRootNodeAutomatic(TalkMessageCtrl *);
    void offRootNodeAutomatic(TalkMessageCtrl *);

    void resetNode(TalkMessageCtrl *);

    bool tryTalkForce(TalkMessageCtrl *);

    void forwardNode(TalkMessageCtrl *);

    void onStartOnlyFront(TalkMessageCtrl *);

    bool isExistNextNode(const TalkMessageCtrl *);

    bool tryTalkRequest(TalkMessageCtrl *);
    bool tryTalkSelectLeft(TalkMessageCtrl *);
    bool tryTalkSelectRight(TalkMessageCtrl *);

    void setMessageBalloonFollowOffset(TalkMessageCtrl *, const TVec3f &);

    TalkMessageCtrl* createTalkCtrlDirect(LiveActor *, const JMapInfoIter &, const char *,const TVec3f &, MtxPtr);
    TalkMessageCtrl* createTalkCtrlDirectOnRootNodeAutomatic(LiveActor *, const JMapInfoIter &, const char *, const TVec3f &, MtxPtr);

    inline void setMessageArgToCurrentGalaxyName(TalkMessageCtrl *pCtrl, const char *pName) {
        MR::setMessageArg(pCtrl, MR::getGalaxyNameOnCurrentLanguage(pName));
    }

    bool isNearPlayer(const TalkMessageCtrl *, f32);

    bool tryTalkNearPlayer(TalkMessageCtrl *);

    bool tryTalkForceWithoutDemo(TalkMessageCtrl *);
    bool tryTalkForceWithoutDemoAtEnd(TalkMessageCtrl *);
};
