#pragma once

#include "Game/LiveActor/ModelManager.h"

class MultiEmitter;
class MultiSceneActor;

class MultiSceneEffectKeeper {
public:
    MultiSceneEffectKeeper(const char *,  ModelManager *, int, const char *);

    void init(const MultiSceneActor *, const EffectSystem *);
    void clear();

    MultiEmitter** mEmitters;       // _0
    u32 mEmitterNum;                // _4
    u32 _8;
    const char* mResName;           // _C
};