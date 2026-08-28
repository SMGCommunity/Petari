#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ClipAreaShape;

class ClipArea : public LiveActor {
public:
    ClipArea(const char*);

    /* 0x08 */ virtual ~ClipArea();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;
    /* 0x38 */ virtual MtxPtr getBaseMtx() const;
    /* 0x78 */ virtual bool isInArea(const TVec3f&, f32) const;

    void initBaseMatrix(const JMapInfoIter&);
    void setShape(ClipAreaShape*);

    /* 0x8C */ TPos3f mBaseMatrix;
    /* 0xBC */ ClipAreaShape* mShape;
};
