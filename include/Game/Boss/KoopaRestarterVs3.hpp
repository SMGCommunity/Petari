#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaRestarterVs3 : public LiveActor {
public:
    KoopaRestarterVs3(const char* pName);

    virtual ~KoopaRestarterVs3();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
};
