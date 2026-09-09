#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class MarioParts : public PartsModel {
public:
    MarioParts(LiveActor*, const char*, const char*, MtxPtr);
    MarioParts(LiveActor*, const char*, const char*, bool, MtxPtr, MtxPtr);

    virtual void init(const JMapInfoIter&);

    /* 0x9C */ const char* _9C;
};
