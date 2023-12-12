#pragma once

#include <revolution.h>
#include "Game/LiveActor/LiveActorGroup.h"
#include "JSystem/JKernel/JKRDisposer.h"

class MiiFacePartsHolder : public LiveActorGroup, JKRDisposer {
public:
    MiiFacePartsHolder(int);

    u32 _30;
    u32 _34;
};
