#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/NPCUtil.hpp"

class ButlerExplain : public NPCActor {
public:
    ButlerExplain(const char*);

    virtual ~ButlerExplain();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void startDemo();
    inline void exeDemo();

    bool _D8;
    TalkMessageCtrl* mTalk;
};
