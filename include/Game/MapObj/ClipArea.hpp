#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ClipAreaShape;

class ClipArea : public LiveActor {
public:
    /// @brief Creates a new `ClipArea`.
    /// @param pName A pointer to the null-terminated name of the object.
    ClipArea(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)&mBaseMtx;
    }
    virtual bool isInArea(const TVec3f&, f32) const;

    void initBaseMatrix(const JMapInfoIter&);
    void setShape(ClipAreaShape*);

    /* 0x8C */ TPos3f mBaseMtx;
    /* 0xBC */ ClipAreaShape* mShape;
};

namespace MR {
    void setClipSphereArea(ClipArea*);
    void setClipCenterBoxArea(ClipArea*);
    void setClipBottomBoxArea(ClipArea*);
    void setClipTopConeArea(ClipArea*);
    void setClipBottomConeArea(ClipArea*);
};  // namespace MR
