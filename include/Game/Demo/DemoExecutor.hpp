#pragma once

#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/Demo/DemoTimeKeeper.hpp"
#include "Game/Demo/DemoSubPartKeeper.hpp"
#include "Game/Demo/DemoActionKeeper.hpp"

class DemoActionKeeper;

class DemoExecutor : public DemoCastGroup {
public:
    DemoExecutor(const char *);

    bool tryStartProperDemoSystem();
    bool tryStartDemoSystemPart(const char *, s32);
    bool tryStartProperDemoSystemPart(const char *);

    void pause();
    void resume();

    void addTalkMessageCtrl(LiveActor *, TalkMessageCtrl *);
    void setTalkMessageCtrl(const LiveActor *, TalkMessageCtrl *);

    const char *mSheetName;            // _14
    DemoTimeKeeper *mTimeKeeper;       // _18
    DemoSubPartKeeper *mSubPartKeeper; // _1C
    void *_20;
    void *_24;
    DemoActionKeeper *mActionKeeper;   // _28
};
