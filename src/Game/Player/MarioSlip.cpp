#include "Game/Player/Mario.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/MathUtil.hpp"

extern "C" {
    extern const char lbl_805CC3E0[];
    extern const char lbl_805CC46E[];
}

void Mario::clearSlope() {
    _8F0 = 0.0f;
    mMovementStates._23 = false;
    _8F8.zero();
    _280 = 0.0f;
    _910.zero();
    _284.zero();
    _3C2 = 0;
    _2C4.zero();
    _3C4 = 0;
    _904.zero();
}

bool Mario::isEnableSlopeMove() const {
    if (!isSlipFloorCode(_960)) {
        if ((mMovementStates_HIGH_WORD & 0x10000000) == 0) {
            if (MR::isNearZero(_8F8, 0.001f)) {
                return false;
            }
        }
    }

    const s32 status = static_cast<s32>(getCurrentStatus());
    if (status == 1) {
        return false;
    }

    if (status < 1) {
        if (status >= 0) {
            return true;
        }
    }
    else if (status < 5) {
        return true;
    }

    return false;
}

void Mario::moveSlopeSlide() {
    const char* pAnimTable = lbl_805CC3E0;

    if (calcAngleD(_368) < 5.8f) {
        if ((mDrawStates_WORD & 0x00080000) != 0) {
            const TVec3f* pWorldPadDir = &getWorldPadDir();
            f32 speed = PSVECMag(&_16C);
            if (PSVECMag(&_910) < speed) {
                _910 = _16C;
            }

            TVec3f stack_8(*pWorldPadDir);
            stack_8.scale(0.5f);
            _910 += stack_8;

            if (PSVECMag(&_910) > 15.0f) {
                _910.setLength(15.0f);
            }

            addVelocity(_910);
        }
    }
    else if (isAnimationRun(pAnimTable + 0x2F) || isAnimationRun(pAnimTable + 0x18, 2) || isAnimationRun(pAnimTable + 0x3E, 3)) {
        TVec3f stack_20;
        PSVECCrossProduct(&getAirGravityVec(), &_368, &stack_20);
        MR::normalizeOrZero(&stack_20);

        const TVec3f* pWorldPadDir = &getWorldPadDir();
        if (MR::isNearZero(_280, 0.001f)) {
            _284 = stack_20;
        }

        if (isStickOn() && _3C4 == 0) {
            f32 dot = _284.dot(*pWorldPadDir);
            if (__fabsf(dot) > 0.2f) {
                MarioConstTable* pConstTable = mActor->mConst->mTable[mActor->mConst->mCurrentTable];
                f32 sideInertia = pConstTable->mSlopeSideMoveInertia;
                _280 = (_280 * sideInertia) + (dot * (1.0f - sideInertia));
            }
            else {
                MarioConstTable* pConstTable = mActor->mConst->mTable[mActor->mConst->mCurrentTable];
                _280 = _280 * pConstTable->mSlopeSideStopInertia;
            }
        }
        else {
            MarioConstTable* pConstTable = mActor->mConst->mTable[mActor->mConst->mCurrentTable];
            _280 = _280 * pConstTable->mSlopeSideStopInertia;
        }

        TVec3f stack_14;
        PSVECCrossProduct(&_284, &_368, &stack_14);
        PSVECCrossProduct(&_368, &stack_14, &_284);

        MarioConstTable* pConstTable = mActor->mConst->mTable[mActor->mConst->mCurrentTable];
        addVelocity(_284, _280 * pConstTable->mSlopeSideMoveSpeed);
    }
}

