#include "Game/Util/MathUtil.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "revolution/mtx.h"
#include <JSystem/JGeometry/TUtil.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <JSystem/JMath/JMath.hpp>

extern "C" int __fpclassifyf(f32);

void MathUtil_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::epsilon();
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
    (void)MR::pi();
    (void)(MR::pi() / 2.0f);
    (void)2.0f;
}

void MathUtil_DUMMY() {
    TVec3f vec;
    vec.setLength(1.0f);

    TVec3f a, b, c;
    a.sub(b, c);

    TQuat4f rot;
    rot.setRotate(a, 1.0f);
}

const Vec gZeroVec = {0.0f, 0.0f, 0.0f};

namespace {
    static f32* gAcosTable;
};  // namespace

namespace MR {
    void initAcosTable() {
        ::gAcosTable = new f32[256];

        for (u32 i = 0; i < 256; i++) {
            f64 x = (i / 255.0) * (1.0 - 0.98) + 0.98;

            if (x > 1.0f) {
                x = 1.0f;
            }

            ::gAcosTable[i] = ::acos(x);
        }
    }

    f32 acosEx(f32 x) {
        if (MR::abs(x) < 0.98f) {
            return JMAAcosRadian(x);
        } else if (x < 0.0f) {
            u32 index = static_cast< u32 >((-x - 0.98f) * 50.0f * 255.0f);
            f32 acos = ::gAcosTable[index];

            return pi() - acos;
        } else {
            u32 index = static_cast< u32 >((x - 0.98f) * 50.0f * 255.0f);

            return ::gAcosTable[index];
        }
    }

    f32 getRandom() {
        u32 rand = SingletonHolder< GameSystem >::get()->mObjHolder->mRandom.rand();
        u32 value = (rand >> 9) | 0x3F800000;

        return reinterpret_cast< f32& >(value) - 1.0f;
    }

    f32 getRandom(f32 min, f32 max) {
        return min + (max - min) * getRandom();
    }

    s32 getRandom(s32 min, s32 max) {
        return getRandom(static_cast< f32 >(min), static_cast< f32 >(max));
    }

    f32 getRandomDegree() {
        return getRandom(0.0f, 360.0f);
    }

    void calcRandomVec(TVec3f* pDst, f32 min, f32 max) {
        pDst->set< f32 >(getRandom(min, max), getRandom(min, max), getRandom(min, max));
    }

    bool isHalfProbability() {
        return getRandom() < 0.5f;
    }

    f32 getSignHalfProbability() {
        if (isHalfProbability()) {
            return -1.0f;
        } else {
            return 1.0f;
        }
    }

    void getRandomVector(TVec3f* pDst, f32 range) {
        pDst->set< f32 >(getRandom(-range, range), getRandom(-range, range), getRandom(-range, range));
    }

    void addRandomVector(TVec3f* pDst, const TVec3f& rSrc, f32 range) {
        f32 x = getRandom(-range, range);
        f32 y = getRandom(-range, range);
        f32 z = getRandom(-range, range);

        pDst->set(rSrc + TVec3f(x, y, z));
    }

    void turnRandomVector(TVec3f* pDst, const TVec3f& rSrc, f32 range) {
        f32 srcLength = rSrc.length();

        addRandomVector(pDst, rSrc, range);

        if (isNearZero(*pDst)) {
            pDst->set(rSrc);
        } else {
            pDst->setLength(srcLength);
        }
    }

    f32 getInterpolateValue(f32 t, f32 start, f32 end) {
        return start + (end - start) * t;
    }

    f32 getLinerValue(f32 x, f32 start, f32 end, f32 max) {
        return getInterpolateValue(x / max, start, end);
    }

    f32 getLinerValueFromMinMax(f32 x, f32 min, f32 max, f32 start, f32 end) {
        return getInterpolateValue((JGeometry::TUtil< f32 >::clamp(x, min, max) - min) / (max - min), start, end);
    }

    f32 getEaseInValue(f32 x, f32 start, f32 end, f32 max) {
        f32 t = x / max * pi() / 2.0f;
        return getInterpolateValue(1.0f - JMACosRadian(x / max * pi() / 2.0f), start, end);
    }

    f32 getEaseOutValue(f32 x, f32 start, f32 end, f32 max) {
        f32 t = x / max * pi() / 2.0f;
        return getInterpolateValue(JMASinRadian(t), start, end);
    }

    f32 getEaseInOutValue(f32 x, f32 start, f32 end, f32 max) {
        f32 t = x / max * pi();
        return getInterpolateValue((1.0f - JMACosRadian(t)) / 2.0f, start, end);
    }

    f32 getScaleWithReactionValueZeroToOne(f32 x, f32 rate, f32 amplitude) {
        if (x < 0.5f) {
            return getEaseOutValue(x * 2.0f, 0.0f, 1.0f, 1.0f);
        } else {
            // reaction oscillation, TODO: inline?
            f32 t = (x - 0.5f) * 2.0f;
            return 1.0f + (1.0f - JMACosRadian(pi() * rate * t)) * (1.0f - t) * amplitude;
        }
    }

    f32 getConvergeVibrationValue(f32 x, f32 start, f32 end, f32 dampScale, f32 rate) {
        f32 vibWeight = (x * x) * (x * x);
        f32 t1 = 1.0f - x;
        f32 convergeWeight = (t1 * t1) * (t1 * t1);
        f32 dampRatio = dampScale * t1;

        f32 vibration = JMASinRadian(PI * (x + rate * vibWeight));
        return getInterpolateValue((1.0f - convergeWeight) + vibration * dampRatio, start, end);
    }

    f32 getReduceVibrationValue(f32 x, f32 time, f32 base, f32 amplitude, f32 freq) {
        // FIXME: float swap
        f32 vibMax = base + amplitude;

        f32 vib = JMACosRadian(x * (1.0f / freq * PI));
        f32 vibration = (amplitude * 0.5f) * (vib - 1.0f);
        if (x >= time) {
            return vibration + vibMax;
        } else {
            f32 t = x - time;
            f32 reduce = (1.0f - vibMax) * (1.0f / (time * time)) * t * t;
            return vibration + (vibMax + reduce);
        }
    }

    void makeAxisFrontUp(TVec3f* pSide, TVec3f* pUp, const TVec3f& rFront, const TVec3f& rUp) {
        pSide->cross(rUp, rFront);
        normalize(pSide);
        pUp->cross(rFront, *pSide);
    }

    void makeAxisFrontSide(TVec3f* pUp, TVec3f* pSide, const TVec3f& rFront, const TVec3f& rSide) {
        pUp->cross(rFront, rSide);
        normalize(pUp);
        pSide->cross(*pUp, rFront);
    }

