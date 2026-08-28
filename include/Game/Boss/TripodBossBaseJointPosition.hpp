#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class TripodBossBaseJointPosition : public LiveActor {
public:
    TripodBossBaseJointPosition(const char*);

    /* 0x08 */ virtual ~TripodBossBaseJointPosition();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x38 */ virtual MtxPtr getBaseMtx() const;
    /* 0x48 */ virtual void control();

    s32 _8C;
    TPos3f _90;
};
