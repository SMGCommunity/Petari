#pragma once

#include "LiveActor/Clipping/ClippingDirector.h"

namespace MR
{
    ClippingDirector* getClippingDirector();
    void addToClippingTarget(LiveActor *);
    void removeFromClippingTarget(LiveActor *);
};