    void makeAxisUpFront(TVec3f* pSide, TVec3f* pFront, const TVec3f& rUp, const TVec3f& rFront) {
        pSide->cross(rUp, rFront);
        normalize(pSide);
        pFront->cross(*pSide, rUp);
    }

    void makeAxisUpSide(TVec3f* pFront, TVec3f* pSide, const TVec3f& rUp, const TVec3f& rSide) {
        pFront->cross(rSide, rUp);
        normalize(pFront);
        pSide->cross(rUp, *pFront);
    }

    void makeAxisVerticalZX(TVec3f* pVec, const TVec3f& rAxis) {
        pVec->killElement(TVec3f(0.0f, 0.0f, 1.0f), rAxis);
        if (isNearZero(*pVec)) {
            pVec->killElement(TVec3f(1.0f, 0.0f, 0.0f), rAxis);
        }
        normalize(pVec);
    }

    void makeAxisCrossPlane(TVec3f* pVec, TVec3f* pCross, const TVec3f& rNorm) {
        makeAxisVerticalZX(pVec, rNorm);
        pCross->cross(*pVec, rNorm);
        normalizeOrZero(pCross);
    }

    bool makeAxisAndCosignVecToVec(TVec3f* pAxis, f32* pCos, const TVec3f& rFrom, const TVec3f& rTo) {
        TVec3f from;
        if (isOppositeDirection(rFrom, rTo)) {
            turnRandomVector(&from, rFrom, 0.01f);
            normalize(&from);
        } else {
            from.set(rFrom);
        }

        TVec3f axis = from.cross(rTo);

        if (isNearZero(axis)) {
            pAxis->zero();
            *pCos = 1.0f;

            return false;
        } else {
            normalize(axis, pAxis);
            *pCos = JGeometry::TUtil< f32 >::clamp(from.dot(rTo), -1.0f, 1.0f);

            return true;
        }
    }

    f32 calcPerpendicFootToLine(TVec3f* pDst, const TVec3f& rPos, const TVec3f& rPointA, const TVec3f& rPointB) {
        TVec3f offset = rPointB - rPointA;
        f32 ratio = rPos.dot(offset) - rPointA.dot(offset);
        ratio /= offset.squared();
        offset.scale(ratio);
        pDst->set(rPointA);
        pDst->add(offset);
        return ratio;
    }

    f32 calcPerpendicFootToLineInside(TVec3f* pDst, const TVec3f& rPos, const TVec3f& rPointA, const TVec3f& rPointB) {
        TVec3f offset = rPointB - rPointA;
        f32 ratio = rPos.dot(offset) - rPointA.dot(offset);
        ratio /= offset.squared();
        ratio = JGeometry::TUtil< f32 >::clamp(ratio, 0.0f, 1.0f);
        offset.scale(ratio);
        pDst->set(rPointA);
        pDst->add(offset);
        return ratio;
    }

    bool checkHitSegmentSphere(const TVec3f& rSpherePos, const TVec3f& rPointA, const TVec3f& rPointB, f32 radius, TVec3f* pDir) {
        TVec3f pos = rSpherePos;
        TVec3f segment = rPointB;
        pos -= rPointA;
        segment -= rPointA;

        f32 dot = pos.dot(segment);
        f32 radSquared = radius * radius;
        if (dot < 0.0f) {
            if (rPointA.squared(rSpherePos) < radSquared) {
                if (pDir != nullptr) {
                    pDir->set(rSpherePos - rPointA);
                    MR::normalizeOrZero(pDir);
                }
                return true;
            }
        } else if (segment.squared() < dot) {
            if (rPointB.squared(rSpherePos) < radSquared) {
                if (pDir != nullptr) {
                    pDir->set(rSpherePos - rPointB);
                    MR::normalizeOrZero(pDir);
                }
                return true;
            }
        } else {
            TVec3f offset = segment;
            offset.scale(dot / segment.squared());
            offset.sub(pos);
            if (offset.squared() <= radSquared) {
                if (pDir != nullptr) {
                    pDir->set(-offset);
                    MR::normalizeOrZero(pDir);
                }
                return true;
            }
        }

        return false;
    }

    bool checkHitSemilinePlane(TVec3f* pHitPos, const TVec3f& rPos, const TVec3f& rDir, const TVec3f& rPlanePos, const TVec3f& rPlaneNorm) {
        f32 dot = rDir.dot(rPlaneNorm);
        TVec3f offset = rPlanePos - rPos;
        if (0.0f < dot) {
            return false;
        }

        f32 ratio = rPlaneNorm.dot(offset) / dot;
        pHitPos->set(rPos);
        pHitPos->add(rDir * ratio);

        return true;
    }

    bool calcReboundVelocity(TVec3f* pVel, const TVec3f& rNormal, f32 reboundRate) {
        f32 dot = pVel->dot(rNormal);
        if (dot < 0.0f) {
            f32 rate = reboundRate + 1.0f;
            pVel->x -= rate * rNormal.x * dot;
            pVel->y -= rate * rNormal.y * dot;
            pVel->z -= rate * rNormal.z * dot;
            return true;
        }

        return false;
    }

    bool calcReboundVelocity(TVec3f* pVel, const TVec3f& rNormal, f32 reboundRateV, f32 reboundRateH) {
        f32 dot = pVel->dot(rNormal);
        if (dot < 0.0f) {
            pVel->sub(rNormal * dot);
            pVel->scale(reboundRateH);
            pVel->sub(rNormal * dot * reboundRateV);
            return true;
        }

        return false;
    }

    void calcParabolicFunctionParam(f32* pAccel, f32* pVel, f32 max, f32 end) {
        f32 discrim = max * (max - end);

        if (discrim <= 0.0f) {
            *pAccel = -max;
            *pVel = end + max;
            return;
        }

        if (MR::abs(end) < 0.0001f) {
            *pAccel = max * -4.0f;
            *pVel = max * 4.0f;
            return;
        }

        f32 discRoot = JMASqrt(discrim);

        f32 t1 = (max + discRoot) / end;
        f32 t2 = (max - discRoot) / end;

        f32 t;
        if (0.0f <= t1 && t1 <= 1.0f) {
            t = t1;
        } else if (0.0f <= t2 && t2 <= 1.0f) {
            t = t2;
        } else {
            t = 1.0f;
        }

        f32 a = -max / (t * t);
        *pAccel = a;
        *pVel = -2.0f * t * a;
    }

