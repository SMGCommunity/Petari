#include "Game/LiveActor/Nerve.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "revolution/mtx.h"


void Mario::check2DMode() {
    mMovementStates._37 = false;
    _10._15 = false;

    AreaObj* area = MR::getAreaObj("PlaneModeCube", mPosition);
    if (area != nullptr) {
        mMovementStates._37 = true;

        TVec3f rotate;
        MR::calcCubeRotate(area, &rotate);
        MR::makeMtxTR(_F4, 0.0f, 0.0f, 0.0f, rotate.x, rotate.y, rotate.z);

        if (!(_24 & 0x100)) {
            _688 = mPosition;
            MR::getRotatedAxisZ(&_6A0, rotate);
            MR::calcCubePos(area, &_694);
        }
    } else {
        area = MR::getAreaObj("PlaneCircularModeCube", mPosition);
        if (area != nullptr) {
            TVec3f rotate;
            MR::calcCubeRotate(area, &rotate);
            PSMTXCopy(MR::tmpMtxRotYDeg(rotate.y), _F4);
            _10._15 = true;

            if (!(_28 & 0x400)) {
                _688 = mPosition;
                MR::getRotatedAxisZ(&_6A0, rotate);
                MR::calcCubePos(area, &_694);
            }
        }
    }

    mMovementStates._3A = false;
    area = MR::getAreaObj("PipeModeCube", mPosition);
    if (area != nullptr) {
        set25Dmode(area);
    }
}

void Mario::calcMoveDir2D(f32 a1, f32 a2, TVec3f* pOut) {
    if (a1 != 0.0f) {
        a2 = 0.0f;
    }

    TVec3f stack_8C;
    TVec3f stack_80;
    TVec3f stack_74;
    TVec3f stack_68;
    TVec3f stack_5C;

    stack_8C = _63C;
    stack_80 = _648;
    stack_74 = _654;

    TVec3f stack_50(_660);
    PSVECCrossProduct(&stack_74, &stack_50, &stack_68);
    MR::normalize(&stack_68);

    if (stack_50.dot(stack_80) < 0.0f) {
        TVec3f stack_38 = -stack_68;
        stack_68 = stack_38;
    }

    TVec3f stack_2C = -stack_80;
    stack_5C = stack_2C;
    TVec3f stack_44(_398);

    if (_398.dot(stack_50) <= 0.0f) {
        _398 = stack_50;
    } else {
        MR::vecBlendSphere(_398, stack_50, &_398, 0.1f);
    }

    if (MR::isNearZero(_398)) {
        _398 = stack_44;
    } else {
        MR::normalize(&_398);
    }

    TVec3f stack_8(stack_5C * a2);
    TVec3f stack_14(stack_68 * a1);
    TVec3f stack_20(stack_14);
    stack_20.addInline(stack_8);
    *pOut = stack_20;
}

void Mario::calcShadowDir2D(const TVec3f& rVec, TVec3f* pOut) {
    *pOut = rVec;
    MR::normalizeOrZero(pOut);
}

