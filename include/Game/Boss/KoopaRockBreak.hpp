#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class KoopaRockBreak : public PartsModel {
public:
    KoopaRockBreak(LiveActor* pActor);

    /* 0x08 */ virtual ~KoopaRockBreak();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x48 */ virtual void control();
};
