#pragma once

#include "Actor/Clipping/ClippingDirector.h"

namespace MR
{
    ClippingDirector* getClippingDirector();
    void addToClippingTarget(LiveActor *);
    void removeFromClippingTarget(LiveActor *);
};