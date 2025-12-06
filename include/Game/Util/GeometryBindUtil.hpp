#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class BindSphere {
public:
    TVec3f _0;
    f32 _C;
};

class BindCone {
public:
    BindCone(const TVec3f&, const TVec3f&, f32, f32);

    bool isInPoint(const TVec3f&) const;
    void calcVertexPosition();
    void calcGeneratixLength();
    void setPosition(const TVec3f&);
    void setDirection(const TVec3f&);

    TVec3f _0;
    TVec3f _C;
    TVec3f _18;
    TVec3f _24;
};