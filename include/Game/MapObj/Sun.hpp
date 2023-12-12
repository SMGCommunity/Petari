#pragma once

#include "Game/LiveActor/LiveActor.h"

class Sun : public LiveActor {
public:
    Sun(const char *);

    virtual ~Sun();
    virtual void init(const JMapInfoIter &);
};