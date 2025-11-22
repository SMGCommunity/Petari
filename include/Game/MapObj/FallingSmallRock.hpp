#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FallingSmallRock : public LiveActor {
public:
    FallingSmallRock(const char*);

    virtual ~FallingSmallRock();
    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();

    void        exeMove();
    inline void exeWait();
};

namespace NrvFallingSmallRock {
    NERVE_DECL(HostTypeWait, FallingSmallRock, FallingSmallRock::exeWait);
    NERVE_DECL(HostTypeMove, FallingSmallRock, FallingSmallRock::exeMove);
}; // namespace NrvFallingSmallRock