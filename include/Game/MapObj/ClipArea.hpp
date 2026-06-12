#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ClipAreaShape;

class ClipArea : public LiveActor {
public:
    ClipArea(const char*);

    virtual ~ClipArea();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual MtxPtr getBaseMtx() const;
    virtual bool isInArea(const TVec3f&, f32) const;

    void initBaseMatrix(const JMapInfoIter&);
    void setShape(ClipAreaShape*);

    /* 0x8C */ TPos3f mBaseMatrix;
    /* 0xBC */ ClipAreaShape* mShape;
};
