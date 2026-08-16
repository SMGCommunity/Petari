#include "Game/Util/GeometryBindUtil.hpp"
#include "revolution/types.h"

BindSphere::BindSphere(const TVec3f& a1, f32 a2) : _0(a1) {
    _C = a2;
}

// BindCone::BindCone
// BindCone::isInPoint

void BindCone::calcVertexPosition() {
    _C = _24 + (_18 * _24.x);
}

// ???
void BindCone::calcGeneratrixLength() {
    f32 v1 = (_24.x * _24.x) + (_24.y + _24.y);
    f32 v3;

    if (v1 > 0.0f) {
        f32 v2 = __frsqrte(v1);
        v3 = ((-(((v2 * ((_24.x * _24.x) + (_24.y * _24.y))) * v2) - 3.0) * (v2 * ((_24.x * _24.x) + (_24.y * _24.y)))) / 2.0f);
    } else {
        v3 = (_24.x * _24.x) + (_24.y + _24.y);
    }

    _24.z = v3;
}

void BindCone::setPosition(const TVec3f& rPos) {
    _0 = rPos;
    calcVertexPosition();
}

void BindCone::setDirection(const TVec3f& rDir) {
    _18 = rDir;
    calcVertexPosition();
}
