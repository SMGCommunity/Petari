#pragma once

#include "Game/LiveActor/LiveActor.h"

class BenefitItemObj : public LiveActor {
public:
    BenefitItemObj(const char *, const char *);

    u8 _8C[0x13C-0x8C];
};