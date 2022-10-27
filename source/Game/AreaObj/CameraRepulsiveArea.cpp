#include "Game/AreaObj/CameraRepulsiveArea.h"

CameraRepulsiveArea::~CameraRepulsiveArea() {

}

CameraRepulsiveSphere::~CameraRepulsiveSphere() {

}

CameraRepulsiveCylinder::~CameraRepulsiveCylinder() {

}

TVec3f CameraRepulsiveSphere::getRepulsion(const TVec3f &rRep) {
    TVec3f ret;
    ret.x = 0.0f;
    ret.y = 0.0f;
    ret.z = 0.0f;
    return ret;
}

TVec3f CameraRepulsiveCylinder::getRepulsion(const TVec3f &rRep) {
    TVec3f position;
    MR::calcCylinderPos(&position, this);
    TVec3f upVec;
    MR::calcCylinderUpVec(&upVec, this);

    TVec3f ret(rRep);
    ret -= position;
    f32 dot = ret.dot(upVec);
    TVec3f _14;
    TVec3f _8(upVec);
    _8.scale(dot);
    ret -= _8;

    PSVECCrossProduct(ret.toVec(), upVec.toVec(), _14.toVec());
    f32 mag = PSVECMag(_14.toVec());
    f32 radius = MR::getCylinderRadius(this);

    f32 scale = (2.0f * mag) / radius;

    // denom = scale^3
    f32 denom = scale;

    for (int i = 0; i < 2; i++) {
        denom *= scale;
    }

    ret.scale(1.0f / denom);
    return ret;
}

const char* CameraRepulsiveArea::getManagerName() const {
    return "CameraRepulsiveArea";
}