    void makeQuatRotateRadian(TQuat4f* pQuat, const TVec3f& rRot) {
        pQuat->setEuler(rRot.x, rRot.y, rRot.z);
    }

    void makeQuatRotateDegree(TQuat4f* pQuat, const TVec3f& rRot) {
        pQuat->setEulerDegree(rRot.x, rRot.y, rRot.z);
    }

    void makeQuatFromVec(TQuat4f* pQuat, const TVec3f& rFront, const TVec3f& rUp) {
        TPos3f mtx;
        makeMtxFrontUp(&mtx, rFront, rUp);
        mtx.getQuat(*pQuat);
    }

    void makeQuatSideUp(TQuat4f* pParam1, const TVec3f& rParam2, const TVec3f& rParam3) {
        TPos3f mtx;
        makeMtxSideUp(&mtx, rParam2, rParam3);
        mtx.getQuat(*pParam1);
    }

    void makeQuatUpFront(TQuat4f* pQuat, const TVec3f& rUp, const TVec3f& rFront) {
        TPos3f mtx;
        makeMtxUpFront(&mtx, rUp, rFront);
        mtx.getQuat(*pQuat);
    }

    void makeQuatUpNoSupport(TQuat4f* pQuat, const TVec3f& rUp) {
        TPos3f mtx;
        makeMtxUpNoSupport(&mtx, rUp);
        mtx.getQuat(*pQuat);
    }

    void blendQuatUpFront(TQuat4f* pDst, const TVec3f& rUp, const TVec3f& rFront, f32 upRate, f32 frontRate) {
        blendQuatUpFront(pDst, *pDst, rUp, rFront, upRate, frontRate);
    }

    void blendQuatUpFront(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rUp, const TVec3f& rFront, f32 upRate, f32 frontRate) {
        TQuat4f q;
        q.set(rSrc);
        TVec3f up;
        q.getYDir(up);

        bool isUpSame = up.dot(rUp) >= 0.0f ? false : isSameDirection(up, rUp);
        if (isUpSame) {
            turnRandomVector(&up, up, 0.001f);
        }

        TQuat4f upRot;
        upRot.setRotate(up, rUp, upRate);
        q.mult(upRot);

        q.getYDir(up);
        TVec3f front;
        q.getZDir(front);

        TVec3f targetFront = rFront.killElement(up);
        MR::normalizeOrZero(&targetFront);

        bool isFrontSame = front.dot(targetFront) >= 0.0f ? false : isSameDirection(front, targetFront);
        if (isFrontSame) {
            turnRandomVector(&front, front, 0.001f);
        }

        TQuat4f frontRot;
        frontRot.setRotate(front, targetFront, frontRate);
        q.mult(frontRot);

        q.normalize();
        pDst->set(q);
    }

    void blendQuatFrontUp(TQuat4f* pDst, const TVec3f& rFront, const TVec3f& rUp, f32 upRate, f32 frontRate) {
        blendQuatFrontUp(pDst, *pDst, rFront, rUp, upRate, frontRate);
    }

    void blendQuatFrontUp(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rFront, const TVec3f& rUp, f32 frontRate, f32 upRate) {
        // NOTE: axes are reversed here for the quat.

        TQuat4f q;
        q.set(rSrc);
        TVec3f up;
        q.getZDir(up);

        bool isUpSame = up.dot(rUp) >= 0.0f ? false : isSameDirection(up, rUp);
        if (isUpSame) {
            turnRandomVector(&up, up, 0.001f);
        }

        TQuat4f upRot;
        upRot.setRotate(up, rUp, upRate);
        q.mult(upRot);

        TVec3f front;
        q.getYDir(front);
        q.getZDir(up);

        TVec3f targetFront = rFront.killElement(up);
        MR::normalizeOrZero(&targetFront);

        bool isFrontSame = front.dot(targetFront) >= 0.0f ? false : isSameDirection(front, targetFront);
        if (isFrontSame) {
            turnRandomVector(&front, front, 0.001f);
        }

        TQuat4f frontRot;
        frontRot.setRotate(front, targetFront, frontRate);
        q.mult(frontRot);

        q.normalize();
        pDst->set(q);
    }

    void rotateQuatMoment(TQuat4f* pQuat, const TVec3f& rMoment) {
        TQuat4f rot;
        f32 rate;
        TVec3f axis;
        separateScalarAndDirection(&rate, &axis, rMoment);
        rot.setRotate(axis, rate);
        pQuat->mult(rot);
        pQuat->normalize();
    }

    void rotateQuatRollBall(TQuat4f* pQuat, const TVec3f& rVel, const TVec3f& rNorm, f32 radius) {
        TVec3f moment;
        calcMomentRollBall(&moment, rVel, rNorm, radius);
        rotateQuatMoment(pQuat, moment);
    }

    bool turnQuat(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rFrom, const TVec3f& rTo, f32 angle) {
        TVec3f from;
        TVec3f target = rTo;

        bool isSameDir = rFrom.dot(rTo) >= 0.0f ? false : isSameDirection(rFrom, rTo);
        if (isSameDir) {
            turnRandomVector(&from, rFrom, 0.001f);
        } else {
            from.set(rFrom);
        }

        MR::normalizeOrZero(&from);
        MR::normalizeOrZero(&target);

        f32 angleBetween = acosEx(JGeometry::TUtil< f32 >::clamp(from.dot(target), -1.0f, 1.0f));
        f32 rate;
        if (angleBetween <= angle) {
            rate = 1.0f;
        } else {
            rate = JGeometry::TUtil< f32 >::clamp(angle / angleBetween, 0.0f, 1.0f);
        }

        TQuat4f rot;
        rot.setRotate(from, target, rate);
        pDst->mult(rot, rSrc);
        pDst->normalize();

        return angleBetween < 0.015f;
    }

    bool turnQuatXDirRad(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 angle) {
        TVec3f xDir;
        rSrc.getXDir(xDir);
        return turnQuat(pDst, rSrc, xDir, rTo, angle);
    }

    void turnQuatXDirRate(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 rate) {
        TVec3f xDir;
        rSrc.getXDir(xDir);
        TQuat4f rot;
        rot.setRotate(xDir, rTo, rate);
        pDst->mult(rot, rSrc);
    }

    bool turnQuatYDirRad(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 angle) {
        TVec3f yDir;
        rSrc.getYDir(yDir);
        return turnQuat(pDst, rSrc, yDir, rTo, angle);
    }

    void turnQuatYDirRate(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 rate) {
        TVec3f yDir;
        rSrc.getYDir(yDir);
        TQuat4f rot;
        rot.setRotate(yDir, rTo, rate);
        pDst->mult(rot, rSrc);
    }

