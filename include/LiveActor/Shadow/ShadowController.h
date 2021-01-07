#pragma once

#include <revolution.h>
#include "LiveActor/LiveActor.h"

class ShadowControllerList
{
public:
    ShadowControllerList(LiveActor *, u32);

    u8 _0[0x10];
};

namespace MR
{
    void requestCalcActorShadow(LiveActor *);
};