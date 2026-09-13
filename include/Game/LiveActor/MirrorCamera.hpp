#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class J3DModelData;

class MirrorCamera : public NameObj {
public:
    MirrorCamera(const char*);

    virtual ~MirrorCamera();

    virtual void init(const JMapInfoIter&) override;
    virtual void movement() override;

    void setMirrorMapInfo(const TVec3f&, const TVec3f&);
    void setMirrorMapInfo(J3DModelData*);
    void updateViewMtx();
    void updateModelTexMtx();
    f32 getDistance(const TVec3f&) const;

    /* 0x0C */ TVec3f _C;
    /* 0x18 */ TVec3f _18;
    /* 0x24 */ f32 _24;
    /* 0x28 */ TPos3f mViewMtx;
    /* 0x58 */ TPos3f mModelTexMtx;
};

namespace MR {
    MirrorCamera* getMirrorCamera();
    f32 getDistanceToMirror(const TVec3f&);
};  // namespace MR
