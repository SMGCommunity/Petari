#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JetTurtleShadow : public LiveActor {
public:
    JetTurtleShadow(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    void calcType0(MtxPtr);
    void drawType0() const;
    void drawType1() const;
};
