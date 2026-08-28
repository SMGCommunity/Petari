#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"

class DemoPositionController;
class JMapInfoIter;

class BossStinkBugFinishDemo : public BossStinkBugActionBase {
public:
    BossStinkBugFinishDemo(BossStinkBug* pHost, const JMapInfoIter& rIter);

    /* 0x10 */ virtual void appear();

    void exeDemo();
    void exeTryStart();
    void exeAppearPowerStar();

    /* 0x64 */ DemoPositionController* mDemoPositionController;
};
