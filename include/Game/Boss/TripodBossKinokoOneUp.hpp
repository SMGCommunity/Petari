#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BenefitItemOneUp;

class TripodBossKinokoOneUp : public LiveActor {
public:
    TripodBossKinokoOneUp(const char*);

    virtual ~TripodBossKinokoOneUp();
    virtual void init(const JMapInfoIter&);
    virtual MtxPtr getBaseMtx() const;
    virtual void control();

    void exeActive();
    void exeEnd();

    TPos3f _8C;
    TPos3f _BC;
    BenefitItemOneUp* mOneUp;  // 0xEC
    s32 _F0;
};
