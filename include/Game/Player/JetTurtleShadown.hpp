#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Player/J3DModelX.hpp"
class JetTurtleShadow : public LiveActor {
public:
    JetTurtleShadow(const char*);
    virtual ~JetTurtleShadow();
    virtual void init(const JMapInfoIter& rIter);

    void calcType0(MtxPtr);
    void drawType0() const;
    void drawType1() const;
};