void Mario::stick2Dadjust(f32& rStickX, f32& rStickY) {
    if (_10.turning && mMovementStates._1 && !mMovementStates.jumping) {
        _10.turning = false;
        clear2DStick();
    }

    f32 stickXInput = rStickX;
    f32 stickYInput = rStickY;

    _63C.set(1.0f, 0.0f, 0.0f);
    _648.set(0.0f, 1.0f, 0.0f);
    _654.set(0.0f, 0.0f, -1.0f);
    PSMTXMultVecSR(_F4, &_63C, &_63C);
    PSMTXMultVecSR(_F4, &_654, &_654);

    TVec3f stack_38(0.0f, 1.0f, 0.0f);
    TVec3f* camDirZ = &getCamDirZ();
    f32 angle = MR::diffAngleSignedHorizontal(getCamDirY(), stack_38, *camDirZ);

    TVec3f stack_2C(1.0f, 0.0f, 0.0f);
    camDirZ = &getCamDirZ();
    const f32 absHorizontal = MR::diffAngleAbsHorizontal(getCamDirX(), stack_2C, *camDirZ);
    if (absHorizontal <= 3.0415928f && __fabsf(angle) <= 3.0415928f) {
        angle = 0.0f;
        rStickX = -rStickX;
    }

    Mtx rotMtx;
    PSMTXRotAxisRad(rotMtx, &_6A0, angle);
    PSMTXMultVecSR(rotMtx, &_63C, &_63C);
    PSMTXMultVecSR(rotMtx, &_648, &_648);

    MR::vecKillElement(_368, _6A0, &_660);
    if (MR::normalizeOrZero(&_660)) {
        _660 = _368;
    }

    set2Dmode(_660.dot(_648) >= 0.0f);

    bool isMainY = false;
    bool isMainX = false;
    bool isNearHalfPi = false;

    if (_66C) {
        if (MR::isNearZero(mStickPos.z)) {
            _66C = 0;
        } else {
            TVec3f stickVec(rStickX, rStickY, 0.0f);
            const f32 diff = MR::diffAngleAbs(stickVec, _670);

            if (_670.x * rStickX < 0.0f) {
                _66C = 0;
            } else if (_670.y * rStickY < 0.0f) {
                _66C = 0;
            } else if (diff < 0.3f) {
                rStickX = _67C.x;
                rStickY = _67C.y;
            } else {
                _66C = 0;
            }
        }
    }

    if (!_66C && !MR::isNearZero(mStickPos.z) && !_60E && !_60F) {
        TVec3f stickVec(rStickX, rStickY, 0.0f);
        PSMTXMultVecSR(_F4, &stickVec, &stickVec);
        PSMTXMultVecSR(rotMtx, &stickVec, &stickVec);

        TVec3f invMove(-_660);
        const f32 diff = MR::diffAngleAbs(invMove, stickVec);
        f32 clamp = 1.0f - (mStickPos.z * mStickPos.z);
        if (clamp < 0.0f) {
            clamp = 0.0f;
        } else if (clamp > 1.0f) {
            clamp = 1.0f;
        }

        const f32 threshold = MR::getInterpolateValue(clamp, 0.5f, 1.5f);
        if (diff < threshold || diff > (3.1415927f - threshold)) {
            rStickX = 0.0f;
            rStickY = 0.0f;
            mStickPos.z = 0.0f;
        } else {
            _670 = stickVec;
            MR::vecKillElement(stickVec, _660, &stickVec);
            rStickX = stickVec.x;
            rStickY = stickVec.y;
            _67C = stickVec;
            _66C = 1;
        }
    }

    if (__fabsf(rStickY) > __fabsf(rStickX)) {
        isMainY = true;
        rStickX = 0.0f;
        rStickY = rStickY > 0.0f ? mStickPos.z : -mStickPos.z;
    } else {
        isMainX = true;
        rStickY = 0.0f;
        rStickX = rStickX > 0.0f ? mStickPos.z : -mStickPos.z;
    }

    const f32 angleMove = MR::diffAngleAbs(_660, _648);
    u8 lockX = 0;
    u8 lockY = 0;

    if (!((_60E != 0 || _60F != 0) && _610 == 0 && _611 == 0)) {
        bool doSecondBlock = false;

        if (angleMove < 0.5235988f || angleMove > 2.617994f || _610 != 0) {
            if (_611 == 0) {
                if (_620 == 0.0f) {
                    rStickY = 0.0f;
                    if (isMainY && __fabsf(stickXInput) > 0.3f) {
                        rStickX = stickXInput < 0.0f ? -mStickPos.z : mStickPos.z;
                        lockX = 1;
                    }
                }
            } else {
                doSecondBlock = true;
            }
        } else {
            doSecondBlock = true;
        }

        if (doSecondBlock) {
            if ((angleMove <= 1.3962635f || angleMove >= 1.7453294f) && _611 == 0) {
            } else if (_610 == 0 && _61C == 0.0f) {
                rStickX = 0.0f;
                if (isMainX && __fabsf(stickYInput) > 0.3f) {
                    rStickY = stickYInput < 0.0f ? -mStickPos.z : mStickPos.z;
                    lockY = 1;
                }
            }
        }
    }

    _61C = rStickX;
    _620 = rStickY;
    _610 = lockX;
    _611 = lockY;

    if (!_60E && !_60F && rStickY != 0.0f) {
        if (MR::isInRange(angleMove, 0.19634955f, 1.5707964f)) {
            _60F = _660.dot(_63C) < 0.0f ? 1 : 2;
        } else if (MR::isInRange(angleMove, 1.5707964f, 2.9452431f)) {
            _60F = _660.dot(_63C) < 0.0f ? 2 : 1;
        }
    }

    switch (_60F) {
    case 1:
        if (rStickY == 0.0f) {
            _60F = 0;
        } else {
            rStickX = rStickY;
        }
        break;
    case 2:
        if (rStickY == 0.0f) {
            _60F = 0;
        } else {
            rStickX = -rStickY;
            rStickY = -rStickY;
        }
        break;
    default:
        _60F = 0;
        break;
    }

    switch (_60E) {
    case 1:
        if (rStickX > 0.0f) {
            rStickX = -rStickX;
        } else {
            _60E = 0;
        }
        break;
    case 2:
        if (rStickX < 0.0f) {
            rStickX = -rStickX;
        } else {
            _60E = 0;
        }
        break;
    default:
        _60E = 0;
        break;
    }

    _614 = rStickX;
    _618 = rStickY;

    if (MR::isNearZero(angleMove - 1.5707964f, 0.0001f)) {
        isNearHalfPi = true;
    }

    if (!isNearHalfPi) {
        rStickY = 0.0f;
        mStickPos.z = __fabsf(rStickX);
    } else {
        rStickX = 0.0f;
        mStickPos.z = __fabsf(rStickY);
    }
}

void Mario::set2Dmode(bool mode) {
    if (mode == _624) {
        return;
    }

    if (_614 > 0.0f) {
        _60E = 1;
    }

    if (_614 < 0.0f) {
        _60E = 2;
    }

    _624 = mode;
}

void Mario::beforeJumping2D() {
    _628 = _660;
}

void Mario::afterLanding2D() {
}

void Mario::clear2DStick() {
    _60E = 0;
    _60F = 0;
    _610 = 0;
    _611 = 0;
    _618 = 0.0f;
    _614 = 0.0f;
    _620 = 0.0f;
    _61C = 0.0f;
    _624 = 0;
    _628.zero();
    _66C = 0;
    _634 = 0.0f;
    _670.zero();
    _67C.zero();
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
