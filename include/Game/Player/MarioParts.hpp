#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class MarioParts : public PartsModel {
public:
    MarioParts(LiveActor*, const char*, const char*, MtxPtr);
    MarioParts(LiveActor*, const char*, const char*, bool, MtxPtr, MtxPtr);

    virtual ~MarioParts();
    virtual void init(const JMapInfoIter&) override;
};
