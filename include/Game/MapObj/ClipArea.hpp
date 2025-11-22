#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class ClipAreaShape;

class ClipArea : public LiveActor {
public:
    ClipArea(const char*);
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual MtxPtr getBaseMtx() const;
    virtual ~ClipArea();

    void initBaseMatrix(const JMapInfoIter&);
    void setShape(ClipAreaShape*);
    bool isInArea(const TVec3f&, f32) const;

    TPos3f mBaseMatrix;     // 0x8C
    ClipAreaShape* mShape;  // 0xBC
};
