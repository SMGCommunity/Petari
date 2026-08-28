#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaPowerUpSwitch : public LiveActor {
public:
    KoopaPowerUpSwitch(const char* pName);

    /* 0x08 */ virtual ~KoopaPowerUpSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x10 */ virtual void initAfterPlacement();
};
