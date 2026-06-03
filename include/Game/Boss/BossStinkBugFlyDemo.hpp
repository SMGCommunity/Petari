#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"

class DemoPositionController;
class JMapInfoIter;

class BossStinkBugFlyDemo : public BossStinkBugActionBase {
public:
    BossStinkBugFlyDemo(BossStinkBug* pHost, const JMapInfoIter& rIter);

    virtual void appear();

    void exeDemo();
    void exeTryStart();

    /* 0x64 */ DemoPositionController* mDemoPositionController;
};
