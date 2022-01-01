#pragma once

#include "Game/LiveActor/LiveActor.h"

class MirrorReflectionModel : public LiveActor {
public:
    MirrorReflectionModel(const LiveActor *, const char *, const char *, MtxPtr);

    const LiveActor* _8C;
    MtxPtr _90;
    u8 _94;
};