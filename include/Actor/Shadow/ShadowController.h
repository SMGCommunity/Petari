#ifndef SHADOWCONTROLLER_H
#define SHADOWCONTROLLER_H

#include <revolution.h>
#include "Actor/LiveActor/LiveActor.h"

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

#endif // SHADOWCONTROLLER_H