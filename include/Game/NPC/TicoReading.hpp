#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TicoReading : public LiveActor {
public:
    TicoReading(const char*);

    virtual ~TicoReading();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();

    void exeWait();
};