    bool turnQuatZDirRad(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 angle) {
        TVec3f zDir;
        rSrc.getZDir(zDir);
        return turnQuat(pDst, rSrc, zDir, rTo, angle);
    }

    void turnQuatZDirRate(TQuat4f* pDst, const TQuat4f& rSrc, const TVec3f& rTo, f32 rate) {
        TVec3f zDir;
        rSrc.getZDir(zDir);
        TQuat4f rot;
        rot.setRotate(zDir, rTo, rate);
        pDst->mult(rot, rSrc);
    }

    f32 sign(f32 x) {
        if (0.0f < x) {
            return 1.0f;
        }

        if (x < 0.0f) {
            return -1.0f;
        }

        return 0.0f;
    }

    void clampVecAngleDeg(TVec3f* pDst, const TVec3f& rBase, f32 angle) {
        if (pDst->angle(rBase) * _180_PI <= angle) {  // TODO: value written directly?
            return;
        }

        TVec3f axis = rBase.cross(*pDst);
        if (MR::normalizeOrZero(&axis)) {
            return;
        }

        f32 degree = JMACosDegree(angle);

        rotateVecDegree(pDst, rBase, axis, angle);
    }

    void clampLength(TVec3f* pDst, const TVec3f& rSrc, f32 length) {
        if (rSrc.squared() > length * length) {
            pDst->setLength(rSrc, length);
        } else {
            pDst->set(rSrc);
        }
    }

    f32 convergeRadian(f32 angle, f32 target, f32 speed) {
        f32 convergeTarget = target;
        if ((target + pi2()) - angle < pi()) {
            convergeTarget = target + pi2();
        } else if (angle - (target - pi2()) < pi()) {
            convergeTarget = target - pi2();
        }

        if (angle < convergeTarget) {
            angle += speed;
            if (angle > convergeTarget) {
                angle = convergeTarget;
            }
        } else {
            angle -= speed;
            if (angle < convergeTarget) {
                angle = convergeTarget;
            }
        }

        return repeat(angle, 0.0f, pi2());
    }

    bool isInRange(f32 x, f32 min, f32 max) {
        if (min > max) {
            if (x < max) {
                return false;
            }
            if (x > min) {
                return false;
            }
            return true;
        } else {
            if (x < min) {
                return false;
            }
            if (x > max) {
                return false;
            }
            return true;
        }
    }

    f32 calcRotateY(f32 x, f32 z) {
        return MR::repeatDegree(MR::toDegree(MR::atan2(-z, x)) + 90.0f);
    }

    f32 calcRotateZ(const TVec3f& rVecA, const TVec3f& rVecB) {
        // FIXME:
        // Compiler refuses to cooperate, but mathematically this is correct
        TVec2f vec(rVecB.y - rVecA.y, rVecB.x - rVecA.x);
        return MR::atan2(vec.x, vec.y) * (180.0f / PI);
    }

    f32 calcDistanceXY(const TVec3f& rPos1, const TVec3f& rPos2) {
        f32 xDelta = rPos1.x - rPos2.x;
        f32 yDelta = rPos1.y - rPos2.y;
        return JGeometry::TUtil< f32 >::sqrt(xDelta * xDelta + yDelta * yDelta);
    }

    void rotateVecDegree(f32* pX, f32* pY, f32 degree) {
        f32 x = *pX;
        f32 y = *pY;
        f32 Sin = JMASinDegree(degree);
        f32 Cos = JMACosDegree(degree);

        *pX = (Cos * x) - (Sin * y);
        *pY = (Sin * x) + (Cos * y);
    }

    void rotateVecDegree(TVec2f* pDst, f32 degree) {
        rotateVecDegree(&pDst->x, &pDst->y, degree);
    }

    void rotateVecDegree(TVec3f* pDst, const TVec3f& rAxis, f32 degree) {
        rotateVecDegree(pDst, *pDst, rAxis, degree);
    }

    void rotateVecDegree(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rAxis, f32 degree) {
        TRot3f rotation;
        rotation.identity();
        rotation.setRotate(rAxis, degree * PI_180);
        rotation.mult(rSrc, *pDst);
    }

    void rotateVecRadian(TVec3f* pDst, const TVec3f& rAxis, f32 angle) {
        rotateVecRadian(pDst, *pDst, rAxis, angle);
    }

    void rotateVecRadian(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rAxis, f32 angle) {
        TRot3f rotation;
        rotation.identity();
        rotation.setRotate(rAxis, angle);
        rotation.mult(rSrc, *pDst);
    }

    void calcLocalVec(TVec3f* pVec, MtxPtr pMtx) {
        TPos3f inv = pMtx;
        inv.invert(inv);
        inv.mult(*pVec, *pVec);
    }

    void normalize(TVec2f* pVec) {
        TVec3f temp(pVec->x, pVec->y, 0.0f);
        normalize(&temp);
        pVec->x = temp.x;
        pVec->y = temp.y;
    }

    void normalize(TVec3f* pVec) {
        PSVECNormalize(pVec, pVec);
    }

    void normalize(TPos3f* pMtx) {
        TVec3f x, y, z;
        pMtx->getXYZDir(x, y, z);
        normalize(&x);
        normalize(&y);
        normalize(&z);
        pMtx->setXYZDir(x, y, z);
    }

    void normalize(const TVec2f& rSrc, TVec2f* pDst) {
        *pDst = rSrc;
        normalize(pDst);
    }

    void normalize(const TVec3f& rSrc, TVec3f* pDst) {
        pDst->set(rSrc);
        normalize(pDst);
    }

    bool normalizeOrZero(TVec2f* pVec) {
        if (isNearZero(*pVec)) {
            pVec->zero();
            return true;
        }

        normalize(pVec);
        return false;
    }

    bool normalizeOrZero(TVec3f* pVec) {
        if (isNearZero(*pVec)) {
            pVec->zero();
            return true;
        }

        normalize(pVec);
        return false;
    }

    bool normalizeOrZero(const TVec3f& rSrc, TVec3f* pDst) {
        pDst->set(rSrc);

        return normalizeOrZero(pDst);
    }

    void separateScalarAndDirection(f32* pScalar, TVec2f* pDir, const TVec2f& rVec) {
        *pScalar = rVec.length();

        if (isNearZero(rVec)) {
            pDir->zero();
        } else {
            normalize(rVec, pDir);
        }
    }

    void separateScalarAndDirection(f32* pScalar, TVec3f* pDir, const TVec3f& rVec) {
        *pScalar = rVec.length();

        if (isNearZero(rVec)) {
            pDir->zero();
        } else {
            normalize(rVec, pDir);
        }
    }

