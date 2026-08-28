#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AstroOverlookObj : public LiveActor {
public:
    AstroOverlookObj(const char*);

    /* 0x08 */ virtual ~AstroOverlookObj();
    /* 0x48 */ virtual void control();

    u8 _8C;
};