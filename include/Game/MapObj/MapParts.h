#pragma once

#include "Game/LiveActor/LiveActor.h"

class MapParts : public LiveActor {
public:
    MapParts(const char *);

    TVec3f _8C;
};