    f32 normalize(f32 x, f32 min, f32 max) {
        f32 range = max - min;

        if (!isNearZero(range)) {
        } else if (x < min) {
            return 0.0f;
        } else {
            return 1.0f;
        }

        return (JGeometry::TUtil< f32 >::clamp(x, min, max) - min) / range;
    }

    f32 normalizeAbs(f32 x, f32 min, f32 max) {
        if (x >= 0.0f) {
            return normalize(x, min, max);
        } else {
            return -normalize(-x, min, max);
        }
    }

    bool turnVecToVecCos(TVec3f* pDst, const TVec3f& rFrom, const TVec3f& rTo, f32 cosAngle, const TVec3f& rAxis, f32 fallbackLength) {
        if (isNearZero(rFrom)) {
            return false;
        }

        if (isNearZero(rTo)) {
            return false;
        }

        if (rFrom.dot(rTo) > cosAngle) {
            pDst->set(rTo);
            normalize(pDst);
            return true;
        }

        f32 sinAngle = JMASqrt(1.0f - cosAngle * cosAngle);
        TVec3f rejectFrom = rTo.killElement(rFrom);

        if (isNearZero(rejectFrom)) {
            TVec3f fallback = rFrom.cross(rAxis);
            normalize(&fallback);
            pDst->set(rFrom + fallback * fallbackLength);
            normalize(pDst);
            return false;
        }

        normalize(&rejectFrom);
        pDst->set(rFrom * cosAngle + rejectFrom * sinAngle);
        normalize(pDst);
        return false;
    }

    bool turnVecToVecCosOnPlane(TVec3f* pDst, const TVec3f& rFrom, const TVec3f& rTo, const TVec3f& rAxis, f32 cosAngle) {
        TVec3f planarTo = rTo.killElement(rAxis);
        normalizeOrZero(&planarTo);
        if (!MR::isNearZero(planarTo)) {
            if (cosAngle > -1.0f) {
                return turnVecToVecCos(pDst, rFrom, planarTo, cosAngle, rAxis);
            }

            pDst->set(planarTo);
            return true;
        }
        return false;
    }

    bool turnVecToVecCosOnPlane(TVec3f* pVec, const TVec3f& rTo, const TVec3f& rAxis, f32 cosAngle) {
        return turnVecToVecCosOnPlane(pVec, *pVec, rTo, rAxis, cosAngle);
    }

    f32 turnVecToVecRadian(TVec3f* pDst, const TVec3f& rFrom, const TVec3f& rTo, f32 angle, const TVec3f& rAxis) {
        // FIXME: extra load causing out-of-order compare
        f32 angleBetween = JMAAcosRadian(JGeometry::TUtil< f32 >::clamp(rFrom.dot(rTo), -1.0f, 1.0f));
        f32 angleMoved = 0.0f;
        f32 angleClamped = angleBetween;
        if (angle <= angleClamped) {
            angleMoved = angleClamped - angle;
            angleClamped = angle;
        }

        if (isNearZero(angleBetween)) {
            return 0.0f;
        }

        TQuat4f rot;
        bool isSameDir = rFrom.dot(rTo) >= 0.0f ? false : isSameDirection(rFrom, rTo);
        if (isSameDir) {
            rot.setRotate(rAxis, angleClamped);
        } else {
            rot.setRotate(rFrom, rTo, angleClamped / angleBetween);
        }

        TVec3f v = rFrom;
        rot.transform(v);
        normalizeOrZero(&v);
        if (!isNearZero(v)) {
            pDst->set(v);
        }

        return angleMoved;
    }

    f32 turnVecToVecDegree(TVec3f* pDst, const TVec3f& rFrom, const TVec3f& rTo, f32 angle, const TVec3f& rAxis) {
        return turnVecToVecRadian(pDst, rFrom, rTo, angle * (PI / 180.0f), rAxis) * (180.0f / PI);
    }

    void calcMomentRollBall(TVec3f* pMoment, const TVec3f& rVel, const TVec3f& rUp, f32 radius) {
        TVec3f up;
        up.set(rUp);
        if (normalizeOrZero(&up)) {
            pMoment->zero();
            return;
        }

        TVec3f moment = up.cross(rVel);
        moment /= radius;
        pMoment->set(moment);
    }

    bool calcReflectionVector(TVec3f* pDir, const TVec3f& rNorm, f32 rate, f32 cosAngleMin) {
        f32 dot = rNorm.dot(*pDir);
        if (dot < -cosAngleMin) {
            pDir->sub(rNorm * dot * (rate + 1.0f));
            return true;
        }

        if (dot < 0.0f) {
            pDir->sub(rNorm * dot);
            return false;
        }

        return false;
    }

    bool isSameDirection(const TVec3f& rVec1, const TVec3f& rVec2, f32 tolerance) {
        if (MR::abs(rVec1.y * rVec2.z - rVec1.z * rVec2.y) > tolerance) {
            return false;
        }

        if (MR::abs(rVec1.z * rVec2.x - rVec1.x * rVec2.z) > tolerance) {
            return false;
        }

        if (MR::abs(rVec1.x * rVec2.y - rVec1.y * rVec2.x) > tolerance) {
            return false;
        }

        return true;
    }

    bool isOppositeDirection(const TVec3f& rVec1, const TVec3f& rVec2, f32 tolerance) {
        if (rVec1.dot(rVec2) >= 0.0f) {
            return false;
        }

        return isSameDirection(rVec1, rVec2, tolerance);
    }

    bool isNearZero(f32 x, f32 tolerance) {
        if (x < 0.0f) {
            x = -x;
        }

        if (x < tolerance) {
            return true;
        }

        return false;
    }

    bool isNearZero(const TVec3f& rVec, f32 tolerance) {
        if (rVec.x > tolerance) {
            return false;
        }

        if (rVec.x < -tolerance) {
            return false;
        }

        if (rVec.y > tolerance) {
            return false;
        }

        if (rVec.y < -tolerance) {
            return false;
        }

        if (rVec.z > tolerance) {
            return false;
        }

        if (rVec.z < -tolerance) {
            return false;
        }

        return true;
    }

    bool isNearZero(const TVec2f& rVec, f32 tolerance) {
        if (rVec.x > tolerance) {
            return false;
        }

        if (rVec.x < -tolerance) {
            return false;
        }

        if (rVec.y > tolerance) {
            return false;
        }

        if (rVec.y < -tolerance) {
            return false;
        }

        return true;
    }

