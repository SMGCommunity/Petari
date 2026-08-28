#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"

class JMapInfoIter;
class DemoPositionController;
class BossStinkBugOpeningDemo : public BossStinkBugActionBase {
public:
    BossStinkBugOpeningDemo(BossStinkBug* pStinkBug, const JMapInfoIter& rIter);

    /* 0x10 */ virtual void appear();

    void exeDemo();
    void exeTryStart();

    /*0x64 */ DemoPositionController* mDemoPositionController;
};
