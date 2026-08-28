#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MogucchiRefuseTerritory : public LiveActor {
public:
    MogucchiRefuseTerritory(const char*);
    /* 0x08 */ virtual ~MogucchiRefuseTerritory();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};
