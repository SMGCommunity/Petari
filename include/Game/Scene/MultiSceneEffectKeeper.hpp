#pragma once

#include "Game/Util/Array.hpp"

class EffectSystem;
class ModelManager;
class MultiEmitter;
class MultiSceneActor;

class MultiSceneEffectKeeper {
public:
    MultiSceneEffectKeeper(const char*, ModelManager*, int, const char*);

    void init(const MultiSceneActor*, const EffectSystem*);
    void add(const char*, const TVec3f*, const TVec3f*, const TVec3f*, const char*);
    void add(const char*, MtxPtr, const char*);
    MultiEmitter* create(const char*, EffectSystem*);
    void deleteAll();
    void forceDeleteAll(EffectSystem*);
    void clear();
    MultiEmitter* get(const char*) const;
    MultiEmitter* find(const char*) const;
    void registerEmitter(MultiEmitter*, const char*);

    /* 0x00 */ MR::Vector< MR::AssignableArray< MultiEmitter* > > mEmitter;
    /* 0x0C */ const char* mResName;
};
