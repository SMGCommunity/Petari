#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "revolution/mtx.h"

class ModelHolder : public LiveActor {
public:
    ModelHolder(const char*, bool);

    void updateMtx(MtxPtr);
};
