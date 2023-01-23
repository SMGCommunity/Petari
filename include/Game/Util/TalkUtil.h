#pragma once

#include "Game/NPC/TalkMessageFunc.h"

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
};