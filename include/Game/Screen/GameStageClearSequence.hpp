#pragma once

#include "Game/Screen/LayoutActor.hpp"

class SimpleEffectLayout;

class GameStageClearSequence : public LayoutActor {
public:
    GameStageClearSequence();

    virtual void init(const JMapInfoIter&);

    void startPowerStarGetDemo();
    void startGrandStarGetDemo();
    void appearGetStarLayout();
    void exePowerStarGetDemo();
    void exeGrandStarGetDemo();

    SimpleEffectLayout* mEffectLayout; // 0x20
    TVec2f              mFollowPos;    // 0x24
};
