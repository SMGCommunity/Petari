#pragma once

#include "Game/Screen/LayoutActor.hpp"

class SimpleEffectLayout;

class GameStageClearSequence : public LayoutActor {
public:
    /// @brief Creates a new `GameStageClearSequence`.
    GameStageClearSequence();

    virtual void init(const JMapInfoIter& rIter);

    void startPowerStarGetDemo();
    void startGrandStarGetDemo();
    void appearGetStarLayout();
    void exePowerStarGetDemo();
    void exeGrandStarGetDemo();

    /* 0x20 */ SimpleEffectLayout* mEffectLayout;
    /* 0x24 */ TVec2f mEffectFollowPos;
};
