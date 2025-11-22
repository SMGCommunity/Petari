#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Sun : public LiveActor {
public:
    Sun(const char*);

    virtual ~Sun();
    virtual void init(const JMapInfoIter&);
};