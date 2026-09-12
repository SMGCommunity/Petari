#include <revolution/types.h>

f32 JMAAcosRadian(f32) NO_INLINE;

#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioHang.hpp"
#include "Game/Player/MarioMapCode.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

bool Mario::checkPressDamage() {
    if (_5FC) {
        return false;
    }
    if (isSwimming()) {
        return false;
    }
    if (checkVerticalPress(false)) {
        if (isStatusActive(MarioStatus_Hang)) {
            mHang->forceDrop();
        } else {
            mActor->setPress(0, 0);
            if (mActor->_390) {
                return true;
            }
            _350.zero();
            _35C.zero();
            return true;
        }
    }
    if (checkSidePressPre()) {
        mActor->setPress(1, 0);
        return true;
    }
    if (checkSidePress()) {
        return true;
    }
    if (checkWallFloorCode(0x1D) && mMovementStates._1 && getFloorCode() != 0x1D) {
        mActor->_3B4 = _368;
        mActor->setPress(4, 120);
        return true;
    }
    if (_72C < 200.0f) {
        if (mMovementStates._8 && (!mMovementStates.jumping || !(getFrontWallNorm().dot(getAirGravityVec()) > 0.707f))) {
            if (_72C < 100.0f) {
                _72C = 100.0f;
            }
            addVelocity(getFrontWallNorm() * (_4E4 * (200.0f / _72C) - _4E4));
        }
        if (mMovementStates._1A && (!mMovementStates.jumping || !(getSideWallNorm().dot(getAirGravityVec()) > 0.707f))) {
            if (_72C < 100.0f) {
                _72C = 100.0f;
            }
            addVelocity(getSideWallNorm() * (60.0f * (200.0f / _72C) - 60.0f));
        }
    }
    return false;
}

bool Mario::checkVerticalPress(bool atCurrentPosition) {
    if (mMovementStates.debugMode) {
        return false;
    }
    TVec3f position;
    if (isStatusActive(MarioStatus_Hang)) {
        position = mActor->_2A0;
    } else {
        position = mPosition;
    }
    if (!atCurrentPosition && !mActor->_390) {
        position += mVelocity;
    }
    f32 height;
    if (mMovementStates._A) {
        height = 80.0f;
    } else {
        height = 160.0f;
    }
    bool nearCeiling = false;
    TVec3f up(-*getGravityVec());
    f32 distance = 0.0f;
    Triangle ceiling;
    Triangle floor;
    TVec3f hitPosition;
    bool hit = MR::getFirstPolyOnLineToMap(&hitPosition, &ceiling, position - up * 10.0f, up * height);
    if (hit) {
        distance = up.dot(hitPosition - position);
        if (distance < 150.0f) {
            nearCeiling = true;
        }
    }
    if (hit) {
        TVec3f origin;
        if (nearCeiling) {
            origin = hitPosition;
        } else {
            origin = position;
        }
        if (MR::getFirstPolyOnLineToMap(&hitPosition, &floor, origin - *ceiling.getNormal(0) * 10.0f, *ceiling.getNormal(0) * height)) {
            if (ceiling.mSensor != floor.mSensor && (MR::isSensorPressObj(ceiling.mSensor) || MR::isSensorPressObj(floor.mSensor))) {
                if (!atCurrentPosition && !mActor->_390) {
                    if (!checkVerticalPress(true)) {
                        mVelocity.zero();
                        return false;
                    }
                    return true;
                }
                mActor->_3B4 = _368;
                return true;
            }
        } else if (!atCurrentPosition && MR::isSensorPressObj(ceiling.mSensor)) {
            mDrawStates._1E = true;
            TVec3f displacement;
            if (distance < 0.0f) {
                displacement = hitPosition - position;
                displacement.setLength(height - distance);
            } else {
                displacement = position - hitPosition;
                displacement.setLength(150.0f - distance);
            }
            TVec3f pushDirection;
            bool blended = MR::vecBlendSphere(*ceiling.getNormal(0), *floor.getNormal(0), &pushDirection, 0.5f);
            if (!mActor->_390) {
                if (mMovementStates._1) {
                    if (mGroundPolygon->mSensor == ceiling.mSensor) {
                        if (blended) {
                            push(pushDirection * displacement.length());
                        }
                    } else {
                        mActor->_F48 = ceiling.mSensor;
                        mActor->_3B4 = _368;
                        return true;
                    }
                } else if (blended) {
                    push(pushDirection * displacement.length());
                }
            }
        }
    }
    return false;
}

