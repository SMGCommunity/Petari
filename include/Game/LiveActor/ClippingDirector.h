#pragma once

#include <revolution.h>
#include "Game/Util.h"

class LiveActor;

class ClippingDirector {
public:
    void initActorSystemInfo(LiveActor *, const JMapInfoIter &);
};

namespace MR {
    ClippingDirector* getClippingDirector();
};
