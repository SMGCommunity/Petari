#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TicoDomeLecture : public LiveActor {
public:
    TicoDomeLecture(const char*);

    virtual ~TicoDomeLecture();
    virtual void init(const JMapInfoIter&);
    virtual void appear();

    void exeWait();
    void exeMove();
    void exeMetamorphosis();

    TVec3f _8C;
    TVec3f _98;
};
