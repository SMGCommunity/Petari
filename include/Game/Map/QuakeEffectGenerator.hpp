#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class QuakeEffectGenerator : public LiveActor {
public:
    QuakeEffectGenerator();

    virtual ~QuakeEffectGenerator();
    virtual void init(const JMapInfoIter&);

    void exeWaitInArea();
    void exeQuaking();
    void exeWait();
};
