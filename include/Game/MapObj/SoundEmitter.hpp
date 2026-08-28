#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JAISoundID;

class SoundEmitter : public LiveActor {
public:
    SoundEmitter(const char*);

    /* 0x08 */ virtual ~SoundEmitter();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    void exePlaySound();
    void exeStandBy();

    s32 _8C;
};
