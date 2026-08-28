#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class InvisiblePolygonObj : public LiveActor {
public:
    InvisiblePolygonObj(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x38 */ virtual MtxPtr getBaseMtx() const;

    void initBaseMtx();
    void initCollision(const JMapInfoIter&);

    TMtx34f mMatrix;
};
