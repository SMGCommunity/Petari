#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class NPCSupportRail : public LiveActor {
public:
    /// @brief Creates a new `NPCSupportRail`.
    /// @param pName A pointer to the null-terminated name of the object.
    NPCSupportRail(const char* pName);

    virtual void init(const JMapInfoIter&);
};
