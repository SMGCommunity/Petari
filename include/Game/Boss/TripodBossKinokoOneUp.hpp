#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class BenefitItemOneUp;

class TripodBossKinokoOneUp : public LiveActor {
public:
    TripodBossKinokoOneUp(const char*);

    /* 0x08 */ virtual ~TripodBossKinokoOneUp();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x38 */ virtual MtxPtr getBaseMtx() const;
    /* 0x48 */ virtual void control();

    void exeActive();
    void exeEnd();

    TPos3f _8C;
    TPos3f _BC;
    BenefitItemOneUp* mOneUp;  // 0xEC
    s32 _F0;
};
