#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DinoPackunEggShell : public LiveActor {
public:
    DinoPackunEggShell(const char*, HitSensor*, MtxPtr);

    virtual ~DinoPackunEggShell();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeWait();

    /* 0x8C */ HitSensor* _8C;
    /* 0x90 */ MtxPtr _90;
};