void Mario::slopeMove() {
    if (mActor->mHealth == 0) {
        return;
    }

    if ((mMovementStates_LOW_WORD & 0x40000000) == 0) {
        return;
    }

    f32 blendRate = MR::clamp(_278, 0.0f, 1.0f);

    TVec3f stack_170;
    PSVECCrossProduct(&_368, &getAirGravityVec(), &stack_170);
    MR::normalizeOrZero(&stack_170);

    TVec3f stack_17C;
    if (MR::isNearZero(stack_170, 0.001f)) {
        stack_17C.zero();
        _904.zero();
    } else {
        PSVECCrossProduct(&_368, &stack_170, &stack_17C);
        MR::normalize(&stack_17C);
        _904 = stack_17C;
    }

    if (MR::isSameDirection(stack_17C, getAirGravityVec(), 0.01f) ||
        MR::isOppositeDirection(stack_17C, getAirGravityVec(), 0.01f)) {
        mMovementStates_HIGH_WORD &= ~0x10000000;
        _904.zero();
        stack_17C.zero();
    }

    f32 slopeAngle = calcAngleD(_368);

    if ((mDrawStates_WORD & 0x00000100) != 0 && slopeAngle >= 30.0f) {
        if ((mMovementStates_HIGH_WORD & 0x10000000) == 0) {
            TVec3f stack_E0(-_2C4);
            TVec3f stack_EC(stack_E0);
            stack_EC.scale(2.0f);
            _8F8 = stack_EC;
            mMovementStates_HIGH_WORD |= 0x10000000;
            _3C4 = 30;
            _8F0 = 10.0f;
        } else {
            _8F0 = 10.0f;
        }
    }

    if (_3C4 > 0xF) {
        TVec3f stack_C8(-_2C4);
        TVec3f stack_D4(stack_C8);
        stack_D4.scale(0.5f);
        _8F8 += stack_D4;
    }

    if ((mDrawStates_WORD & 0x00080000) == 0 || !isSlipPolygon(_45C)) {
        blendRate = 1.0f;
        if ((mMovementStates_HIGH_WORD & 0x10000000) != 0) {
            if (slopeAngle >= 15.0f) {
                _8F0 -= 0.4f;
            } else {
                _8F0 -= 0.8f;
            }

            if (_8F0 < 0.0f) {
                _8F0 = 0.0f;
                mMovementStates_HIGH_WORD &= ~0x10000000;
            }

            mVelocity.zero();
        } else {
            _8F0 = 0.0f;
        }
    } else {
        if ((mMovementStates_HIGH_WORD & 0x10000000) == 0) {
            if (slopeAngle >= mActor->mConst->getTable()->mSlipAngle) {
                TVec3f stack_164;
                if (MR::vecKillElement(mVelocity, stack_17C, &stack_164) < 0.0f) {
                    _8F8 = mVelocity;
                } else {
                    _8F8.zero();
                }

                mVelocity.zero();
                mMovementStates_HIGH_WORD |= 0x10000000;
                _8F0 = 10.0f;
                return;
            }

            TVec3f stack_158;
            const f32 sideSpeed = MR::vecKillElement(mVelocity, stack_17C, &stack_158);

            if (_278 < 0.1f) {
                _8F0 = 10.0f;
            }

            if (_71C == 0 && !isAnimationRun(lbl_805CC3E0 + 0x0)) {
                _8F0 = 10.0f;
            }

            if (slopeAngle >= 25.0f && isStickOn()) {
                const TVec3f& worldPadDir = getWorldPadDir();

                TVec3f stack_14C;
                MR::vecKillElement(stack_17C, getCamDirY(), &stack_14C);
                if (MR::isNearZero(stack_14C, 0.001f)) {
                    MR::vecKillElement(stack_17C, getCamDirZ(), &stack_14C);
                }

                MR::normalize(&stack_14C);
                if (stack_14C.dot(worldPadDir) < 0.0f && mFrontVec.dot(stack_14C) < 0.0f) {
                    mDrawStates_WORD |= 0x08000000;
                }
            }

            if ((mDrawStates_WORD & 0x08000000) == 0) {
                _8F0 += 0.3f * (slopeAngle / 90.0f);
            }

            if (_8F0 >= 10.0f) {
                TVec3f stack_158;
                if (MR::vecKillElement(mActor->getLastMove(), stack_17C, &stack_158) < 0.0f) {
                    _8F8 = mActor->getLastMove();
                } else {
                    _8F8.zero();
                }

                mMovementStates_HIGH_WORD |= 0x10000000;
                _8F0 = 10.0f;
                mVelocity.zero();
                mDrawStates_WORD &= ~0x08000000;
            } else if ((mDrawStates_WORD & 0x08000000) == 0 && sideSpeed > 0.0f) {
                const f32 t = _8F0 / 10.0f;
                const f32 remain = 1.0f - t;

                TVec3f stack_A4(stack_17C);
                stack_A4.scale(sideSpeed);
                TVec3f stack_B0(stack_A4);
                stack_B0.scale(remain);

                TVec3f stack_BC(stack_158);
                stack_BC += stack_B0;
                mVelocity = stack_BC;
            }

            blendRate *= 1.0f - (_8F0 / 10.0f);

            if ((mDrawStates_WORD & 0x08000000) != 0) {
                _3FE++;

                const u16 dashMaxTime = mActor->mConst->getTable()->mSlopeDashAccelTime;
                if (_3FE > dashMaxTime) {
                    _3FE = dashMaxTime;
                }

                _8F4 = (slopeAngle / mActor->mConst->getTable()->mSlopeDashAngleFactor) *
                       (static_cast<f32>(_3FE) / static_cast<f32>(dashMaxTime));
                _8F4 = 1.0f + (_8F4 * mActor->mConst->getTable()->mSlopeDashSpeedFactor);

                const f32 dashAccel = 1.0f + ((_8F4 - 1.0f) * mActor->mConst->getTable()->mSlopeDashAccelFactor);

                TVec3f stack_8C(-_904);
                TVec3f stack_98(stack_8C);
                stack_98.scale(dashAccel);
                mVelocity += stack_98;

                if (isAnimationRun(lbl_805CC3E0 + 0xB)) {
                    stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
                }
            }
        }
    }

    MR::vecKillElement(_8F8, *mGroundPolygon->getNormal(0), &_8F8);

    if ((mMovementStates_HIGH_WORD & 0x10000000) != 0) {
        _71E = 0;

        const f32 moveAngle = calcAngleD(_368);
        if ((mDrawStates_WORD & 0x00080000) != 0) {
            MR::vecKillElement(mVelocity, stack_17C, &mVelocity);
            mVelocity += _8F8;

            const f32 slopeAccel = mActor->mConst->getTable()->mSlopeAccel;
            TVec3f stack_68(-stack_17C);
            TVec3f stack_74(stack_68);
            stack_74.scale(slopeAngle);
            TVec3f stack_80(stack_74);
            stack_80.scale(slopeAccel);
            _8F8 += stack_80;
        } else {
            TVec3f stack_140(_8F8);
            if (!MR::normalizeOrZero(&stack_140)) {
                MR::vecKillElement(mVelocity, stack_140, &mVelocity);
            }

            mVelocity += _8F8;
        }

        if (MR::isNearZero(_8F8, 0.001f)) {
            if ((mDrawStates_WORD & 0x00080000) == 0) {
                mMovementStates_HIGH_WORD &= ~0x10000000;
                return;
            }
        }

        if (moveAngle > 0.0f) {
            TVec3f stack_5C(-stack_17C);
            TVec3f stack_134;
            MR::vecKillElement(stack_5C, getAirGravityVec(), &stack_134);

            if (MR::isNearZero(stack_17C, 0.001f) || (MR::isNearZero(stack_134, 0.001f) && (mDrawStates_WORD & 0x00080000) == 0)) {
                mMovementStates_HIGH_WORD &= ~0x10000000;
                return;
            }

            MR::isNearZero(stack_134, 0.001f);
            MR::normalize(&stack_134);

            const f32 speed = PSVECMag(&_8F8);
            MR::normalizeOrZero(&_8F8);

            const f32 ratio = MR::clamp(moveAngle / 30.0f, 0.0f, 1.0f);
            const f32 blend = ratio * mActor->mConst->getTable()->mSlopeCurveAssist;

            TVec3f stack_50(-stack_17C);
            MR::vecBlendSphere(_8F8, stack_50, &_8F8, blend);
            _8F8.setLength(speed);
        }

        if (MR::isNearZero(_8F8, 0.001f)) {
            mMovementStates_HIGH_WORD &= ~0x10000000;
            moveSlopeSlide();
            return;
        }

        if (_8F8.dot(stack_17C) <= 0.0f) {
            TVec3f stack_44(-*getGravityVec());
            if (stack_17C.dot(stack_44) > 0.05f) {
                TVec3f stack_128;
                MR::vecKillElement(mFrontVec, _368, &stack_128);
                MR::normalize(&stack_128);

                TVec3f stack_11C(_8F8);
                MR::normalize(&stack_11C);

                f32 turnDot = MR::cos(1.0471976f);
                if (isAnimationRun(lbl_805CC3E0 + 0x18, 2)) {
                    turnDot = MR::cos(1.4959966f);
                }

                if (isAnimationRun(lbl_805CC3E0 + 0x2F)) {
                    if (_910.dot(stack_17C) > 0.0f && PSVECMag(&_910) > 2.0f) {
                        _8F8 = _910;
                    }

                    _910.zero();
                }

                if (stack_128.dot(stack_17C) < turnDot) {
                    setFrontVecKeepUp(stack_11C, 0.1f);
                    if (!isAnimationRun(lbl_805CC3E0 + 0x18, 2)) {
                        changeAnimation(lbl_805CC3E0 + 0x3E, 3);
                    }

                    if (isStickOn() && _16C.dot(stack_11C) > 0.0f &&
                        MR::diffAngleAbsHorizontal(stack_11C, mFrontVec, *getGravityVec()) < 0.5235988f) {
                        setFrontVecKeepUpAngle(_16C, mActor->mConst->getTable()->mSlipMoveTurnAngleRad);
                    }
                } else {
                    if (!isAnimationRun(lbl_805CC3E0 + 0x3E, 3)) {
                        changeAnimation(lbl_805CC3E0 + 0x18, 2);
                    }

                    if (_3C2 < 5) {
                        TVec3f stack_38(-stack_11C);
                        setFrontVecKeepUp(stack_38, 0.5f);
                    } else {
                        TVec3f stack_2C(-stack_11C);
                        setFrontVecKeepUp(stack_2C, 0.1f);
                    }

                    if (_3C2 > 10 && isStickOn() && _16C.dot(stack_11C) > 0.0f) {
                        TVec3f stack_20(-stack_11C);
                        if (MR::diffAngleAbsHorizontal(stack_20, mFrontVec, *getGravityVec()) < 0.5235988f) {
                            TVec3f stack_14(-_16C);
                            setFrontVecKeepUpAngle(stack_14, mActor->mConst->getTable()->mSlipMoveTurnAngleRad);
                        }
                    }
                }
            } else if ((mDrawStates_WORD & 0x00080000) != 0) {
                if (!isAnimationRun(lbl_805CC3E0 + 0x2F)) {
                    _910 = _8F8;
                    _8F8.zero();
                }

                changeAnimation(lbl_805CC3E0 + 0x2F, static_cast<const char*>(nullptr));
            }
        }

        if (isStickOn() && !MR::isNearZero(stack_17C, 0.001f) && _3C4 == 0) {
            const TVec3f& worldPadDir = getWorldPadDir();

            TVec3f stack_110(stack_17C);
            MR::vecKillElement(stack_17C, getCamDirY(), &stack_17C);
            if (MR::isNearZero(stack_17C, 0.001f)) {
                MR::vecKillElement(stack_110, getCamDirZ(), &stack_17C);
            }

            MR::normalize(&stack_17C);
            if (stack_17C.dot(worldPadDir) > 0.15f) {
                playEffect(lbl_805CC3E0 + 0x55);
                _3D0 = mActor->mConst->getTable()->mTurnSlipTime;
            }
        }

        _1A8.x *= 0.9f;
        _1A8.y *= 0.9f;
        _1A8.z *= 0.9f;

        TVec3f stack_104;
        if (MR::vecKillElement(_1A8, stack_17C, &stack_104) >= 0.0f) {
            _1A8 = stack_104;
        }

        addVelocity(stack_104);
    } else {
        mVelocity += _8F8;

        if (isStickOn() && _3C4 == 0) {
            _8F8.scale(mActor->mConst->getTable()->mSlopeCancelInertia);
        } else {
            _8F8.scale(mActor->mConst->getTable()->mSlopeFinishInertia);
        }

        const MarioConstTable* table = mActor->mConst->getTable();
        if (PSVECMag(&_8F8) < table->mSlopeAnimeFinishSpeed) {
            if (isAnimationRun(lbl_805CC3E0 + 0x18, 2)) {
                changeAnimation(lbl_805CC3E0 + 0x68, static_cast<const char*>(nullptr));
            }

            if (isAnimationRun(lbl_805CC3E0 + 0x3E, 3)) {
                changeAnimation(lbl_805CC3E0 + 0x7B, static_cast<const char*>(nullptr));
            }

            if (isAnimationRun(lbl_805CC3E0 + 0x2F)) {
                stopAnimation(lbl_805CC3E0 + 0x2F, static_cast<const char*>(nullptr));
            }

            if (isStickOn() && _8F0 == 0.0f) {
                if (isAnimationRun(lbl_805CC3E0 + 0x68)) {
                    stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
                } else if (isAnimationRun(lbl_805CC3E0 + 0x7B)) {
                    stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
                }
            }
        }
    }

    const MarioConstTable* table = mActor->mConst->getTable();
    f32 slopeSpeedMax = table->mSlopeSpeedMax;
    if (_3D0 != 0) {
        slopeSpeedMax = table->mSlopeSpeedMaxBraking;
    }

    const f32 slopeSpeed = PSVECMag(&_8F8);
    if (slopeSpeed > slopeSpeedMax) {
        _8F8.setLength((0.8f * slopeSpeed) + (0.2f * slopeSpeedMax));
    }

    TVec3f stack_8(-_368);
    TVec3f stack_188;
    if (!MR::vecBlendSphere(*getGravityVec(), stack_8, &stack_188, blendRate)) {
        mMovementStates_HIGH_WORD &= ~0x10000000;
        tryDrop();
        return;
    }

    if ((mMovementStates_LOW_WORD & 0x00800000) != 0) {
        TVec3f stack_F8;
        if (MR::vecKillElement(mVelocity, mFrontVec, &stack_F8) > 0.0f) {
            mVelocity = stack_F8;
        }
    }

    moveSlopeSlide();
}

void Mario::taskOnSlipTurn(u32) {
    if (!isAnimationRun(lbl_805CC46E)) {
        TVec3f stack_8(-_220);
        setFrontVecKeepUp(stack_8);
        _754 = 0;
        _74C = 0.0f;
        return;
    }

    _2B8.scale(mActor->mConst->getTable()->mInertiaSlipTurn);
    addVelocity(_2B8);
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
