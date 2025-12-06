#pragma once

#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class LiveActor;

class FixedPosition {
public:
    FixedPosition(const LiveActor*, const char*, const TVec3f&, const TVec3f&);
    FixedPosition(const LiveActor*, const TVec3f&, const TVec3f&);
    FixedPosition(MtxPtr, const TVec3f&, const TVec3f&);
    FixedPosition(const LiveActor*, const char*, const LiveActor*);

    void init(MtxPtr, const TVec3f&, const TVec3f&);
    void calc();
    void setBaseMtx(MtxPtr);
    void setLocalTrans(const TVec3f&);
    void copyTrans(TVec3f*) const;
    void copyRotate(TVec3f*) const;

    MtxPtr mBaseMtx;     // 0x0
    TVec3f mLocalTrans;  // 0x4
    TVec3f _10;
    TPos3f _1C;
    bool mNormalizeScale;  // 0x4C
};