    f32 diffAngleAbs(f32 angleA, f32 angleB) {
        f32 normalize = normalizeAngleAbs(angleA - angleB);

        if (normalize > PI) {
            normalize = (TWO_PI - normalize);
        }

        return normalize;
    }

    f32 normalizeAngleAbs(f32 angle) {
        while (true) {
            if (angle < 0.0f) {
                angle += TWO_PI;
            } else if (angle > TWO_PI) {
                angle -= TWO_PI;
            } else {
                return angle;
            }
        }
    }

    bool isAngleBetween(f32 angle, f32 min, f32 max) {
        f32 a1_n = normalizeAngleAbs(angle);
        f32 a2_n = normalizeAngleAbs(min);
        f32 a3_n = normalizeAngleAbs(max);

        if (a3_n > a2_n) {
            f32 val = a3_n;
            a3_n = a2_n;
            a2_n = val;
        }

        bool res = a1_n >= a3_n && a1_n <= a2_n;

        if (a2_n - a3_n > PI) {
            res = !res;
        }

        return res;
    }

    f32 blendAngle(f32 angleA, f32 angleB, f32 rate) {
        // FIXME: float regswap
        f32 a1_n = normalizeAngleAbs(angleA);
        f32 a2_n = normalizeAngleAbs(angleB);

        if (!isAngleBetween((a1_n + a2_n) / 2.0f, a1_n, a2_n)) {
            if (a1_n < a2_n) {
                a1_n += TWO_PI;
            } else {
                a2_n += TWO_PI;
            }
        }

        return normalizeAngleAbs((1.0f - rate) * a1_n + rate * a2_n);
    }

    u8 lerp(u8 start, u8 end, f32 t) {
        return JGeometry::TUtil< f32 >::clamp(start + (end - start) * t, 0.0f, 255.0f);
    }

    GXColor lerp(GXColor start, GXColor end, f32 t) {
        u8 a = lerp(start.a, end.a, t);
        u8 b = lerp(start.b, end.b, t);
        u8 g = lerp(start.g, end.b, t);
        u8 r = lerp(start.r, end.r, t);

        GXColor color = {r, g, b, a};

        return color;
    }

    // This function implements the selection sort sorting algorithm
    // on an array of f32 where sortArray is the array to be sorted
    // and indexArray holds the indices the elements had in the original array
    void sortSmall(s32 length, f32* sortArray, s32* indexArray) {
        for (int i = 0; i < length; i++) {
            indexArray[i] = i;
        }
        for (int index = 0; index < length; index++) {
            f32 element = sortArray[index];
            int indexOfSmallestElement = index;
            for (int i = index + 1; i < length; i++) {
                if (element > sortArray[i]) {
                    element = sortArray[i];
                    indexOfSmallestElement = i;
                }
            }
            s32 temp = indexArray[index];
            f32 temp2 = sortArray[index];
            indexArray[index] = indexArray[indexOfSmallestElement];
            sortArray[index] = element;
            indexArray[indexOfSmallestElement] = temp;
            sortArray[indexOfSmallestElement] = temp2;
        }
    };

    // This function implements the selection sort sorting algorithm
    // on an array of u32 where sortArray is the array to be sorted
    // and indexArray holds the indices the elements had in the original array
    void sortSmall(s32 length, u32* sortArray, s32* indexArray) {
        for (int i = 0; i < length; i++) {
            indexArray[i] = i;
        }
        for (int index = 0; index < length; index++) {
            u32 element = sortArray[index];
            int indexOfSmallestElement = index;
            for (int i = index + 1; i < length; i++) {
                if (element > sortArray[i]) {
                    element = sortArray[i];
                    indexOfSmallestElement = i;
                }
            }
            s32 temp = indexArray[index];
            u32 temp2 = sortArray[index];
            indexArray[index] = indexArray[indexOfSmallestElement];
            sortArray[index] = element;
            indexArray[indexOfSmallestElement] = temp;
            sortArray[indexOfSmallestElement] = temp2;
        }
    };
};  // namespace MR

f32 PSVECKillElement(__REGISTER const Vec* pSrc, __REGISTER const Vec* pKill, __REGISTER const Vec* pDst) {
    // FIXME: out of order instruction
#ifdef __MWERKS__
    __REGISTER f32 dot;
    // clang-format off
    asm {
        psq_l      f2, 4(pSrc),  0, 0
        psq_l      f5, 0(pSrc),  1, 0
        psq_l      f3, 4(pKill), 0, 0
        psq_l      f4, 0(pKill), 1, 0
        ps_mul     f6, f2, f3
        ps_madd    f7, f5, f4, f6
        ps_sum0    dot, f7, f6, f6
        ps_merge00 dot, dot, dot
        ps_nmsub   f5, dot, f4, f5
        ps_nmsub   f2, dot, f3, f2
        psq_st     f5, 0(pDst),  1, 0
        psq_st     f2, 4(pDst),  0, 0
    }
    // clang-format on
    return dot;
#endif
}

namespace MR {
    f32 vecKillElement(const TVec3f& rSrc, const TVec3f& rKillDir, TVec3f* pDst) {
        if (isNearZero(rKillDir)) {
            *pDst = rSrc;

            return 0.0f;
        }

        return PSVECKillElement(rSrc, rKillDir, pDst);
    }

    void vecScaleAdd(const register TVec3f* a1, const register TVec3f* a2, register f32 a3) {
#ifdef __MWERKS__
        __asm {
            psq_l f0, 0(a1), 0, 0
            psq_l f3, 0(a2), 0, 0
            psq_l f2, 8(a1), 1, 0
            psq_l f4, 8(a2), 1, 0
            ps_madds0 f0, f3, a3, f0
            ps_madds0 f2, f4, a3, f2
            psq_st f0, 0(a1), 0, 0
            psq_st f2, 8(a1), 1, 0
        }
#endif
    }

    void PSvecBlend(const register TVec3f* a1, const register TVec3f* a2, register TVec3f* a3, register f32 a4, register f32 a5) {
#ifdef __MWERKS__
        __asm {
            psq_l     f0, 0(a1), 0, 0
            psq_l     f3, 8(a1), 1, 0
            ps_muls0  f4, f0, a4
            psq_l     f0, 0(a2), 0, 0
            ps_muls0  f3, f3, a4
            psq_l     f1, 8(a2), 1, 0
            ps_madds0 f4, f0, f2, f4
            ps_madds0 f3, f1, f2, f3
            psq_st    f4, 0(a3), 0, 0
            psq_st    f3, 8(a3), 1, 0
        }
#endif
    }

    void vecBlend(const TVec3f& rFrom, const TVec3f& rTo, TVec3f* pDst, f32 rate) {
        PSvecBlend(&rFrom, &rTo, pDst, 1.0f - rate, rate);
    }

