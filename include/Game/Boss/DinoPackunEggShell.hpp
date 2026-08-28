#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DinoPackunEggShell : public LiveActor {
public:
    DinoPackunEggShell(const char*, HitSensor*, MtxPtr);

    /* 0x08 */ virtual ~DinoPackunEggShell();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void exeWait();

    /* 0x8C */ HitSensor* _8C;
    /* 0x90 */ MtxPtr _90;
};
