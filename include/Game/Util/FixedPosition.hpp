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

    /* 0x00 */ TMtx34f* mBaseMtx;
    /* 0x04 */ TVec3f mLocalTrans;
    /* 0x10 */ TVec3f mRotDegrees;
    /* 0x1C */ TPos3f mMtx;
    /* 0x4C */ bool mNormalizeScale;
};
