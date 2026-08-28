#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TicoReading : public LiveActor {
public:
    TicoReading(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();

    void exeWait();
};
