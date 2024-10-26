#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.hpp"

class LiveActor;

namespace MR {
    void initStarPointerTarget(LiveActor *, f32, const TVec3f &);
    void initStarPointerTargetAtPos(LiveActor *, f32, const TVec3f *, const TVec3f &);
    void initStarPointerTargetAtMtx(LiveActor *, f32, MtxPtr, const TVec3f &);
    void initStarPointerTargetAtJoint(LiveActor *, const char *, f32, const TVec3f &);

    bool isStarPointerPointingPane(const LayoutActor*, const char*, s32, bool, const char*);

    void startStarPointerModePowerStarGetDemo(void *); 

    f32 calcPointRadius2D(const TVec3f &, f32);

    void setStarPointerDrawSyncToken();

    void endStarPointerMode(void *);

    bool isStarPointerPointing2POnPressButton(const LiveActor *, const char *, bool, bool);

    bool isStarPointerPointing2POnTriggerButton(const LiveActor *, const char *, bool, bool);

    f32 getStarPointerScreenSpeed(s32);

    bool calcStarPointerWorldVelocityDirectionOnPlane(TVec3f *, const TVec3f &, const TVec3f &, s32);

    void createStarPointerLayout();

    bool isStarPointerInScreen(s32);

    bool isStarPointerPointing(const LiveActor *, s32, bool, const char *);

    TVec2f *getStarPointerScreenVelocity(s32);

    void getStarPointerWorldVelocityDirection(TVec3f *, s32);

    void calcStarPointerWorldPointingPos(TVec3f *, const TVec3f &, s32);
};
