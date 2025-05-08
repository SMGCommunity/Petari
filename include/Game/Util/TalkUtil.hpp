#pragma once

#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry.hpp>

class LiveActor;
class TalkMessageCtrl;

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

    void offRootNodeAutomatic(TalkMessageCtrl *);

    bool tryTalkForce(TalkMessageCtrl *);

    void forwardNode(TalkMessageCtrl *);

    bool isExistNextNode(const TalkMessageCtrl *);

    TalkMessageCtrl* createTalkCtrlDirect(LiveActor *, const JMapInfoIter &, const char *,const TVec3f &, MtxPtr);
};
