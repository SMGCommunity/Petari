#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class BenefitItemOneUp;

class TripodBossKinokoOneUp : public LiveActor {
public:
    /// @brief Creates a new `TripodBossKinokoOneUp`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossKinokoOneUp(const char* pName);

    virtual void init(const JMapInfoIter&);

    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)&_BC;
    }

    virtual void control();

    void exeActive();
    void exeEnd();

    /* 0x8C */ TPos3f _8C;
    /* 0xBC */ TPos3f _BC;
    /* 0xEC */ BenefitItemOneUp* mOneUp;
    /* 0xF0 */ s32 mJointID;
};
