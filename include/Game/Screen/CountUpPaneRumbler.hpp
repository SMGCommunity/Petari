#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class LayoutActor;
class RumbleCalculatorCosMultLinear;

class CountUpPaneRumbler {
public:
    /// @brief Creates a new `CountUpPaneRumbler`.
    /// @param pHost The pointer to the owning actor instance.
    /// @param pPaneName The pointer to the null-terminated name of the pane.
    CountUpPaneRumbler(LayoutActor* pHost, const char* pPaneName);

    void update();
    void start();
    void reset();
    bool isRumbling() const;
    void connectPane(LayoutActor*, const char*);

public:
    /* 0x0 */ RumbleCalculatorCosMultLinear* mRumbleCalculator;
    /* 0x4 */ TVec2f mPosition;
};
