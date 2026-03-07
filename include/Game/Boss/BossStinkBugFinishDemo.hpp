#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"


class JMapInfoIter;
class DemoPositionController;
class BossStinkBugFinishDemo : public BossStinkBugActionBase {
public:
    BossStinkBugFinishDemo(BossStinkBug* pStinkBug, const JMapInfoIter& rIter);

    virtual void appear();

    void exeDemo();
    void exeTryStart();
    void exeAppearPowerStar();

    /*0x64 */ DemoPositionController* mDemoPositionController;
};
