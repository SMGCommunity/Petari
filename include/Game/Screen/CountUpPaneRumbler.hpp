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

    RumbleCalculatorCosMultLinear* mRumbleCalculator;   // 0x0
    TVec2f mFollowPos;                                  // 0x4
};