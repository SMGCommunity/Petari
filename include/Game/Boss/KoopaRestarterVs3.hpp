#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaRestarterVs3 : public LiveActor {
public:
    KoopaRestarterVs3(const char*);

    virtual ~KoopaRestarterVs3();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
};