    void vecBlendNormal(const TVec3f& rFrom, const TVec3f& rTo, TVec3f* pVec, f32 rate) {
        TVec3f vA;
        if (isNearZero(rFrom)) {
            vA.zero();
        } else {
            vA.set(rFrom);
            PSVECNormalize(vA, vA);
            PSVECScale(vA, vA, 1.0f - rate);
        }

        TVec3f vB;
        if (isNearZero(rTo)) {
            vB.zero();
        } else {
            vB.set(rTo);
            PSVECNormalize(vB, vB);
            PSVECScale(vB, vB, rate);
        }

        PSVECAdd(vA, vB, pVec);
    }

    bool vecBlendSphere(const TVec3f& rFrom, const TVec3f& rTo, TVec3f* pVec, f32 rate) {
        f32 lenFrom = rFrom.length();
        f32 lenTo = rTo.length();
        f32 diffAngle;
        if (lenFrom == 0.0f || lenTo == 0.0f) {
            diffAngle = 0.0f;
        } else {
            diffAngle = diffAngleAbs(rFrom, rTo);
        }

        if (diffAngle < 0.1f) {
            vecBlendNormal(rFrom, rTo, pVec, rate);
            pVec->setLength(lenFrom * (1.0f - rate) + lenTo * rate);
            return true;
        }

        if (diffAngle == 0.0f) {
            *pVec = rFrom;
            pVec->setLength(lenFrom * (1.0f - rate) + lenTo * rate);
            return false;
        }

        if (diffAngle == pi()) {
            return false;
        }

        *pVec = (rFrom * JMASinRadian(diffAngle * (1.0f - rate)) + rTo * JMASinRadian(diffAngle * rate)) / JMASinRadian(diffAngle);
        pVec->setLength(lenFrom * (1.0f - rate) + lenTo * rate);

        return true;
    }

    void vecRotAxis(const TVec3f& rFrom, const TVec3f& rTo, const TVec3f& rAxis, TVec3f* pVec, f32 angle) {
        f32 lenFrom = rFrom.length();
        f32 lenTo = rTo.length();
        f32 diffAngle;
        if (lenFrom == 0.0f || lenTo == 0.0f) {
            diffAngle = 0.0f;
        } else {
            diffAngle = diffAngleAbs(rFrom, rTo);
        }

        if (diffAngle == 0.0f) {
            *pVec = rTo;
            return;
        }

        if (diffAngle > angle) {
            if (diffAngle < pi() && rFrom.cross(rTo).dot(rAxis) < 0.0f) {
                angle = -angle;
            }

            TPos3f rot;
            PSMTXRotAxisRad(rot, rAxis, angle);
            PSMTXMultVec(rot, rFrom, pVec);
            return;
        }

        *pVec = rTo;
        return;
    }

    void blendColor(GXColor* pDst, const GXColor& rStart, const GXColor& rEnd, f32 t) {
        pDst->r = getInterpolateValue(t, rStart.r, rEnd.r);
        pDst->g = getInterpolateValue(t, rStart.g, rEnd.g);
        pDst->b = getInterpolateValue(t, rStart.b, rEnd.b);
        pDst->a = getInterpolateValue(t, rStart.a, rEnd.a);
    }

    void blendVec(Vec* pDst, const Vec& rStart, const Vec& rEnd, f32 t) {
        pDst->x = getInterpolateValue(t, rStart.x, rEnd.x);
        pDst->y = getInterpolateValue(t, rStart.y, rEnd.y);
        pDst->z = getInterpolateValue(t, rStart.z, rEnd.z);
    }

    f32 turnVecToPlane(TVec3f* pDst, const TVec3f& rSrc, const TVec3f& rNorm) {
        TVec3f vec = rSrc;
        f32 length = rSrc.length();
        f32 proj = rNorm.dot(rSrc);
        vec.sub(rNorm * proj);
        vec.setLength(length);
        pDst->set(vec);
        return length;
    }

    u32 getMinAbsElementIndex(const TVec3f& rVec) {
        f64 abs_x = abs(rVec.x);
        f64 abs_y = abs(rVec.y);
        f64 abs_z = abs(rVec.z);

        if (abs_x < abs_y && abs_x < abs_z) {
            return 0;
        }

        if (abs_y < abs_z) {
            return 1;
        }

        return 2;
    }

    f32 getMaxElement(const TVec3f& rVec) {
        f32* vec_arr = (f32*)(&rVec);
        return vec_arr[getMaxElementIndex(rVec)];
    }

    f32 getMaxAbsElement(const TVec3f& rVec) {
        f32* vec_arr = (f32*)(&rVec);
        return vec_arr[getMaxAbsElementIndex(rVec)];
    }

    u32 getMaxElementIndex(const TVec3f& rVec) {
        if (rVec.x > rVec.y && rVec.x > rVec.z) {
            return 0;
        }

        if (rVec.y > rVec.z) {
            return 1;
        }

        return 2;
    }

    u32 getMaxAbsElementIndex(const TVec3f& rVec) {
        f64 abs_x = abs(rVec.x);
        f64 abs_y = abs(rVec.y);
        f64 abs_z = abs(rVec.z);

        if (abs_x > abs_y && abs_x > abs_z) {
            return 0;
        }

        if (abs_y > abs_z) {
            return 1;
        }

        return 2;
    }

    f32 diffAngleAbsFast(const TVec3f& rA, const TVec3f& rB) {
        return MR::acos(rA.dot(rB));
    }

    f32 diffAngleAbs(const TVec3f& rA, const TVec3f& rB) {
        f32 length1 = rA.length();
        f32 length2 = rB.length();
        f32 x = rA.dot(rB) / (length1 * length2);

        if (x >= 1.0f) {
            return 0.0f;
        } else if (x <= -1.0f) {
            return PI;
        } else {
            return acosEx(x);
        }
    }

    f32 diffAngleAbs(const TVec2f& rA, const TVec2f& rB) {
        f32 length1 = rA.length();
        f32 length2 = rB.length();
        f32 x = rA.dot(rB) / (length1 * length2);

        if (x >= 1.0f) {
            return 0.0f;
        } else if (x <= -1.0f) {
            return PI;
        } else {
            return acosEx(x);
        }
    }

    f32 diffAngleSigned(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis) {
        f32 angleDiff = diffAngleAbs(rA, rB);

        TVec3f v = rA.cross(rAxis);

        if (v.dot(rB) >= 0.0f) {
            return angleDiff;
        } else {
            return -angleDiff;
        }
    }

