#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Flag : public LiveActor {
    Flag(const char*);

    virtual ~Flag();
    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void draw() const;
    virtual void appear();

    // Members go here...
};