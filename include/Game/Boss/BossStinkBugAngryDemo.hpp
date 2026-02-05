#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"


class JMapInfoIter;
class DemoPositionController;
class BossStinkBugAngryDemo : public BossStinkBugActionBase {
public:
    BossStinkBugAngryDemo(BossStinkBug* pStinkBug, const JMapInfoIter& rIter);

    virtual void appear();

    void exeDemo();
    void exeTryStart();

    /*0x64 */ DemoPositionController* mDemoPositionController;
};
