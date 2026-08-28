#pragma once

#include "Game/Boss/PoltaActionBase.hpp"

class PoltaWaitStart : public PoltaActionBase {
public:
    PoltaWaitStart(Polta*);

    /* 0x08 */ virtual ~PoltaWaitStart();
    /* 0x10 */ virtual void appear();
};
