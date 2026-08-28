#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TicoDomeLecture : public LiveActor {
public:
    TicoDomeLecture(const char*);

    /* 0x08 */ virtual ~TicoDomeLecture();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();

    void exeWait();
    void exeMove();
    void exeMetamorphosis();

    TVec3f _8C;
    TVec3f _98;
};
