#pragma once

#include "Game/System/NerveExecutor.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Screen/SurfingGuidance.h"

class TalkMessageCtrl;

class SurfRayTutorial : public NerveExecutor {
public:
    SurfRayTutorial(LiveActor *, TalkMessageCtrl *, const JMapInfoIter &);

    virtual ~SurfRayTutorial();

    LiveActor* mHostActor;      // _8
    u8 _C;
    TVec3f mAcceleration;       // _10
    TalkMessageCtrl* mControl;  // _1C
    SurfingGuidance* mGuidance; // _20
    u32 _24;
    u32 _28;
    u32 _2C;
};
