#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaViewSwitchKeeper : public LiveActor {
public:
    KoopaViewSwitchKeeper(const char* pName);

    /* 0x08 */ virtual ~KoopaViewSwitchKeeper();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
};
