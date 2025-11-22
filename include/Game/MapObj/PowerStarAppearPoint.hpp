#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PowerStarAppearPoint : public LiveActor {
public:
    PowerStarAppearPoint(const char*);

    virtual ~PowerStarAppearPoint();
    virtual void init(const JMapInfoIter&);

    ActorCameraInfo* mCameraInfo;  // 0x8C
};