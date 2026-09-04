#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class TripodBossBaseJointPosition : public LiveActor {
public:
    /// @brief Creates a new `TripodBossBaseJointPosition`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossBaseJointPosition(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();

    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)&mMtx;
    }

    virtual void control();

    /* 0x8C */ s32 mID;
    /* 0x90 */ TPos3f mMtx;
};
