#include "Game/Player/Mario.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "revolution/mtx.h"

extern const char lbl_805C8E48[];

extern "C" {
    extern u8 sInstance__Q213NrvMarioActor17MarioActorNrvWait;
    extern u8 sInstance__Q213NrvMarioActor21MarioActorNrvGameOver;
    extern u8 sInstance__Q213NrvMarioActor26MarioActorNrvGameOverAbyss;
    extern u8 sInstance__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2;
    extern u8 sInstance__Q213NrvMarioActor25MarioActorNrvGameOverFire;
    extern u8 sInstance__Q213NrvMarioActor30MarioActorNrvGameOverBlackHole;
    extern u8 sInstance__Q213NrvMarioActor28MarioActorNrvGameOverNonStop;
    extern u8 sInstance__Q213NrvMarioActor25MarioActorNrvGameOverSink;
    extern u8 sInstance__Q213NrvMarioActor21MarioActorNrvTimeWait;
    extern u8 sInstance__Q213NrvMarioActor19MarioActorNrvNoRush;

    void __ct__Q213NrvMarioActor17MarioActorNrvWaitFv(void*);
    void __ct__Q213NrvMarioActor21MarioActorNrvGameOverFv(void*);
    void __ct__Q213NrvMarioActor26MarioActorNrvGameOverAbyssFv(void*);
    void __ct__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2Fv(void*);
    void __ct__Q213NrvMarioActor25MarioActorNrvGameOverFireFv(void*);
    void __ct__Q213NrvMarioActor30MarioActorNrvGameOverBlackHoleFv(void*);
    void __ct__Q213NrvMarioActor28MarioActorNrvGameOverNonStopFv(void*);
    void __ct__Q213NrvMarioActor25MarioActorNrvGameOverSinkFv(void*);
    void __ct__Q213NrvMarioActor21MarioActorNrvTimeWaitFv(void*);
    void __ct__Q213NrvMarioActor19MarioActorNrvNoRushFv(void*);
}

namespace {
    const f32 sOne = 1.0f;
    const f32 sZero = 0.0f;
    const f32 sHalf = 0.5f;
    const f32 sNegOne = -1.0f;
    const f32 sPi = 3.1415927f;
    const f32 sHalfPi = 1.5707964f;
    const f32 sBlendRate = 0.1f;
    const f32 sNearZero = 0.001f;
    const f32 sAngleLimit = 3.0415928f;
    const f32 sStickAngle = 0.3f;
    const f32 sInterpEnd = 1.5f;
    const f32 sAngleA = 0.5235988f;
    const f32 sAngleB = 2.6179941f;
    const f32 sAngleC = 1.3962635f;
    const f32 sAngleD = 1.7453294f;
    const f32 sAngleE = 0.19634955f;
    const f32 sAngleF = 2.9452431f;
    const f32 sNearZeroAngle = 0.0001f;
}  // namespace

void Mario::check2DMode() {
    mMovementStates_HIGH_WORD &= ~0x100;
    _10_LOW_WORD &= ~0x400;

    AreaObj* areaObj = MR::getAreaObj(lbl_805C8E48, mPosition);
    if (areaObj != nullptr) {
        mMovementStates_HIGH_WORD |= 0x100;

        TVec3f rotate;
        MR::calcCubeRotate(areaObj, &rotate);
        MR::makeMtxTR(_F4.toMtxPtr(), sZero, sZero, sZero, rotate.x, rotate.y, rotate.z);

        if (((_24 << 23) >> 31) == 0) {
            _688 = mPosition;
            MR::getRotatedAxisZ(&_6A0, rotate);
            MR::calcCubePos(areaObj, &_694);
        }
    } else {
        areaObj = MR::getAreaObj(lbl_805C8E48 + 0x0e, mPosition);
        if (areaObj != nullptr) {
            TVec3f rotate;
            MR::calcCubeRotate(areaObj, &rotate);
            PSMTXCopy(MR::tmpMtxRotYDeg(rotate.y), _F4.toMtxPtr());
            _10_LOW_WORD |= 0x400;

            if (((_28 << 21) >> 31) == 0) {
                _688 = mPosition;
                MR::getRotatedAxisZ(&_6A0, rotate);
                MR::calcCubePos(areaObj, &_694);
            }
        }
    }

    mMovementStates_HIGH_WORD &= ~0x20;

    areaObj = MR::getAreaObj(lbl_805C8E48 + 0x24, mPosition);
    if (areaObj != nullptr) {
        set25Dmode(areaObj);
    }
}

