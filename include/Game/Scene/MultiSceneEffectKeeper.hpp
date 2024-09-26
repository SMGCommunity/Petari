#pragma once

#include "Game/LiveActor/ModelManager.hpp"

class MultiEmitter;
class MultiSceneActor;

class MultiSceneEffectKeeper {
public:
    MultiSceneEffectKeeper(const char *,  ModelManager *, int, const char *);

    void init(const MultiSceneActor *, const EffectSystem *);
    void clear();

    MultiEmitter** mEmitters;       // 0x0
    u32 mEmitterNum;                // 0x4
    u32 _8;
    const char* mResName;           // 0xC
};