#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util.hpp"

class CountUpPaneRumbler {
public:
    CountUpPaneRumbler(LayoutActor *, const char *);

    void update();
    void start();
    void reset();
    bool isRumbling() const;
    void connectPane(LayoutActor *, const char *);

    RumbleCalculatorCosMultLinear* mRumbleCalculator;   // _0
    TVec2f mFollowPos;                                  // _4
};