bool Mario::checkSidePressPre() {
    if (_5FC) {
        return false;
    }
    if (mMovementStates._8 && mMovementStates._19) {
        if ((_4F4 - _4E8).length() < 100.0f && mFrontWallTriangle->mSensor != mBackWallTriangle->mSensor &&
            (MR::isSensorPressObj(mFrontWallTriangle->mSensor) || MR::isSensorPressObj(mBackWallTriangle->mSensor))) {
            if (isDossun(mFrontWallTriangle) || isDossun(mBackWallTriangle)) {
                return false;
            }
            if (mFrontWallTriangle->getNormal(0)->dot(*mBackWallTriangle->getNormal(0)) < -0.5f) {
                mActor->_3B4 = getWallNorm();
                return true;
            }
        }
    } else if (isStatusActive(MarioStatus_Hang) && mMovementStates._19 && _8C8->isValid() && _8C8->mSensor != mBackWallTriangle->mSensor &&
               (MR::isSensorPressObj(_8C8->mSensor) || MR::isSensorPressObj(mBackWallTriangle->mSensor))) {
        if (isDossun(_8C8) || isDossun(mBackWallTriangle)) {
            return false;
        }
        TVec3f horizontal;
        MR::vecKillElement(mPosition - _4F4, getAirGravityVec(), &horizontal);
        if (horizontal.length() < 100.0f) {
            mActor->_3B4 = getWallNorm();
            return true;
        }
    }
    return false;
}

