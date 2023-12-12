#pragma once

#include "Game/Screen/LayoutActor.h"

class SurfingGuidance : public LayoutActor {
public:
    SurfingGuidance();

    virtual ~SurfingGuidance();
    virtual void init(const JMapInfoIter &);

    void activate();
    void levelOffReady();
    void levelOffHold();
    void levelOffSuccess();
    void turnLeftReady();
    void turnLeftHold();
    void turnLeftSuccess();
    void turnRightReady();
    void turnRightHold();
    void turnRightSuccess();
    void deactivate();
    void exeFadeIn();
    void exeLevelOffReady();
    void exeTurnLeftReady();
    void exeTurnRightReady();
    void exeFadeOut();

    u32 _20;
    f32 _24;
    u32 _28;
    u8 _2C;
};

namespace {
    NERVE(SurfingGuidanceFadeIn);
    NERVE(SurfingGuidanceLevelOffReady);
    NERVE(SurfingGuidanceLevelOffHold);
    NERVE(SurfingGuidanceLevelOffSuccess);
    NERVE(SurfingGuidanceTurnLeftReady);
    NERVE(SurfingGuidanceTurnLeftHold);
    NERVE(SurfingGuidanceTurnLeftSuccess);
    NERVE(SurfingGuidanceTurnRightReady);
    NERVE(SurfingGuidanceTurnRightHold);
    NERVE(SurfingGuidanceTurnRightSuccess);
    NERVE(SurfingGuidanceFadeOut);  
};