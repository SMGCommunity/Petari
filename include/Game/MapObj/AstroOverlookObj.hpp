#pragma once

#include "Game/LiveActor/LiveActor.h"

class AstroOverlookObj : public LiveActor {
public:
    AstroOverlookObj(const char *);

    virtual ~AstroOverlookObj();
    virtual void control();

    u8 _8C;
};