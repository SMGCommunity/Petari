#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class MarioParts : public PartsModel {
public:
    MarioParts(LiveActor*, const char*, const char*, bool, MtxPtr, MtxPtr);
    MarioParts(LiveActor*, const char*, const char*, MtxPtr);

    virtual ~MarioParts();
    virtual void init(const JMapInfoIter&);

    /* 0x9C */ const char* _9C;
};
