#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaPowerUpSwitch : public LiveActor {
public:
    KoopaPowerUpSwitch(const char*);
    virtual ~KoopaPowerUpSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
};
