#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ModelHolder : public LiveActor {
public:
    ModelHolder(const char *, bool);

    virtual ~ModelHolder();

    void updateMtx(MtxPtr);
};
