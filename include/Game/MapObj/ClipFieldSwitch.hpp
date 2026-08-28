#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ClipFieldSwitch : public LiveActor {
public:
    ClipFieldSwitch(const char*);

    /* 0x08 */ virtual ~ClipFieldSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
};