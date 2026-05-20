#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class KoopaRockBreak : public PartsModel {
public:
    KoopaRockBreak(LiveActor*);

    virtual ~KoopaRockBreak();
    virtual void init(const JMapInfoIter&);
    virtual void control();
};
