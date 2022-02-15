#pragma once

#include <revolution.h>

class LiveActor;

namespace MR {
    bool isExistEffectKeeper(const LiveActor *);

    bool isRegisteredEffect(const LiveActor *, const char *);

    bool isEffectValid(const LiveActor *, const char *);
    void deleteEffect(LiveActor *, const char *);

    void emitEffect(LiveActor *, const char *);

    void onDrawEffect(LiveActor *);
    void offDrawEffect(LiveActor *);
    void forceDeleteEffect(LiveActor *, const char *);
    void forceDeleteEffectAll(LiveActor *);

    void setEffectHostMtx(LiveActor *, const char *, MtxPtr);
};