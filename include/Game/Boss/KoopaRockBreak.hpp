#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class KoopaRockBreak : public PartsModel {
public:
    KoopaRockBreak(LiveActor* pActor);

    virtual ~KoopaRockBreak();
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
};
