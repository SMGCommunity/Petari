
#include "Game/Player/Mario.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void FORCE_ADD() {
    TVec3f vec;
    TVec3f vec2;
    vec + vec2;
}

void Mario::stick2DadjustGround(f32& f1, f32& f2) {
    if (isStageCameraRotate2D()) {
        return;
    }

    if (_10.turning && mMovementStates._1 && !mMovementStates.jumping) {
        _10.turning = false;
        clear2DStick();
        _66C = 0;
    }

    _63C.set(1.0f, 0.0f, 0.0f);
    _648.set(0.0f, 1.0f, 0.0f);
    _654.set(0.0f, 0.0f, -1.0f);

    PSMTXMultVecSR(_F4, _63C, _63C);
    PSMTXMultVecSR(_F4, _654, _654);
    PSMTXMultVecSR(_F4, _648, _648);

    _6A0 = _654;

    MR::vecKillElement(_368, _6A0, &_660);

    if (MR::normalizeOrZero(&_660)) {
        _660 = _368;
    }

    f32 dot = _660.dot(_648);

    if (!MR::isInRange(dot, -0.707f, 0.707f)) {
        set2Dmode(dot >= 0.0f);
    }

    TVec3f vec(f1, f2, 0.0f);
    MR::normalizeOrZero(&vec);

    if (_66C != 0) {
        if (MR::isNearZero(mStickPos.z)) {
            _66C = 0;
            return;
        }

        if (MR::diffAngleAbs(vec, _670) > 0.3f) {
            _66C = 0;
            return;
        }

        PSMTXMultVecSR(MR::tmpMtxRotZRad(-MR::diffAngleSignedHorizontal(_67C, _660, _6A0)), vec, vec);
        f1 = vec.x;
        f2 = vec.y;
    } else if (!MR::isNearZero(mStickPos.z)) {
        TVec3f vec2;
        vec2.x = _660.dot(getCamDirX());
        vec2.y = _660.dot(getCamDirY());
        vec2.z = 0.0f;

        f32 diff = MR::diffAngleAbs(vec, vec2);

        if (diff < 0.5235988f || diff > 2.617994f) {
            f1 = 0.0f;
            f2 = 0.0f;
            mStickPos.z = 0.0f;
        } else {
            _670 = vec;
            _67C = _660;
            _66C = 1;
        }
    }
}

void Mario::calcDir2D(f32 f1, f32 f2, TVec3f* pOut) {
    TVec3f vec;
    TVec3f vec2;
    MR::vecKillElement(getCamDirX(), _6A0, &vec);
    MR::vecKillElement(getCamDirY(), _6A0, &vec2);

    MR::normalizeOrZero(&vec);
    MR::normalizeOrZero(&vec2);

    pOut->set(vec * f1);

    pOut->add(vec2 * f2);
}
