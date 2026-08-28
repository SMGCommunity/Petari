#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"

class DemoPositionController;
class JMapInfoIter;

class BossStinkBugAngryDemo : public BossStinkBugActionBase {
public:
    BossStinkBugAngryDemo(BossStinkBug* pHost, const JMapInfoIter& rIter);

    /* 0x10 */ virtual void appear();

    void exeDemo();
    void exeTryStart();

    /* 0x64 */ DemoPositionController* mDemoPositionController;
};
