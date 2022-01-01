#pragma once

#include <revolution.h>

class LiveActor;

namespace MR {
    void initShadowVolumeBox(LiveActor *, const TVec3f &, MtxPtr);

    void setShadowDropLength(LiveActor *, const char *, f32);

    void setShadowVolumeStartDropOffset(LiveActor *, const char *, f32);

    void initShadowVolumeSphere(LiveActor *, f32);
    void onCalcShadowOneTime(LiveActor *, const char *);

    void onShadowVisibleSyncHostAll(LiveActor *);
    void offShadowVisibleSyncHostAll(LiveActor *);
    void onShadowFollowHostScale(LiveActor *, const char *);
    void onShadowFollowHostScaleAll(LiveActor *);
};