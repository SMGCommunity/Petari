#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Screen/SurfingGuidance.hpp"

class TalkMessageCtrl;

class SurfRayTutorial : public NerveExecutor {
public:
    SurfRayTutorial(LiveActor *, TalkMessageCtrl *, const JMapInfoIter &);

    virtual ~SurfRayTutorial();

    LiveActor* mHostActor;      // 0x8
    u8 _C;
    TVec3f mAcceleration;       // 0x10
    TalkMessageCtrl* mControl;  // 0x1C
    SurfingGuidance* mGuidance; // 0x20
    u32 _24;
    u32 _28;
    u32 _2C;
};
