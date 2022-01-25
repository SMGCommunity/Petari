#pragma once

class TalkMessageCtrl;

namespace MR {
    void resetAndForwardNode(TalkMessageCtrl *, s32);

    bool tryTalkForceWithoutDemoMarioPuppetableAtEnd(TalkMessageCtrl *);
};