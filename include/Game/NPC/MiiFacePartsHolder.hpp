#pragma once

#include <revolution.h>
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "JSystem/JKernel/JKRDisposer.hpp"

class MiiFacePartsHolder : public LiveActorGroup, JKRDisposer {
public:
    MiiFacePartsHolder(int);

    u32 _30;
    u32 _34;
};
