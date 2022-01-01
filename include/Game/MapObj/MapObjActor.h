#pragma once

#include "Game/LiveActor/LiveActor.h"

class MapObjActor : public LiveActor {
public:
    MapObjActor(const char *);
    MapObjActor(const char *, const char *);

    const char* mObjectName;    // _8C
    
};