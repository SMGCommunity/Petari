#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaPowerUpSwitch : public LiveActor {
public:
    KoopaPowerUpSwitch(const char* pName);

    virtual ~KoopaPowerUpSwitch();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
};
