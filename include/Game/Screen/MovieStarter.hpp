#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MovieStarter : public LiveActor {
public:
    /// @brief Creates a new `MovieStarter`.
    /// @param pName A pointer to the null-terminated name of the object.
    MovieStarter(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();

public:
    /* 0x8C */ s32 mMovieType;
};
