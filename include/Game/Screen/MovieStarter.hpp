#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MovieStarter : public LiveActor {
public:
    MovieStarter(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void kill();
    virtual void control();

    /* 0x8C */ s32 mMovieIdx;
};
