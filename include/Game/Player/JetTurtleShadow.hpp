#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JetTurtleShadow : public LiveActor {
public:
    JetTurtleShadow(const char*);

    virtual void init(const JMapInfoIter&);

    void calcType0(MtxPtr);
    void drawType0() const;
    void drawType1() const;
};
