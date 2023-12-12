#pragma once

#include "Game/LiveActor/LiveActor.h"

class PowerStarAppearPoint : public LiveActor {
public:
    PowerStarAppearPoint(const char *);

    virtual ~PowerStarAppearPoint();
    virtual void init(const JMapInfoIter &);

    ActorCameraInfo* mCameraInfo;   // _8C
};