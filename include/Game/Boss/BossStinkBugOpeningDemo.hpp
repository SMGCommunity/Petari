#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"


class JMapInfoIter;
class DemoPositionController;
class BossStinkBugOpeningDemo : public BossStinkBugActionBase {
public:
    BossStinkBugOpeningDemo(BossStinkBug* pStinkBug, const JMapInfoIter& rIter);

    virtual void appear();

    void exeDemo();
    void exeTryStart();

    /*_64 */ DemoPositionController* mDemoPositionController;
};
