#pragma once

#include "Game/Boss/PoltaActionBase.hpp"

class PoltaWaitStart : public PoltaActionBase {
public:
    PoltaWaitStart(Polta*);

    virtual ~PoltaWaitStart();
    virtual void appear();
};
