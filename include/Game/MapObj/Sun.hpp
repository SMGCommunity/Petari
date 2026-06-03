#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Sun : public LiveActor {
public:
    /// @brief Creates a new `Sun`.
    /// @param pName A pointer to the null-terminated name of the object.
    Sun(const char* pName);

    virtual ~Sun();
    virtual void init(const JMapInfoIter& rIter);
};
