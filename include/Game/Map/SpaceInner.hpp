#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpaceInner : public LiveActor {
public:
    SpaceInner(const char*);

    /* 0x08 */ virtual ~SpaceInner();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();

    void exeAppear();
    void exeDisappear();

    void disappear();
    bool isAppeared() const;
};
