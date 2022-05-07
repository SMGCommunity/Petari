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

// CameraRepulsiveCylinder::getRepulsion

const char* CameraRepulsiveArea::getManagerName() const {
    return "CameraRepulsiveArea";
}