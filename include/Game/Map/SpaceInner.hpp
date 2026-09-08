#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpaceInner : public LiveActor {
public:
    /// @brief Creates a new `SpaceInner`.
    /// @param pName A pointer to the null-terminated name of the object.
    SpaceInner(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void appear();

    void exeAppear();
    void exeDisappear();

    void disappear();
    bool isAppeared() const;
};
