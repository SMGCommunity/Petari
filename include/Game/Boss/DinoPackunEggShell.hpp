#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DinoPackunEggShell : public LiveActor {
public:
    DinoPackunEggShell(const char*, HitSensor*, MtxPtr);

    virtual ~DinoPackunEggShell();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    inline void exeWait();

    HitSensor* _8C;
    MtxPtr _90;
};
