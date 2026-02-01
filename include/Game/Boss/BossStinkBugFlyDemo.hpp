#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"


class JMapInfoIter;
class DemoPositionController;
class BossStinkBugFlyDemo : public BossStinkBugActionBase {
public:
    BossStinkBugFlyDemo(BossStinkBug* pStinkBug, const JMapInfoIter& rIter);

    virtual void appear();

    void exeDemo();
    void exeTryStart();

    /*0x64 */ DemoPositionController* mDemoPositionController;
};
