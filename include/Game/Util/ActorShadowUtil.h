#pragma once

#include <revolution.h>

class LiveActor;

namespace MR {
    void initShadowVolumeSphere(LiveActor *, f32);

    void setShadowDropLength(LiveActor *, const char *, f32);

    void onCalcShadowOneTime(LiveActor *, const char *);

    void onShadowVisibleSyncHostAll(LiveActor *);
    void offShadowVisibleSyncHostAll(LiveActor *);
    void onShadowFollowHostScale(LiveActor *, const char *);
    void onShadowFollowHostScaleAll(LiveActor *);
};