#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpaceInner : public LiveActor {
public:
    SpaceInner(const char*);

    virtual ~SpaceInner();
    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void appear();

    void disappear();
    bool isAppeared() const;
};

namespace NrvSpaceInner {
    NERVE(HostTypeDisappear);
    NERVE(HostTypeAppear);
};  // namespace NrvSpaceInner