static const int sMarioMove2DSinit = (__ct__Q213NrvMarioActor17MarioActorNrvWaitFv(&sInstance__Q213NrvMarioActor17MarioActorNrvWait),
    __ct__Q213NrvMarioActor21MarioActorNrvGameOverFv(&sInstance__Q213NrvMarioActor21MarioActorNrvGameOver),
    __ct__Q213NrvMarioActor26MarioActorNrvGameOverAbyssFv(&sInstance__Q213NrvMarioActor26MarioActorNrvGameOverAbyss),
    __ct__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2Fv(&sInstance__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2),
    __ct__Q213NrvMarioActor25MarioActorNrvGameOverFireFv(&sInstance__Q213NrvMarioActor25MarioActorNrvGameOverFire),
    __ct__Q213NrvMarioActor30MarioActorNrvGameOverBlackHoleFv(&sInstance__Q213NrvMarioActor30MarioActorNrvGameOverBlackHole),
    __ct__Q213NrvMarioActor28MarioActorNrvGameOverNonStopFv(&sInstance__Q213NrvMarioActor28MarioActorNrvGameOverNonStop),
    __ct__Q213NrvMarioActor25MarioActorNrvGameOverSinkFv(&sInstance__Q213NrvMarioActor25MarioActorNrvGameOverSink),
    __ct__Q213NrvMarioActor21MarioActorNrvTimeWaitFv(&sInstance__Q213NrvMarioActor21MarioActorNrvTimeWait),
    __ct__Q213NrvMarioActor19MarioActorNrvNoRushFv(&sInstance__Q213NrvMarioActor19MarioActorNrvNoRush),
    0);

void Mario::calcMoveDir2D(f32 f1, f32 f2, TVec3f* pOut) {
    f32 stickX = f1;
    f32 stickY = f2;

    TVec3f stack_8C;
    TVec3f stack_80;
    TVec3f stack_74;
    TVec3f stack_68;
    TVec3f stack_5C;

    if (f1 != sZero) {
        stickY = sZero;
    }

    stack_8C = _63C;
    stack_80 = _648;
    stack_74 = _654;

    TVec3f stack_50(_660);
    PSVECCrossProduct(&stack_74, &stack_50, &stack_68);
    MR::normalize(&stack_68);

    if (stack_50.dot(stack_80) < sZero) {
        TVec3f stack_38 = -stack_68;
        stack_68 = stack_38;
    }

    TVec3f stack_2C = -stack_80;
    stack_5C = stack_2C;

    TVec3f stack_44(_398);

    if (_398.dot(stack_50) <= sZero) {
        _398 = stack_50;
    } else {
        MR::vecBlendSphere(_398, stack_50, &_398, sBlendRate);
    }

    if (MR::isNearZero(_398, sNearZero)) {
        _398 = stack_44;
    } else {
        MR::normalize(&_398);
    }

    TVec3f stack_08 = stack_5C * stickY;
    TVec3f stack_14 = stack_68 * stickX;
    TVec3f stack_20(stack_14);
    stack_20.addInLine(stack_08);
    *pOut = stack_20;
}

void Mario::calcShadowDir2D(const TVec3f& rVec, TVec3f* pOut) {
    *pOut = rVec;
    MR::normalizeOrZero(pOut);
}

