#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JAISoundID;

class SoundEmitter : public LiveActor {
public:
    SoundEmitter(const char*);

    virtual ~SoundEmitter();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void exePlaySound();
    void exeStandBy();

    s32 _8C;
};