    f32 diffAngleAbsHorizontal(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis) {
        TVec3f horizonA, horizonB;

        vecKillElement(rA, rAxis, &horizonA);
        vecKillElement(rB, rAxis, &horizonB);

        return diffAngleAbs(horizonA, horizonB);
    }

    f32 diffAngleSignedHorizontal(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis) {
        TVec3f horizonA, horizonB;
        vecKillElement(rA, rAxis, &horizonA);
        vecKillElement(rB, rAxis, &horizonB);
        f32 angleAbs = diffAngleAbs(horizonA, horizonB);
        TPos3f mtx;
        PSMTXRotAxisRad(mtx, rAxis, 0.00017453293f);  // TODO: written directly?
        f32 angle1 = horizonA.dot(horizonB);
        PSMTXMultVec(mtx, horizonA, horizonA);
        f32 angle2 = horizonA.dot(horizonB);

        if (angle2 > angle1) {
            return angleAbs;
        } else {
            return -angleAbs;
        }
    }

    bool isNearAngleRadian(const TVec3f& rA, const TVec3f& rB, f32 angle) {
        if (isNearZero(rA) || isNearZero(rB)) {
            return false;
        }

        TVec3f normA;
        normalize(rA, &normA);

        TVec3f normB;
        normalize(rB, &normB);

        return normA.dot(normB) >= JMACosRadian(angle);
    }

    bool isNearAngleDegree(const TVec3f& rA, const TVec3f& rB, f32 angle) {
        return isNearAngleRadian(rA, rB, angle * (PI / 180.0f));
    }

    bool isNearAngleRadianHV(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis, f32 angleH, f32 angleV) {
        if (angleV > HALF_PI_D) {  // TODO: value written directly
            angleV = HALF_PI;
        }

        TVec3f unitA;
        if (normalizeOrZero(rA, &unitA)) {
            return false;
        }

        TVec3f unitB;
        if (normalizeOrZero(rB, &unitB)) {
            return false;
        }

        TVec3f unitAxis;
        if (normalizeOrZero(rAxis, &unitAxis)) {
            return false;
        }

        TVec3f horizonA;
        horizonA.killElement(rA, rAxis);
        normalizeOrZero(&horizonA);

        if (horizonA.dot(unitB) < JMACosRadian(angleH)) {
            return false;
        }

        if (MR::abs(unitA.dot(unitAxis)) > MR::abs(JMASinRadian(angleV))) {
            return false;
        }

        return true;
    }

    bool isNearAngleDegreeHV(const TVec3f& rA, const TVec3f& rB, const TVec3f& rAxis, f32 angleH, f32 angleV) {
        angleV = PI_180 * angleV;
        angleH = PI_180 * angleH;

        return isNearAngleRadianHV(rA, rB, rAxis, angleH, angleV);
    }

    void createBoundingBox(const TVec3f* pPoints, u32 numPoints, TVec3f* pMin, TVec3f* pMax) {
        *pMin = pPoints[0];
        *pMax = pPoints[0];

        for (u32 idx = 1; idx < numPoints; idx++) {
            const TVec3f& point = pPoints[idx];

            if (point.x < pMin->x) {
                pMin->x = point.x;
            }
            if (point.y < pMin->y) {
                pMin->y = point.y;
            }
            if (point.z < pMin->z) {
                pMin->z = point.z;
            }

            if (pMax->x < point.x) {
                pMax->x = point.x;
            }
            if (pMax->y < point.y) {
                pMax->y = point.y;
            }
            if (pMax->z < point.z) {
                pMax->z = point.z;
            }
        }
    }

    bool isNormalize(const TVec3f& rVec, f32 tolerance) {
        return MR::abs(1.0f - rVec.length()) <= tolerance;
    }

    // Matches with no consequences, but I am not exactly sure if *THIS* is what Nintendo would've done...
    void setNan(TVec3f& rDst) {
        JGeometry::TVec3< int >* tmp = (JGeometry::TVec3< int >*)&rDst;
        tmp->x = -1;
        tmp->y = -1;
        tmp->z = -1;
    }

    bool isNan(const TVec3f& rVec) {
        if (__fpclassifyf(rVec.x) == 1 || __fpclassifyf(rVec.y) == 1 || __fpclassifyf(rVec.z) == 1) {
            return true;
        }

        return false;
    }

    f32 getFootPoint(const TVec3f& rPointA, const TVec3f& rPointB, const TVec3f& rPos, TVec3f* pDst) {
        TVec3f segment = rPointB - rPointA;
        f32 length = segment.length();
        f32 proj = -(segment.x * (rPointA.x - rPos.x) + segment.y * (rPointA.y - rPos.y) + segment.z * (rPointA.z - rPos.z)) / (length * length);
        *pDst = rPointA + segment * proj;
        return proj;
    }

    f32 mod(f32 x, f32 y) {
        return fmod(x, y);
    }

    void floatToFixed16(TVec3s* pDst, const TVec3f& pSrc, u8 q) {
        f32 scale = 1 << q;

        pDst->x = pSrc.x * scale;
        pDst->y = pSrc.y * scale;
        pDst->z = pSrc.z * scale;
    }

    void fixed16ToFloat(TVec3f* pDst, const TVec3s& pSrc, u8 q) {
        f32 scale = 1.0f / (1 << q);

        pDst->x = pSrc.x * scale;
        pDst->y = pSrc.y * scale;
        pDst->z = pSrc.z * scale;
    }

    void getRotatedAxisY(TVec3f* pDst, const TVec3f& pSrc) {
        f32 var = pSrc.x;
        f32 CosX = JMACosDegree(var);
        var = pSrc.y;
        f32 CosY = JMACosDegree(var);
        var = pSrc.z;
        f32 CosZ = JMACosDegree(var);
        var = pSrc.x;
        f32 SinX = JMASinDegree(var);
        var = pSrc.y;
        f32 SinY = JMASinDegree(var);
        var = pSrc.z;
        f32 SinZ = JMASinDegree(var);

        pDst->set((SinX * (CosZ * SinY)) - (SinZ * CosX), (SinX * (SinZ * SinY)) + (CosZ * CosX), CosY * SinX);
    }

    void getRotatedAxisZ(TVec3f* pDst, const TVec3f& pSrc) {
        TVec3f vec(0.0f, 0.0f, 1.0f);
        TPos3f mtx;
        MR::makeMtxTR(mtx, 0.0f, 0.0f, 0.0f, pSrc.x, pSrc.y, pSrc.z);
        PSMTXMultVec(mtx, &vec, pDst);
    }
};  // namespace MR
