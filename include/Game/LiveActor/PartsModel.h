#pragma once

#include "Game/LiveActor/LiveActor.h"

class PartsModel : public LiveActor {
public:
    PartsModel(LiveActor *, const char *, const char *, MtxPtr, int, bool);

    LiveActor* mHost;   // _8C
    u32 _90;
    MtxPtr mMtx;        // _94
    u8 _98;
    u8 _99;
    u8 _9A;
};