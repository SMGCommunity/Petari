#ifndef CLIPPINGUTIL_H
#define CLIPPINGUTIL_H

#include "Actor/Clipping/ClippingDirector.h"

namespace MR
{
    ClippingDirector* getClippingDirector();
    void addToClippingTarget(LiveActor *);
    void removeFromClippingTarget(LiveActor *);
};

#endif // CLIPPINGUTIL_H