void Mario::stick2Dadjust(f32& rStickX, f32& rStickY) {
    if (((_10_LOW_WORD << 3) >> 31) && ((mMovementStates_LOW_WORD << 1) >> 31) &&
        !(((mMovementStates_LOW_WORD << 1) | (mMovementStates_LOW_WORD >> 31)) & 1)) {
        _10_LOW_WORD &= ~0x10000000;
        clear2DStick();
    }

    f32 inputX = rStickX;
    f32 inputY = rStickY;

    _63C.x = sOne;
    _63C.y = sZero;
    _63C.z = sZero;

    _648.x = sZero;
    _648.y = sOne;
    _648.z = sZero;

    _654.x = sZero;
    _654.y = sZero;
    _654.z = sNegOne;

    PSMTXMultVecSR(_F4.toMtxPtr(), &_63C, &_63C);
    PSMTXMultVecSR(_F4.toMtxPtr(), &_654, &_654);

    TVec3f up(sZero, sOne, sZero);
    f32 angle = MR::diffAngleSignedHorizontal(up, getCamDirY(), getCamDirZ());

    TVec3f right(sOne, sZero, sZero);
    f32 absAngle = MR::diffAngleAbsHorizontal(right, getCamDirX(), getCamDirZ());

    if (absAngle <= sAngleLimit && __fabsf(angle) <= sAngleLimit) {
        angle = sZero;
        rStickX = -rStickX;
    }

    Mtx rot;
    PSMTXRotAxisRad(rot, &_6A0, angle);
    PSMTXMultVecSR(rot, &_63C, &_63C);
    PSMTXMultVecSR(rot, &_648, &_648);

    MR::vecKillElement(_368, _6A0, &_660);
    if (MR::normalizeOrZero(&_660)) {
        _660 = _368;
    }

    set2Dmode(_660.dot(_648) >= sZero);

    int yDominant = 0;
    int xDominant = 0;

    if (_66C != 0) {
        if (MR::isNearZero(mStickPos.z, sNearZero)) {
            _66C = 0;
        } else {
            TVec3f stick(rStickX, rStickY, sZero);
            f32 diffAngle = MR::diffAngleAbs(stick, _670);

            if (_670.x * rStickX < sZero) {
                _66C = 0;
            } else if (_670.y * rStickY < sZero) {
                _66C = 0;
            } else if (diffAngle < sStickAngle) {
                rStickX = _67C.x;
                rStickY = _67C.y;
            } else {
                _66C = 0;
            }
        }
    }

    if (!_66C && !MR::isNearZero(mStickPos.z, sNearZero) && _60E == 0 && _60F == 0) {
        TVec3f stick(rStickX, rStickY, sZero);
        PSMTXMultVecSR(_F4.toMtxPtr(), &stick, &stick);
        PSMTXMultVecSR(rot, &stick, &stick);

        TVec3f neg = -_660;
        angle = MR::diffAngleAbs(neg, stick);

        f32 t = sOne - (mStickPos.z * mStickPos.z);
        if (t < sZero) {
            t = sZero;
        } else if (t > sOne) {
            t = sOne;
        }

        f32 interp = MR::getInterpolateValue(t, sHalf, sInterpEnd);
        if (angle < interp || angle > (sPi - interp)) {
            rStickX = sZero;
            rStickY = sZero;
            mStickPos.z = sZero;
        } else {
            _670 = stick;
            MR::vecKillElement(stick, _660, &stick);
            rStickX = stick.x;
            rStickY = stick.y;
            _67C = stick;
            _66C = 1;
        }
    }

    if (__fabsf(rStickY) > __fabsf(rStickX)) {
        yDominant = 1;
        rStickX = sZero;
        if (rStickY > sZero) {
            rStickY = mStickPos.z;
        } else {
            rStickY = -mStickPos.z;
        }
    } else {
        xDominant = 1;
        rStickY = sZero;
        if (rStickX > sZero) {
            rStickX = mStickPos.z;
        } else {
            rStickX = -mStickPos.z;
        }
    }

    angle = MR::diffAngleAbs(_660, _648);
    u8 flagX = 0;
    u8 flagY = 0;

    if (!((_60E != 0 || _60F != 0) && _610 == 0 && _611 == 0)) {
        bool doRange2 = false;

        if (angle < sAngleA || angle > sAngleB || _610 != 0) {
            if (_611 == 0) {
                if (_620 == sZero) {
                    rStickY = sZero;
                    if (yDominant && __fabsf(inputX) > sStickAngle) {
                        rStickX = (inputX < sZero) ? -mStickPos.z : mStickPos.z;
                        flagX = 1;
                    }
                }
            } else {
                doRange2 = true;
            }
        } else {
            doRange2 = true;
        }

        if (doRange2) {
            if ((angle <= sAngleC || angle >= sAngleD) && _611 == 0) {
            } else if (_610 == 0 && _61C == sZero) {
                rStickX = sZero;
                if (xDominant && __fabsf(inputY) > sStickAngle) {
                    rStickY = (inputY < sZero) ? -mStickPos.z : mStickPos.z;
                    flagY = 1;
                }
            }
        }
    }

    _61C = rStickX;
    _620 = rStickY;
    _610 = flagX;
    _611 = flagY;

    if (_60E == 0 && _60F == 0 && rStickY != sZero) {
        if (MR::isInRange(angle, sAngleE, sHalfPi)) {
            _60F = (_660.dot(_63C) < sZero) ? 1 : 2;
        } else if (MR::isInRange(angle, sHalfPi, sAngleF)) {
            _60F = (_660.dot(_63C) < sZero) ? 2 : 1;
        }
    }

    if (_60F == 1) {
        if (rStickY == sZero) {
            _60F = 0;
        } else {
            rStickX = rStickY;
        }
    } else if (_60F == 2) {
        if (rStickY == sZero) {
            _60F = 0;
        } else {
            rStickX = -rStickX;
            rStickY = -rStickY;
        }
    } else {
        _60F = 0;
    }

    if (_60E == 1) {
        if (rStickX > sZero) {
            rStickX = -rStickX;
        } else {
            _60E = 0;
        }
    } else if (_60E == 2) {
        if (rStickX < sZero) {
            rStickX = -rStickX;
        } else {
            _60E = 0;
        }
    } else {
        _60E = 0;
    }

    _614 = rStickX;
    _618 = rStickY;

    bool useY = MR::isNearZero(angle - sHalfPi, sNearZeroAngle);
    if (!useY) {
        rStickY = sZero;
        mStickPos.z = __fabsf(rStickX);
    } else {
        rStickX = sZero;
        mStickPos.z = __fabsf(rStickY);
    }
}

void Mario::set2Dmode(bool enable) {
    if (_624 == enable) {
        return;
    }

    if (_614 > sZero) {
        _60E = 1;
    }

    if (_614 < sZero) {
        _60E = 2;
    }

    _624 = enable;
}

void Mario::beforeJumping2D() {
    _628 = _660;
}

void Mario::afterLanding2D() {}

void Mario::clear2DStick() {
    _60E = 0;
    _60F = 0;
    _610 = 0;
    _611 = 0;
    _618 = sZero;
    _614 = sZero;
    _620 = sZero;
    _61C = sZero;
    _624 = 0;
    _628.zero();
    _66C = 0;
    _634 = sZero;
    _670.zero();
    _67C.zero();
}
