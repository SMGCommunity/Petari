#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioSkate.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

void FORCE_INLINE() {
    TVec3f vec;
    TVec3f vec2;
    vec - vec2;
    vec.setLength(1.0f);
}

//required (for now) to match the inlined isBeeWallWalk calls
#pragma opt_propagation off

void Mario::mainMove() {
    TVec3f vec1;
    MR::setNan(vec1);

    bool a1 = false;

    if (mDrawStates._5) {
        a1 = true;
    }

    if (mMovementStates._23) {
        mTargetWalkSpeedIndex = 0;
        mWalkSpeed = 0.0f;
        a1 = true;

        playSound("坂滑り");
    }

    if (_754) {
        a1 = true;
    }

    mMovementStates._35 = false;

    if (isSkatableFloor()) {
        mMovementStates._35 = true;
    }

    if (_10._1A) {
        tryJump();
        beforeJumping2D();

        if (isAnimationRun("その場足踏み上半身")) {
            stopAnimationUpper(nullptr, nullptr);
        }

        _420 = 0;
        mJumpVec = _184;
        _10._1A = false;
        return;
    }

    bool a2 = false;
    bool isRequestJump = mActor->isRequestJump();

    if (!isRequestJump && mActor->isRequestJump2P() && _3C8 == 0) {
        _3C8 = 6;
    }

    if (!isRequestJump) {
        if (_3C8 != 0) {
            _3C8--;
            if (_3C8 == 0) {
                isRequestJump = true;
                _1C._A = true;
            }
        }
    } else if (_3C8 != 0) {
        _1C._B = true;
        _3C8 = 0;
    }

    if (isRequestJump || mMovementStates._38) {
        if (mMovementStates._A && calcDistToCeil(false) < 160.0f) {
            mActor->sendMsgUpperPunch(_730);
            changeAnimation("しゃがみアッパー", static_cast< const char* >(nullptr));
        } else if (isStatusActive(MarioStatus_Skate)) {
            mSkate->exitJump();
            closeStatus(mSkate);
        } else {
            a2 = true;
        }
    }

    if (_436 != 0) {
        _436--;
        if (a2 && checkSquat(false)) {
            if (mStickPos.z >= 0.5f) {
                a2 = false;
                _436 = 0;
                mWalkSpeed = 5.0f;
                _434 = 180;

                playSound("スペシャルダッシュ強");
                playSound("ダッシュ加速強成功");
                playSound("声物ジャンプ");
            }
        } else if (a2 && mStickPos.z >= 0.5f) {
            a2 = false;
            _436 = 0;
            mWalkSpeed = 2.0f;
            _434 = 60;
            playSound("スペシャルダッシュ弱");
            playSound("声物ジャンプ");
        }
    }

    if (a2) {
        saveLastSafetyTrans();
        tryJump();
        beforeJumping2D();

        if (isAnimationRun("その場足踏み上半身")) {
            stopAnimationUpper(nullptr, nullptr);
        }

        _420 = 0;
        return;
    }

    if (!mMovementStates._1) {
        if (_3C0 != 0 && !mMovementStates._23 && MR::isNearZero(_8F8)) {
            _3C0--;
        } else if (!mActor->_EA4) {
            tryDrop();
            beforeJumping2D();
            _420 = 0;
            return;
        }
    } else if (mActor->_1C8 < mActor->getConst().getTable()->mWallBackHangStickPower) {
        _3C0 = 0;
    } else if (mDrawStates._A) {
        _3C0 = 0;
    } else if (_3C0 < mActor->getConst().getTable()->mDropWaitTime) {
        _3C0 = mActor->getConst().getTable()->mDropWaitTime;
    }

    if (_420 != 0) {
        _420--;
        _10._28 = true;
        return;
    }

    checkTornado();

    getAnimator()->setWalkMode();

    if (MR::isNearZero(mStickPos.x) && MR::isNearZero(mStickPos.y)) {
        _328 = _334;

        if (MR::isNearZero(_328)) {
            _328 = mFrontVec;
        }

        vec1 = _328;

        calcShadowDir(_328, &_22C);
        _40C = 10;
    } else if (!(mTargetWalkSpeedIndex != 0 && _71D) && (!isAnimationRun("ブレーキ") && !isAnimationRun("ターンブレーキ") && !mMovementStates._10 &&
                                                         !mMovementStates._F && mWalkSpeed < 0.05f && !mDrawStates._5)) {
        bool a3 = true;
        if (_40C != 0) {
            if (mStickPos.z < 0.75f) {
                a3 = false;
                _40C--;
                mStickPos.set(0.0f);
            } else {
                _40C = 0;
            }
        }

        if (a3) {
            calcMoveDir(mStickPos.x, mStickPos.y, &vec1, true);
            calcShadowDir(vec1, &_22C);
            calcShadowDir(mFrontVec, &_214);

            f32 angle = MR::pi() * 10.0f / 180.0f;
            f32 f1;
            if (mMovementStates._37) {
                f1 = MR::diffAngleAbsHorizontal(_214, _22C, _6A0);
                angle = MR::pi() * 90.0f / 180.0f;
            } else {
                f1 = MR::diffAngleAbsHorizontal(_214, _22C, *getGravityVec());
            }

            if (f1 > angle) {
                mDrawStates._E = true;
                u32 turnTime = mActor->getConst().getTable()->mStandingTurnTime;
                if (mActor->isBeeWallWalk()) {
                    turnTime = 30;
                }

                if (!a1) {
                    setFrontVecKeepUp(_22C, turnTime);
                }

                if (!isEnableTurn()) {
                    return;
                }

                changeAnimation("その場足踏み", static_cast< const char* >(nullptr));

                if (isAnimationRun("カリカリ限界")) {
                    return;
                }

                changeAnimationUpperWeak("その場足踏み上半身", nullptr);
                return;
            } else if (!a1) {
                setFrontVecKeepUp(_22C);
            }
        } else {
            vec1 = _328;
        }
    } else {
        f32 stickposX = mStickPos.x;
        f32 stickposY = mStickPos.y;

        if (!getPlayer()->_10._11) {
            if (MR::abs(stickposY) > mActor->getConst().getTable()->mStickMarginYstart) {
                if (MR::abs(stickposX) < mActor->getConst().getTable()->mStickMarginX) {
                    stickposX = 0.0f;
                } else if (stickposX > 0.0f) {
                    stickposX = (stickposX - mActor->getConst().getTable()->mStickMarginX) / (1.0f - mActor->getConst().getTable()->mStickMarginX);
                } else {
                    stickposX = (stickposX + mActor->getConst().getTable()->mStickMarginX) / (1.0f - mActor->getConst().getTable()->mStickMarginX);
                }
            } else if (MR::abs(stickposX) > mActor->getConst().getTable()->mStickMarginXstart) {
                if (MR::abs(stickposY) < mActor->getConst().getTable()->mStickMarginY) {
                    stickposY = 0.0f;
                } else if (stickposY > 0.0f) {
                    stickposY = (stickposY - mActor->getConst().getTable()->mStickMarginY) / (1.0f - mActor->getConst().getTable()->mStickMarginY);
                } else {
                    stickposY = (stickposY + mActor->getConst().getTable()->mStickMarginY) / (1.0f - mActor->getConst().getTable()->mStickMarginY);
                }
            }
        }

        calcMoveDir(stickposX, stickposY, &vec1, true);

        calcShadowDir(vec1, &_22C);
    }

    calcShadowDir(mFrontVec, &_214);

    f32 dot = _214.dot(_22C);
    f32 dot2 = dot;

    if (_3D2 != 0) {
        dot2 = _3E4.dot(_22C);
    }

    if (dot >= MR::cos(mActor->getConst().getTable()->mTurnSlipAngle) && !mMovementStates._4 &&
        mWalkSpeed > mActor->getConst().getTable()->mSlipSpeed && _38 > mStickPos.z && !mDrawStates._D) {
        recordTurnSlipAngle();
    } else if (_3D2 != 0) {
        _3D2--;
    }

    if (mMovementStates._F || mMovementStates._34) {
        _3D2 = 0;
        mMovementStates._10 = false;
    }

    if (_3D2 != 0) {
        if (mStickPos.z < mActor->getConst().getTable()->mTurnSlipNeutral) {
            mMovementStates._10 = true;
        }

        if (mWalkSpeed < 0.1f) {
            _3D2 = 0;
            mWalkSpeed = 0.0f;
        }

        if (isAnimationRun("ブレーキ")) {
            a1 = true;
        }

        if (mMovementStates._10) {
            a1 = true;
        }
    } else {
        if (!mMovementStates._34 && !mMovementStates._A && mMovementStates._10 == true && !mMovementStates._F && !mMovementStates._23 &&
            !isStatusActive(MarioStatus_Skate) && dot2 < MR::cos(mActor->getConst().getTable()->mTurnSlipAngle) && !mMovementStates._4) {
            if (mMovementStates._35) {
                a1 = true;
            }

            _220 = _3E4;

            if (!a1) {
                setFrontVecKeepUp(-_220);
            }

            if (mMovementStates._35) {
                _3D0 = mActor->getConst().getTable()->mTurnSlipTimeB;
                mMovementStates._4 = true;
                changeAnimation("ターンブレーキ滑り床", static_cast< const char* >(nullptr));
                _2B8 = mActor->getLastMove();
                stopWalk();
                _754 = 10;
                pushTask(&taskOnSlipTurn, 1);
            } else {
                _3D0 = mActor->getConst().getTable()->mTurnSlipTime;
                mMovementStates._4 = true;
                mWalkSpeed = 0.0f;
                changeAnimation("ターンブレーキ", static_cast< const char* >(nullptr));
                playEffect("共通ブレーキ");
            }
        }

        mMovementStates._10 = false;
    }

    if (_3D0 != 0) {
        if (!mMovementStates._35) {
            playSound("スリップ");
        }

        if (_3D0 == static_cast< u16 >(mActor->getConst().getTable()->mTurnSlipTime)) {
            startPadVib(2);
        }

        _3D0--;

        if (!mMovementStates._35) {
            mWalkSpeed = 0.0f;
        }
    }

    if (_3D0 == 0) {
        if (mMovementStates._4) {
            TVec3f* frontVec = &mFrontVec;
            if (MR::diffAngleAbsHorizontal(getWorldPadDir(), *frontVec, *getGravityVec()) > MR::pi() * 90.0f / 180.0f) {
                stopAnimation("ターンブレーキ");
                setFrontVecKeepUp(-*frontVec);
                mMovementStates._4 = false;
            }

            if (isAnimationTerminate("ターンブレーキ")) {
                stopAnimation("ターンブレーキ");
                mMovementStates._4 = false;

                if (mStickPos.z < 0.1f) {
                    stopWalk();
                }
            }
        }

        // needs to be written as two nested if statements to match for some reason
        if (isActiveTask(&taskOnSlipTurn)) {
            if (isAnimationRun("ターンブレーキ滑り床")) {
                setFrontVecKeepUp(-_220);
                a1 = true;

                stopAnimation("ターンブレーキ滑り床");
                _754 = 0;
                _74C = 0.0f;
                mWalkSpeed = 0.0f;
                popTask(&taskOnSlipTurn);
            }
        }
    }

    f32 turnAngleSpeed = mActor->getConst().getTable()->mTurnAngleSpeed;

    if (mWalkSpeed > mActor->getConst().getTable()->mFastTurnSpeed) {
        turnAngleSpeed = mActor->getConst().getTable()->mTurnAngleSpeed2;
        if (mTargetWalkSpeedIndex < 5) {
            turnAngleSpeed = mActor->getConst().getTable()->mTurnAngleSpeedSlowWalk;
        }

        if (isStatusActive(MarioStatus_Skate)) {
            turnAngleSpeed *= 0.5f;
        }
    }

    if (mMovementStates._23) {
        turnAngleSpeed = mActor->getConst().getTable()->mTurnAngleSpeed3;
    }

    if (mMovementStates._F) {
        turnAngleSpeed = mActor->getConst().getTable()->mTurnAngleSpeedTornado;
    }

    bool a4;

    if (!mMovementStates._4) {
        TVec3f cross(_238.cross(_22C));
        TVec3f stick;
        mActor->getStickValue(&stick.x, &stick.y);
        stick.z = 0.0f;
        cross.cross(_250, stick);

        a4 = true;

        if (mMovementStates._37) {
            a4 = false;
        }

        if (MR::isNearZero(mStickPos.z)) {
            _10._C = false;
            mMovementStates.turning = false;
        } else if (MR::isNearZero(_244) && a4) {
            if (_250 != stick) {
                mMovementStates.turning = true;
            }

        } else if (cross.dot(_244) > 0.0f && a4) {
            mMovementStates.turning = true;
        } else {
            if (_3D0 != 0) {
                _10._C = false;
                mMovementStates.turning = false;
            }

            if (MR::isInRange(MR::diffAngleAbs(_214, _22C), 0.0f, 0.707f)) {
                TVec3f cross2(_214.cross((_22C)));

                if (_274 != 0) {
                    if (cross2.dot(mHeadVec) > 0.0f) {
                        _10._C = false;
                    }
                } else if (cross2.dot(mHeadVec) < 0.0f) {
                    _10._C = false;
                }
            }
        }

        _250 = stick;

        if (mMovementStates.turning) {
            TVec3f cross3(_214.cross((_22C)));

            if (cross3.dot(_3D8) < 0.0f) {
                if (!(cross.dot(_244) > 0.0f) && !(_214.dot(_22C) < 0.0f)) {
                    _3D4 = 0;
                }
            } else if (_3D4 < mActor->getConst().getTable()->mWeakTurnTime) {
                _3D4++;
            }

            _3D8 = cross3;
            _244 = cross;
            _238 = _22C;

            f32 f1 = 1.0f;

            if (_3D4 < mActor->getConst().getTable()->mWeakTurnTime && mTargetWalkSpeedIndex > 4) {
                f1 = static_cast< f32 >(_3D4) / static_cast< f32 >(mActor->getConst().getTable()->mWeakTurnTime);
            }

            turnAngleSpeed *= MR::sqrt(f1);
        } else {
            if (_3D4 != 0) {
                _3D4--;
            }

            if (_3D4 == 0) {
                turnAngleSpeed *= 0.01f;
                _244.zero();
                _3D8.cross(_214, _22C);
            }

            _238 = _22C;
        }

        bool a5 = true;

        if (mDrawStates._C) {
            if (MR::diffAngleAbsHorizontal(_8F8, -mFrontVec, *getGravityVec()) < MR::pi() * 45.0f / 180.0f) {
                turnAngleSpeed *= 0.1f;
            } else {
                turnAngleSpeed *= 0.3f;
            }
        }

        TVec3f vec2;

        if (a5) {
            if (mMovementStates._37) {
                _40E = 0;
                mDrawStates._D = false;
                _10._C = false;
                vec2 = _22C;
            } else if (_10._C) {
                f32 angle = 0.12f;
                if (_274 == 0) {
                    angle = -angle;
                }
                Mtx mtx;
                PSMTXRotAxisRad(mtx, mHeadVec, angle);
                PSMTXMultVecSR(mtx, _214, &vec2);
            } else if (MR::diffAngleAbs(mFrontVec, _22C) < 0.1f) {
                if (_524 == _528) {
                    vec2 = _22C;
                } else {
                    vec2 = mFrontVec;
                }
            } else {
                f32 diffAngleAbs = MR::diffAngleAbs(mFrontVec, _22C);

                if (diffAngleAbs > 0.0f) {
                    f32 f1 = turnAngleSpeed / diffAngleAbs;
                    MR::clamp01(&f1);

                    if (mActor->isBeeWallWalk() && f1 > 0.1f) {
                        f1 = 0.1f;
                    }

                    if (getPlayer()->_10._12) {
                        f32 ratio = mActor->getConst().getTable()->mStickHeavyMinRatio;
                        f32 minAngle = mActor->getConst().getTable()->mStickHeavyMinAngle;
                        if (diffAngleAbs >= mActor->getConst().getTable()->mStickHeavyMaxAngle) {
                            ratio = 1.0f;
                        } else if (diffAngleAbs > minAngle) {
                            ratio += (1.0f - ratio) * ((diffAngleAbs - minAngle) / (mActor->getConst().getTable()->mStickHeavyMaxAngle - minAngle));
                        }

                        f1 *= ratio;

                        f32 f2 = 1.0f - ((MR::pi() - diffAngleAbs) / MR::pi());
                        MR::clamp01(&f2);

                        mWalkSpeed *= 1.0f - 0.1f * f2;
                    }

                    if (!MR::vecBlendSphere(mFrontVec, _22C, &vec2, f1)) {
                        _3D4 = 0;
                        MR::vecRotAxis(_214, _22C, mHeadVec, &vec2, 22.5f * MR::pi() / 180.0f);
                    }

                    if (diffAngleAbs > 2.5f && _3D4 == mActor->getConst().getTable()->mWeakTurnTime && !mDrawStates._D) {
                        TVec3f cross3(_214.cross(vec2));
                        _274 = cross3.dot(mHeadVec) > 0.0f;
                        _10._C = true;
                    }
                }
            }

            mMovementStates.turning = true;

            if (!isAnimationRun("その場足踏み") && dot > 0.99f) {
                mMovementStates.turning = false;
            }

            if (mMovementStates._37 && mFrontVec.dot(vec2) < 0.0f && !isAnimationRun("ブレーキ")) {
                _750 = 10;
                _74C = MR::pi();
            }

            if (!a1) {
                setFrontVecKeepUp(vec2);
            }
        }
    } else {
        if (_3D0 == 0) {
            TVec3f blendSphere;
            if (!MR::vecBlendSphere(_214, _22C, &blendSphere, turnAngleSpeed)) {
                MR::vecRotAxis(_214, _22C, mHeadVec, &blendSphere, 22.5f * MR::pi() / 180.0f);
            }

            if (!a1) {
                setFrontVecKeepUp(blendSphere);
            }
        }

        _10._C = false;
        mMovementStates.turning = false;
    }

    if (_750 != 0 && mTargetWalkSpeedIndex == 0 && isEnableTurn()) {
        changeAnimation("その場足踏み", static_cast< const char* >(nullptr));

        if (!isAnimationRun("カリカリ限界")) {
            changeAnimationUpperWeak("その場足踏み上半身", nullptr);
        }
    } else {
        if (mTargetWalkSpeedIndex != 0) {
            stopAnimation("その場足踏み");
        }

        if (isAnimationRun("その場足踏み上半身")) {
            stopAnimationUpper(nullptr, nullptr);
        }
    }

    TVec3f vec2;

    if (mMovementStates._34) {
        TVec3f vec3(mFrontVec);
        TVec3f vec4(mFrontVec);
        if (!MR::isNearZero(_16C)) {
            vec4 = _16C;
        }

        MR::normalize(&vec4);

        f32 f1 = (1.1f - mWalkSpeed) * mActor->getConst().getTable()->mInertiaIceTurn;

        MR::clamp01(&f1);

        MR::vecBlendSphere(vec4, vec3, &vec2, f1);

        MR::normalize(&vec2);

        vec2.scale(mWalkSpeed * mActor->getConst().getTable()->mWalkSpeed);

    } else if (mDrawStates._5) {
        vec2 = _22C * mActor->getConst().getTable()->mWalkSpeed * mWalkSpeed;
    } else {
        vec2 = mFrontVec * mActor->getConst().getTable()->mWalkSpeed * mWalkSpeed;
    }

    addVelocity(vec2);

    _328 = vec1;

    if (mMovementStates._A) {
        if (mTargetWalkSpeedIndex != 0) {
            _334 = vec1;
        }
    } else if (_3D2 == 0) {
        _334 = vec1;
    }

    if (mMovementStates._32 && _4E0 > 120.0f) {
        if (mVelocity.dot(*mFrontWallTriangle->getNormal(0)) < 0.0f) {
            TVec3f vec4;
            MR::vecKillElement(mPosition - _4E8, *mFrontWallTriangle->getNormal(0), &vec4);
            MR::vecKillElement(vec4, *getGravityVec(), &vec4);
            MR::vecKillElement(vec4, mFrontVec, &vec4);
            MR::normalizeOrZero(&vec4);

            if (!MR::isNearZero(vec4)) {
                mVelocity = vec4 * mVelocity.length();
            }
        }
    } else if (mMovementStates._8 && mVelocity.dot(*mFrontWallTriangle->getNormal(0)) < 0.0f) {
        f32 pushAngle = -mFrontVec.dot(*mFrontWallTriangle->getNormal(0));
        if (pushAngle < MR::cosDegree(mActor->getConst().getTable()->mWallPushAngleRange)) {
            MR::vecKillElement(mVelocity, *mFrontWallTriangle->getNormal(0), &mVelocity);
        }
    }

    TVec3f newVelocity(mVelocity);
    f32 speed = newVelocity.length();

    MR::normalizeOrZero(&newVelocity);

    TVec3f cross4(_368.cross(newVelocity));
    MR::normalizeOrZero(&cross4);

    newVelocity.cross(cross4, _368);
    newVelocity.setLength(speed);

    mVelocity = newVelocity;
}
