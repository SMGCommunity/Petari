#include "Game/Camera/CameraInwardSphere.hpp"
#include "Game/Camera/CamTranslatorInwardSphere.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/TriangleFilter.hpp"

void CameraInwardSphere_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    static const f32 sBaseDist = 300.0f;
};  // namespace

CameraInwardSphere::~CameraInwardSphere() {
}

CameraInwardSphere::CameraInwardSphere(const char* pName)
    : Camera(pName), mDistMax(1500.0f), mDistMin(500.0f), mDistBase(::sBaseDist), mVelocity(0.0f, 0.0f, 0.0f), mPos(0.0f, 0.0f, 0.0f) {
}

void CameraInwardSphere::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    calcGravityCenter();
    f32 dist = CameraLocalUtil::getTarget(this)->getPosition().distance(mGravityCenter);
    TVec3f offset = CameraLocalUtil::getPos(this) - mGravityCenter;
    offset.setLength(dist - mDistBase);
    CameraLocalUtil::setPos(this, mGravityCenter + offset);
    mVelocity.zero();
    mPos.set(CameraLocalUtil::getPos(this));
}

CameraTargetObj* CameraInwardSphere::calc() {
    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    CameraLocalUtil::setWatchPos(this, watchPoint);

    TVec3f camOffset = mPos - CameraLocalUtil::getWatchPos(this);
    f32 camDist = camOffset.length();
    f32 dist;
    if (camDist < 1.0f) {
        camOffset.set(-CameraLocalUtil::getTarget(this)->getFrontVec());
        dist = mDistMin;
    } else {
        MR::normalize(&camOffset);
        dist = camDist;
        if (dist < mDistMin) {
            dist = mDistMin;
        } else if (dist > mDistMax) {
            dist = mDistMax;
        }
    }
    camOffset.scale(dist);

    TVec3f camOffsetPos = camOffset + CameraLocalUtil::getWatchPos(this);

    TVec3f grav;
    MR::calcGravityVector(this, camOffsetPos, &grav, nullptr, 0);

    Triangle tri;
    TriangleFilterFunc filterFunc(&MR::isCameraCodeThrough);

    TVec3f newPos;
    TVec3f mapPos;
    if (MR::getFirstPolyOnLineToMap(&mapPos, &tri, camOffsetPos - grav * 1000.0f, grav * 2000.0f, nullptr, &filterFunc)) {
        newPos = mapPos - grav * mDistBase;
    } else {
        newPos = camOffsetPos;
    }

    mVelocity.add(grav * 0.7f);
    if (mVelocity.length() > 35.0f) {
        mVelocity.setLength(35.0f);
    }
    newPos.add(mVelocity);

    f32 maxDist = 0.0f;
    s32 collisions = Collision::checkStrikeBallToMap(newPos, mDistBase, nullptr, nullptr);
    for (u32 idx = 0; idx < collisions; idx++) {
        const HitInfo* info = Collision::getStrikeInfoMap(idx);
        f32 dot = grav.dot(*info->mParentTriangle.getNormal(0) * info->_60);
        if (maxDist < -dot) {
            maxDist = -dot;
        }
    }
    if (maxDist > 0.0f) {
        mVelocity.zero();
    }
    newPos.add(-grav * maxDist);

    mPos.set(newPos);

    TVec3f upPos = newPos - grav * 1000.0f;
    f32 oldDist = (CameraLocalUtil::getPos(this) - upPos).length();
    f32 newDist = (newPos - upPos).length();

    newDist = oldDist * 0.9f + newDist * 0.1f;
    CameraLocalUtil::setPos(this, upPos + grav * newDist);

    TVec3f up = CameraLocalUtil::getUpVec(this) * 0.9f - grav * 0.1f;
    MR::normalize(&up);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraInwardSphere::createTranslator() {
    return new CamTranslatorInwardSphere(this);
}

void CameraInwardSphere::calcGravityCenter() {
    // Sample two (close) points along the gravity field, and the intersections
    // of the two gravity vectors will produce the center of gravity
    TVec3f up1;
    TVec3f pos1 = CameraLocalUtil::getTarget(this)->getPosition();
    MR::calcGravityVector(this, pos1, &up1, nullptr, 0);
    up1.negate();

    TVec3f pos2 = CameraLocalUtil::getTarget(this)->getFrontVec();
    pos2.orthogonalize(up1);

    if (MR::isNearZero(pos2)) {
        pos2 = CameraLocalUtil::getTarget(this)->getSideVec();
        pos2.orthogonalize(up1);
    }
    pos2.setLength(1000.0f);
    pos2.add(CameraLocalUtil::getTarget(this)->getPosition());

    TVec3f up2;
    MR::calcGravityVector(this, pos2, &up2, nullptr, 0);
    up2.negate();

    f32 sq1 = up1.squared();
    f32 sq2 = up2.squared();
    f32 dot = up1.dot(up2);
    f32 dotSq = dot * dot;
    TVec3f front1 = pos1 - pos2;
    TVec3f front2 = pos2 - pos1;
    f32 proj1 = front1.dot(up2);
    f32 proj2 = front2.dot(up1);
    f32 cosDiff = sq1 * sq2 - dotSq;
    if (cosDiff < 0.001f) {
        mGravityCenter = CameraLocalUtil::getTarget(this)->getPosition();
    } else {
        f32 dist = 1.0f / cosDiff * (sq2 * proj2 + dot * proj1);
        mGravityCenter = pos1 + up1 * dist;
    }
}
