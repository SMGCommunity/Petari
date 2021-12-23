#pragma once

#include "Game/LiveActor/LiveActor.h"

class ModelObj : public LiveActor {
public:
    ModelObj(const char *, const char *, MtxPtr, int, int, int, bool);

    virtual ~ModelObj();
    virtual void init(const JMapInfoIter &);

    virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;    // _8C
};