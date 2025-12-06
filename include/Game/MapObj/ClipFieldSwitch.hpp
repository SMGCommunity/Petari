#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ClipFieldSwitch : public LiveActor {
public:
    ClipFieldSwitch(const char*);

    virtual ~ClipFieldSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void control();
};