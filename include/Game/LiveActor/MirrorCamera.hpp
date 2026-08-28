#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class J3DModelData;

class MirrorCamera : public NameObj {
public:
    MirrorCamera(const char*);

    /* 0x08 */ virtual ~MirrorCamera();

    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x14 */ virtual void movement() override;

    void setMirrorMapInfo(const TVec3f&, const TVec3f&);
    void setMirrorMapInfo(J3DModelData*);
    void updateViewMtx();
    void updateModelTexMtx();
    f32 getDistance(const TVec3f&) const;

    TVec3f _C;
    TVec3f _18;
    f32 _24;
    TPos3f mViewMtx;      // 0x28
    TPos3f mModelTexMtx;  // 0x58
};

namespace MR {
    MirrorCamera* getMirrorCamera();
    f32 getDistanceToMirror(const TVec3f&);
};  // namespace MR
