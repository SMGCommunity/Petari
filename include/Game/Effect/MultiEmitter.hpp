#pragma once

#include <revolution.h>

class EffectSystem;

class MultiEmitter {
public:
    void forceDelete(EffectSystem *);
    void create(EffectSystem *);

    void setHostMtx(MtxPtr);
};
