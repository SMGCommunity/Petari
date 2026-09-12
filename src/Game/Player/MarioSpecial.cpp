#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "revolution/types.h"

void Mario::checkOnimasu(const HitSensor* pSensor) {
    if (strstr(pSensor->mHost->mName, "オニマス") == nullptr) {
        return;
    }

    if (_5FC == nullptr) {
        _5FC = pSensor;
        _60C = 1;
    } else if (_5FC != pSensor && (pSensor->mPosition - mPosition).length() < (_5FC->mPosition - mPosition).length()) {
        _5FC = pSensor;
        _60C = 1;
    }
}

bool Mario::isDossun(const Triangle* pTriangle) const {
    if (!pTriangle->isValid()) {
        return false;
    }

    if (strstr(pTriangle->mSensor->mHost->getName(), "ドッスン") != nullptr) {
        return true;
    }

    return false;
}

bool Mario::isStageCameraRotate2D() const {
    return MR::isEqualStageName("HellProminenceGalaxy") && MR::getCurrentScenarioNo() == 3;
}

bool Mario::isNoWalkFallOnDossun() const {
    if (!MR::isEqualStageName("CannonFleetGalaxy")) {
        return false;
    }

    if (isDossun(mGroundPolygon) && isDossun(_45C)) {
        return true;
    }

    return false;
}

bool Mario::isNotReflectGlassGround() const {
    if (!MR::isEqualStageName("AstroGalaxy")) {
        return false;
    }

    TVec3f vec(1627.0f, 783.0f, -2152.0f);
    TVec3f v10 = mPosition - vec;

    f32 v4 = MR::diffAngleAbs(getCamDirZ(), v10);
    f32 v5 = v4;
    if (v4 >= 1.5707964f) {
        v5 = (PI - v4);
    }

    f32 v7 = (400.0f + (300.0f * (MR::sin(v5) * MR::sin(v5))));
    MR::vecKillElement(v10, getAirGravityVec(), &v10);
    f32 length = v10.length();

    if (length >= v7) {
        if (length <= 700.0f) {
            return true;
        }
    }

    return false;
}

bool Mario::isUseAnotherMovingPolygon() const {
    if (MR::isEqualStageName("BattleShipGalaxy")) {
        return true;
    }

    if (MR::isEqualStageName("SandClockGalaxy")) {
        return true;
    }

    return MR::isEqualStageName("TriLegLv1Galaxy");
}

bool Mario::isUseFooSpecialGravity(const TVec3f& a1, TVec3f* a2) const {
    if (!isStatusActive(MarioStatus_Foo)) {
        return false;
    }

    if (MR::isEqualStageName("HeavensDoorGalaxy") && MR::getCurrentScenarioNo() == 2) {
        TVec3f vec(14760.0f, -10676.2f, 6770.0f);
        TVec3f res = vec - a1;
        a2->set(res);
        MR::normalizeOrZero(a2);
        return true;
    }

    return false;
}

