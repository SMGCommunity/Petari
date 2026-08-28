#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaRestarterVs3 : public LiveActor {
public:
    KoopaRestarterVs3(const char* pName);

    virtual ~KoopaRestarterVs3();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
};
