#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaSwitchKeeper : public LiveActor {
public:
    KoopaSwitchKeeper(const char*);
    virtual ~KoopaSwitchKeeper();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
};
