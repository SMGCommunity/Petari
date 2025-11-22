#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LavaHomeVolcanoFlow : public LiveActor {
public:
    LavaHomeVolcanoFlow(const char*);

    virtual ~LavaHomeVolcanoFlow();
    virtual void init(const JMapInfoIter& rIter);
    virtual void startClipped();
    virtual void endClipped();

    void exeMove();
    void exeWait();
};