void Mario::updateOnimasu() {
    if (!_5FC) {
        return;
    }
    if ((_5FC->mPosition - mPosition).length() >= 1000.0f) {
        _5FC = nullptr;
        return;
    }

    const HitSensor* sensor = _5FC;
    MtxPtr inverse = sensor->mHost->mCollisionParts->mInvBaseMatrix;
    TVec3f localPosition;
    TVec3f previousPosition;
    previousPosition = _600;
    PSMTXMultVec(inverse, &mActor->mPosition, &localPosition);
    if (_60C) {
        _600 = localPosition;
        _60C = 0;
        return;
    }

    if (!_10._1D) {
        if (previousPosition.y < -400.0f && localPosition.y > -400.0f) {
            if (MR::isInRange(localPosition.x, -400.0f, 400.0f) && MR::isInRange(localPosition.z, -400.0f, 400.0f)) {
                localPosition = _600;
                TVec3f position;
                PSMTXMultVec(sensor->mHost->mCollisionParts->mBaseMatrix, &localPosition, &position);
                setTrans(position, nullptr);
                mVelocity.zero();
            }
        } else if (previousPosition.y > 400.0f && localPosition.y <= 400.0f) {
            if (MR::isInRange(localPosition.x, -400.0f, 400.0f) && MR::isInRange(localPosition.z, -400.0f, 400.0f)) {
                _10._1D = true;
                if (previousPosition.x < -320.0f) {
                    previousPosition.x = -320.0f;
                } else if (previousPosition.x > 320.0f) {
                    previousPosition.x = 320.0f;
                }
                if (previousPosition.z < -320.0f) {
                    previousPosition.z = -320.0f;
                } else if (previousPosition.z > 320.0f) {
                    previousPosition.z = 320.0f;
                }
                localPosition = previousPosition;
            }
        } else if (MR::isInRange(localPosition.x, -400.0f, 400.0f) && MR::isInRange(localPosition.y, -400.0f, 400.0f) &&
                   MR::isInRange(localPosition.z, -400.0f, 400.0f)) {
            mVelocity.zero();
            localPosition = _600;
            TVec3f position;
            PSMTXMultVec(sensor->mHost->mCollisionParts->mBaseMatrix, &localPosition, &position);
            TVec3f displacement(position - mPosition);
            f32 height = 10.0f + mVerticalSpeed;
            if (-displacement.dot(getShadowNorm()) > height) {
                MR::vecKillElement(displacement, getShadowNorm(), &displacement);
                displacement += -getShadowNorm() * mVerticalSpeed;
                position = mPosition + displacement;
            }
            setTrans(position, nullptr);
        }

        bool correctX = false;
        bool correctY = false;
        bool correctZ = false;
        TVec3f axisX;
        TVec3f axisY;
        TVec3f axisZ;
        sensor->mHost->mCollisionParts->mBaseMatrix.getXDir(axisX);
        sensor->mHost->mCollisionParts->mBaseMatrix.getYDir(axisY);
        sensor->mHost->mCollisionParts->mBaseMatrix.getZDir(axisZ);
        f32 gravityX = axisX.dot(*getGravityVec());
        f32 absGravityX = MR::abs(gravityX);
        f32 gravityY = axisY.dot(*getGravityVec());
        f32 absGravityY = MR::abs(gravityY);
        f32 gravityZ = axisZ.dot(*getGravityVec());
        f32 absGravityZ = MR::abs(gravityZ);
        f32 lower = -500.0f;
        f32 upper = -300.0f;

        if (absGravityX > absGravityY && absGravityX > absGravityZ) {
            if (gravityX > 0.0f) {
                lower = 300.0f;
                upper = 500.0f;
            }
            if (MR::isInRange(localPosition.x, lower, upper)) {
                bool insideY = MR::isInRange(localPosition.y, -320.0f, 320.0f);
                bool insideZ = MR::isInRange(localPosition.z, -320.0f, 320.0f);
                if (insideY && insideZ) {
                    mActor->_3B4 = axisX;
                    mActor->setPress(0, 0);
                } else {
                    if (insideZ) {
                        correctY = true;
                    }
                    if (insideY) {
                        correctZ = true;
                    }
                }
            }
        } else if (absGravityY > absGravityX && absGravityY > absGravityZ) {
            if (gravityY > 0.0f) {
                lower = 300.0f;
                upper = 500.0f;
            }
            if (MR::isInRange(localPosition.y, lower, upper)) {
                bool insideX = MR::isInRange(localPosition.x, -320.0f, 320.0f);
                bool insideZ = MR::isInRange(localPosition.z, -320.0f, 320.0f);
                if (insideX && insideZ) {
                    mActor->_3B4 = axisY;
                    if (gravityY < 0.0f) {
                        mActor->setPress(0, 0);
                    }
                } else {
                    if (insideZ) {
                        correctX = true;
                    }
                    if (insideX) {
                        correctZ = true;
                    }
                }
            }
        } else {
            if (gravityZ > 0.0f) {
                lower = 300.0f;
                upper = 500.0f;
            }
            if (MR::isInRange(localPosition.z, lower, upper)) {
                bool insideX = MR::isInRange(localPosition.x, -320.0f, 320.0f);
                bool insideY = MR::isInRange(localPosition.y, -320.0f, 320.0f);
                if (insideX && insideY) {
                    mActor->_3B4 = axisZ;
                    mActor->setPress(0, 0);
                } else {
                    if (insideY) {
                        correctX = true;
                    }
                    if (insideX) {
                        correctY = true;
                    }
                }
            }
        }

        if (correctX) {
            if (localPosition.x > -400.0f && localPosition.x < -320.0f) {
                localPosition.x = -400.0f;
            } else if (localPosition.x < 400.0f && localPosition.x > 320.0f) {
                localPosition.x = 400.0f;
            }
        }
        if (correctY) {
            if (localPosition.y > -400.0f && localPosition.y < -320.0f) {
                localPosition.y = -400.0f;
            } else if (localPosition.y < 400.0f && localPosition.y > 320.0f) {
                localPosition.y = 400.0f;
            }
        }
        if (correctZ) {
            if (localPosition.z > -400.0f && localPosition.z < -320.0f) {
                localPosition.z = -400.0f;
            } else if (localPosition.z < 400.0f && localPosition.z > 320.0f) {
                localPosition.z = 400.0f;
            }
        }
        if (correctX | correctY | correctZ) {
            TVec3f position;
            PSMTXMultVec(sensor->mHost->mCollisionParts->mBaseMatrix, &localPosition, &position);
            if (mMovementStates._8 || mMovementStates._19 || mMovementStates._1A) {
                TVec3f displacement(position - mPosition);
                if (getWallNorm().dot(displacement) < 0.0f) {
                    addVelocity(-displacement);
                } else {
                    setTrans(position, nullptr);
                }
            }
        }
    } else if (localPosition.y > 400.0f) {
        _10._1D = false;
    } else {
        bool corrected = false;
        if (previousPosition.y >= -320.0f && localPosition.y < -320.0f) {
            localPosition.y = previousPosition.y;
            corrected = true;
        }
        if (previousPosition.x >= -320.0f && localPosition.x < -320.0f) {
            localPosition.x = previousPosition.x;
            corrected = true;
        }
        if (previousPosition.x <= 320.0f && localPosition.x > 320.0f) {
            localPosition.x = previousPosition.x;
            corrected = true;
        }
        if (previousPosition.z >= -320.0f && localPosition.z < -320.0f) {
            localPosition.z = previousPosition.z;
            corrected = true;
        }
        if (previousPosition.z <= 320.0f && localPosition.z > 320.0f) {
            localPosition.z = previousPosition.z;
            corrected = true;
        }
        if (corrected) {
            TVec3f position;
            PSMTXMultVec(sensor->mHost->mCollisionParts->mBaseMatrix, &_600, &position);
            setTrans(position, nullptr);
            localPosition = _600;
        }
    }
    _600 = localPosition;
}

bool Mario::isHeadPushEnableArea() const {
    if (!mMovementStates._17) {
        return false;
    }

    if (MR::isEqualStageName("SandClockGalaxy")) {
        if (MR::isInRange(mPosition.x, -7020.0f, -6920.0f)) {
            return true;
        }
    }

    return false;
}

bool Mario::isOnimasuBinderPressSkip() const {
    if (isStatusActive(MarioStatus_SideStep)) {
        if (mFrontWallTriangle->mSensor != nullptr) {
            if (strstr(mFrontWallTriangle->mSensor->mHost->mName, "オニマス")) {
                return true;
            }
        }
    }

    return false;
}
