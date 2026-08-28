#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MovieStarter : public LiveActor {
public:
    /// @brief Creates a new `MovieStarter`.
    /// @param pName A pointer to the null-terminated name of the object.
    MovieStarter(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();

private:
    /* 0x8C */ s32 mMovieType;
};
