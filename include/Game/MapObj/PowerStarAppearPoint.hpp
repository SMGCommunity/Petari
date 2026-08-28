#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;

class PowerStarAppearPoint : public LiveActor {
public:
    /// @brief Creates a new `PowerStarAppearPoint`.
    /// @param pName A pointer to the null-terminated name of the object.
    PowerStarAppearPoint(const char* pName);

    /* 0x08 */ virtual ~PowerStarAppearPoint();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    /* 0x8C */ ActorCameraInfo* mCameraInfo;
};
