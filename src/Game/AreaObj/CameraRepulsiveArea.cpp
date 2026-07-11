#include "Game/AreaObj/CameraRepulsiveArea.hpp"
#include "Game/Util/AreaObjUtil.hpp"

namespace {
    // static const s32 sPow = _;
    // static const s32 sCylPower = _;
};  // namespace

CameraRepulsiveArea::~CameraRepulsiveArea() {
}

CameraRepulsiveSphere::~CameraRepulsiveSphere() {
}

CameraRepulsiveCylinder::~CameraRepulsiveCylinder() {
}

TVec3f CameraRepulsiveSphere::getRepulsion(const TVec3f& rRep) {
    return TVec3f(0.0f, 0.0f, 0.0f);
}

TVec3f CameraRepulsiveCylinder::getRepulsion(const TVec3f& rRep) {
    TVec3f position;
    MR::calcCylinderPos(&position, this);
    TVec3f upVec;
    MR::calcCylinderUpVec(&upVec, this);

    TVec3f ret(rRep);
    ret -= position;
    f32 dot = upVec.dot(ret);
    TVec3f _14;
    TVec3f _8(upVec);
    _8 *= dot;
    ret -= _8;

    _14.cross(ret, upVec);

    f32 base = (_14.length() * 2.0f) / MR::getCylinderRadius(this);

    // denom = base^3
    f32 denom = base;

    for (int i = 0; i < 2; i++) {
        denom *= base;
    }

    ret /= denom;

    return ret;
}
