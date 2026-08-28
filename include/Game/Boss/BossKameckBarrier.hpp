#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BossKameckBarrier : public LiveActor {
public:
    BossKameckBarrier(const char*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    void exeWait();

    /* 0x8C */ const char* mModelName;
};
