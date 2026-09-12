#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void Mario::set25Dmode(const AreaObj* area) {
    Mtx matrix;
    TVec3f rotation;
    MR::calcCubeRotate(area, &rotation);
    TVec3f normal(0.0f, 0.0f, 1.0f);
    TVec3f up(0.0f, 1.0f, 0.0f);
    MR::makeMtxTR(matrix, 0.0f, 0.0f, 0.0f, rotation.x, rotation.y, rotation.z);
    PSMTXMultVec(matrix, &normal, &normal);
    PSMTXMultVec(matrix, &up, &up);
    _6D0 = normal;
    _6DC = up;
    _6E8.cross(_6D0, _6DC);
    MR::normalize(&_6E8);
    mMovementStates._3A = true;
}

void Mario::update25Dmode() {
    TVec3f up;
    MR::vecKillElement(-*getGravityVec(), _6D0, &up);
    MR::normalizeOrZero(&up);
    if (MR::isNearZero(up)) {
        _6AC = -getGravityVec()->dot(_6D0) > 0.0f ? 4 : 5;
        return;
    }

    f32 sideDot = up.dot(_6E8);
    f32 upDot = up.dot(_6DC);
    f32 normalDot = up.dot(_6D0);
    f32 sideAbs = __fabsf(sideDot);
    f32 upAbs = __fabsf(upDot);
    f32 normalAbs = __fabsf(normalDot);
    u8 mode;
    if (sideAbs > upAbs && sideAbs > normalAbs) {
        mode = sideDot > 0.0f ? 2 : 3;
    } else if (upAbs > sideAbs && upAbs > normalAbs) {
        if (upDot > 0.0f) {
            mode = 0;
        } else {
            mode = 1;
        }
    } else {
        mode = normalDot > 0.0f ? 4 : 5;
    }
    _6AD = mode;
    if (mode == 2 || mode == 3) {
        if (mode == 2 || mode == 3) {
            f32 threshold = 0.965f;
            if (_6AC == 2 || _6AC == 3) {
                threshold = 0.9397f;
            }
            if (sideAbs < threshold) {
                if (upAbs > normalAbs) {
                    if (upDot > 0.0f) {
                        mode = 0;
                    } else {
                        mode = 1;
                    }
                } else {
                    return;
                }
            }
        }
    }

    if (!isStickOn()) {
        _6AC = mode;
        mMovementStates._3B = false;
    } else if (!mMovementStates._3B && mStickPos.z > 0.7f) {
        _6C8.set(mStickPos.x, mStickPos.y);
        _6AC = mode;
        mMovementStates._3B = true;
    } else {
        TVec2f stick(mStickPos.x, mStickPos.y);
        if (MR::diffAngleAbs(_6C8, stick) >= 0.7853982f) {
            _6AC = mode;
            _6C8.set(mStickPos.x, mStickPos.y);
        }
    }
}

void Mario::updateAxisFromMode(u8 mode) {
    if (mMovementStates.jumping) {
        return;
    }

    TVec3f side;
    TVec3f normal;
    if (mode < 4) {
        TVec3f up;
        TVec3f planeUp;
        MR::vecKillElement(-*getGravityVec(), _6D0, &up);
        MR::vecKillElement(_6DC, _6D0, &planeUp);
        MR::normalizeOrZero(&up);
        MR::normalizeOrZero(&planeUp);
        f32 dot = up.dot(planeUp);
        if (__fabsf(dot) > 0.99f) {
            switch (mode) {
            case 0:
                side = _6E8;
                normal = _6D0;
                if (dot < 0.0f) {
                    side = -side;
                }
                break;
            case 1:
                side = _6E8;
                normal = _6D0;
                if (dot > 0.0f) {
                    side = -side;
                }
                break;
            case 2:
                side = _6D0;
                normal = -_6E8;
                if (dot < 0.0f) {
                    side = -side;
                    normal = -normal;
                }
                break;
            case 3:
                side = -_6D0;
                normal = _6E8;
                if (dot < 0.0f) {
                    side = -side;
                    normal = -normal;
                }
                break;
            }
        } else {
            TVec3f axis;
            axis.cross(up, planeUp);
            MR::normalize(&axis);
            if (axis.dot(_6D0) < 0.0f) {
                axis = -axis;
            }
            TVec3f tangent;
            tangent.cross(axis, up);
            MR::normalize(&tangent);
            switch (mode) {
            case 0:
                side = tangent;
                normal = axis;
                break;
            case 1:
                side = -tangent;
                normal = axis;
                break;
            case 2:
                side = axis;
                normal = -tangent;
                break;
            case 3:
                side = -axis;
                normal = tangent;
                break;
            }
        }
    }
    _6B0 = side;
    _6BC = -normal;
}

void Mario::calcMoveDir25D(f32 x, f32 y, TVec3f* direction) {
    *direction = _6B0 * x - _6BC * y;
}
