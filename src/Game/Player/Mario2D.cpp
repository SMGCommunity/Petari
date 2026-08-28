
#include "Game/Player/Mario.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "math_types.hpp"

void FORCE_ADD() {
    TVec3f vec;
    TVec3f vec2;
    vec + vec2;
}

void Mario::stick2DadjustGround(f32& rX, f32& rY) {
    if (isStageCameraRotate2D()) {
        return;
    }

    if (_10.turning && mMovementStates._1 && !mMovementStates.jumping) {
        _10.turning = false;
        clear2DStick();
        _66C = false;
    }

    mSide2D.set(1.0f, 0.0f, 0.0f);
    mUp2D.set(0.0f, 1.0f, 0.0f);
    mNormal2D.set(0.0f, 0.0f, -1.0f);

    PSMTXMultVecSR(_F4, mSide2D, mSide2D);
    PSMTXMultVecSR(_F4, mNormal2D, mNormal2D);
    PSMTXMultVecSR(_F4, mUp2D, mUp2D);

    _6A0 = mNormal2D;

    MR::vecKillElement(_368, _6A0, &_660);

    if (MR::normalizeOrZero(&_660)) {
        _660 = _368;
    }

    f32 dot = _660.dot(mUp2D);

    if (!MR::isInRange(dot, -0.707f, 0.707f)) {
        set2Dmode(dot >= 0.0f);
    }

    TVec3f stickDir2D(rX, rY, 0.0f);
    MR::normalizeOrZero(&stickDir2D);

    if (_66C) {
        if (MR::isNearZero(mStickPos.z)) {
            _66C = false;
            return;
        }

        if (MR::diffAngleAbs(stickDir2D, _670) > 0.3f) {
            _66C = false;
            return;
        }

        PSMTXMultVecSR(MR::tmpMtxRotZRad(-MR::diffAngleSignedHorizontal(_67C, _660, _6A0)), stickDir2D, stickDir2D);
        rX = stickDir2D.x;
        rY = stickDir2D.y;
        return;
    }

    if (MR::isNearZero(mStickPos.z)) {
        return;
    }

    TVec3f vec2;
    vec2.x = _660.dot(getCamDirX());
    vec2.y = _660.dot(getCamDirY());
    vec2.z = 0.0f;

    f32 diff = MR::diffAngleAbs(stickDir2D, vec2);

    // explicit conversions to radian instead of calling MR::toRadian()
    if (diff < 30.0f * MR::pi() / 180.0f || diff > 150.0f * MR::pi() / 180.0f) {
        rX = 0.0f;
        rY = 0.0f;
        mStickPos.z = 0.0f;
        return;
    }

    _670 = stickDir2D;
    _67C = _660;
    _66C = true;
}

void Mario::calcDir2D(f32 x, f32 y, TVec3f* pOut) {
    TVec3f xDir;
    TVec3f yDir;
    MR::vecKillElement(getCamDirX(), _6A0, &xDir);
    MR::vecKillElement(getCamDirY(), _6A0, &yDir);

    MR::normalizeOrZero(&xDir);
    MR::normalizeOrZero(&yDir);

    pOut->set(xDir * x);
    pOut->add(yDir * y);
}
