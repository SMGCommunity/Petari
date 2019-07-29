#ifndef ACTORMOVEMENTUTIL_H
#define ACTORMOVEMENTUTIL_H

#include "types.h"

class LiveActor;

namespace MR
{
    void resetPosition(LiveActor *);
    void makeMtxTransRotateY(Mtx, const LiveActor *);
    void makeMtxTR(Mtx, const LiveActor *);
};

#endif // ACTORMOVEMENTUTIL_H