bool Mario::checkSidePress() {
    TVec3f position;
    if (isStatusActive(MarioStatus_Hang)) {
        position = mActor->_2A0;
    } else {
        position = mPosition;
    }
    f32 radius;
    f32 height;
    if (mMovementStates._A) {
        radius = 50.0f;
        height = 40.0f;
    } else {
        radius = 80.0f;
        height = 70.0f;
    }
    f32 ceilingDistance = calcDistToCeil(false);
    TVec3f center(position + mHeadVec * height);
    u32 hitCount = Collision::checkStrikeBallToMap(center, radius, nullptr, nullptr);
    if (hitCount < 2) {
        return false;
    }
    for (u32 i = 0; i < hitCount; i++) {
        const HitInfo* first = Collision::getStrikeInfoMap(i);
        TVec3f firstNormal(*first->mParentTriangle.getNormal(0));
        for (u32 j = i + 1; j < hitCount; j++) {
            const HitInfo* second = Collision::getStrikeInfoMap(j);
            TVec3f secondNormal(*second->mParentTriangle.getNormal(0));
            if (firstNormal.dot(secondNormal) >= -0.707f) {
                continue;
            }
            if ((first->mHitPos - center).dot(second->mHitPos - center) > 0.0f) {
                continue;
            }
            f32 separation = (first->mHitPos - second->mHitPos).length();
            TVec3f firstVelocity;
            TVec3f secondVelocity;
            MR::calcVelocityMovingPoint(&first->mParentTriangle, first->mHitPos, &firstVelocity);
            MR::calcVelocityMovingPoint(&second->mParentTriangle, second->mHitPos, &secondVelocity);
            if (!MR::isNearZero(firstVelocity) || !MR::isNearZero(secondVelocity)) {
                if (!MR::isNearZero(firstVelocity)) {
                    MR::normalize(&firstVelocity);
                    if (firstVelocity.dot(firstNormal) < 0.2f || firstVelocity.dot(secondNormal) > -0.2f) {
                        continue;
                    }
                }
                if (!MR::isNearZero(secondVelocity)) {
                    MR::normalize(&secondVelocity);
                    if (secondVelocity.dot(secondNormal) < 0.2f || secondVelocity.dot(firstNormal) > -0.2f) {
                        continue;
                    }
                }
                if (first->mParentTriangle.getSensor() != second->mParentTriangle.getSensor() &&
                    (MR::isSensorPressObj(first->mParentTriangle.getSensor()) || MR::isSensorPressObj(second->mParentTriangle.getSensor()))) {
                    if (isStatusActive(MarioStatus_Hang)) {
                        mMovementStates._1 = false;
                        closeStatus(mHang);
                    }
                    if (!mMovementStates._1 && __fabsf(firstNormal.dot(*getGravityVec())) < 0.707f) {
                        if (!first->isCollisionAtFace()) {
                            TVec3f horizontal;
                            f32 vertical = MR::vecKillElement(first->mHitPos - center, *getGravityVec(), &horizontal);
                            if (vertical != 0.0f) {
                                addVelocity(*getGravityVec() * -vertical);
                                mDrawStates._1E = true;
                                continue;
                            }
                        }
                        if (!second->isCollisionAtFace()) {
                            TVec3f horizontal;
                            f32 vertical = MR::vecKillElement(second->mHitPos - center, *getGravityVec(), &horizontal);
                            if (vertical != 0.0f) {
                                addVelocity(*getGravityVec() * -vertical);
                                mDrawStates._1E = true;
                                continue;
                            }
                        }
                    }
                    if (!isStatusActive(MarioStatus_Bury) && !mSinkTimer && ceilingDistance >= 180.0f) {
                        TVec3f firstOffset(first->mHitPos - center);
                        TVec3f secondOffset(center - second->mHitPos);
                        if (!MR::isNearZero(MR::diffAngleAbs(firstOffset, secondOffset), 0.017453292f)) {
                            MR::normalizeOrZero(&firstOffset);
                            MR::normalizeOrZero(&secondOffset);
                            addVelocity(-firstOffset * 5.0f);
                            addVelocity(secondOffset * 5.0f);
                            continue;
                        }
                    }
                    f32 angle = JMAAcosRadian(-firstNormal.dot(secondNormal));
                    if (mActor->selectTiltPress(first->mParentTriangle.getSensor()) || mActor->selectTiltPress(second->mParentTriangle.getSensor())) {
                        angle = 0.0f;
                    }
                    if (!isStatusActive(MarioStatus_Bury) && !mSinkTimer && angle > 0.34906587f) {
                        f32 distance = (160.0f - ceilingDistance) / JMASinRadian(angle);
                        if (!(distance < 0.0f)) {
                            TVec3f firstHorizontal;
                            TVec3f secondHorizontal;
                            MR::vecKillElement(firstNormal, *getGravityVec(), &firstHorizontal);
                            MR::vecKillElement(secondNormal, *getGravityVec(), &secondHorizontal);
                            if (firstHorizontal.length() > secondHorizontal.length()) {
                                firstHorizontal.setLength(distance);
                                addVelocity(firstHorizontal);
                            } else {
                                secondHorizontal.setLength(distance);
                                addVelocity(secondHorizontal);
                            }
                        }
                        continue;
                    }
                    s32 previousFloorCode = _95C->getCode(_45C);
                    if (previousFloorCode != _95C->getCode(mGroundPolygon) && MR::isSensorPressObj(mGroundPolygon->mSensor) &&
                        mVerticalSpeed >= 200.0f) {
                        mDrawStates._A = true;
                        TVec3f horizontal;
                        MR::vecKillElement(mShadowPos - mGroundPos, getAirGravityVec(), &horizontal);
                        horizontal.setLength(10.0f);
                        addVelocity(horizontal);
                        return false;
                    }
                    if (__fabsf(firstNormal.dot(mHeadVec)) > 0.707f) {
                        mActor->setPress(0, 0);
                    } else {
                        mActor->setPress(1, 0);
                    }
                    mActor->_3B4 = firstNormal;
                    return true;
                }
            }
            if (getPlayer()->mMovementStates.jumping) {
                MR::diffAngleAbsHorizontal(firstNormal, secondNormal, getAirGravityVec());
                f32 normalDot = firstNormal.dot(secondNormal);
                TVec3f tangent;
                TVec3f firstUp;
                TVec3f secondUp;
                PSVECCrossProduct(&firstNormal, &getAirGravityVec(), &tangent);
                MR::normalizeOrZero(&tangent);
                PSVECCrossProduct(&tangent, &firstNormal, &firstUp);
                MR::normalizeOrZero(&firstUp);
                PSVECCrossProduct(&secondNormal, &getAirGravityVec(), &tangent);
                MR::normalizeOrZero(&tangent);
                PSVECCrossProduct(&tangent, &secondNormal, &secondUp);
                MR::normalizeOrZero(&secondUp);
                TVec3f firstPosition;
                TVec3f secondPosition;
                firstPosition = first->mHitPos + firstUp * height;
                secondPosition = second->mHitPos + secondUp * height;
                f32 distance = (firstPosition - secondPosition).length();
                if (isRising()) {
                    if (normalDot < -0.707f && distance < 120.0f && separation < 100.0f) {
                        cutGravityElementFromJumpVec(true);
                    }
                } else if (normalDot < -0.707f && separation < 100.0f && distance < 80.0f) {
                    cutGravityElementFromJumpVec(true);
                    mMovementStates._1 = true;
                    stopJump();
                }
            }
        }
    }
    return false;
}
