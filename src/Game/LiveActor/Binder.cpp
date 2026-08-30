#include "Game/LiveActor/Binder.hpp"
#include "Game/Map/HitInfo.hpp"

HitInfo& HitInfo::operator=(const HitInfo& rOther) {
    mParentTriangle.mParts = rOther.mParentTriangle.mParts;
    mParentTriangle.mIdx = rOther.mParentTriangle.mIdx;
    mParentTriangle.mSensor = rOther.mParentTriangle.mSensor;
    mParentTriangle.mNormals[0] = rOther.mParentTriangle.mNormals[0];
    mParentTriangle.mNormals[1] = rOther.mParentTriangle.mNormals[1];
    mParentTriangle.mNormals[2] = rOther.mParentTriangle.mNormals[2];
    mParentTriangle.mNormals[3] = rOther.mParentTriangle.mNormals[3];
    mParentTriangle.mPos[0] = rOther.mParentTriangle.mPos[0];
    mParentTriangle.mPos[1] = rOther.mParentTriangle.mPos[1];
    mParentTriangle.mPos[2] = rOther.mParentTriangle.mPos[2];
    _60 = rOther._60;
    mHitPos = rOther.mHitPos;
    _70 = rOther._70;
    _7C = rOther._7C;
    _88 = rOther._88;

    return *this;
}

Binder::Binder(MtxPtr mtx, const TVec3f* v1, const TVec3f* v2, f32 radius, f32 offsetY, u32 planeNum)
    : mTriangleFilter(), mCollisionPartsFilter(), mExCollisionParts(), _C(mtx), _10(v1), _14(v2), mRadius(radius), mOffsetY(offsetY), mOffsetVec(),
      _24(planeNum), mPlaneNum(), mPlane(), mFixReactionVector(0, 0, 0), mGroundInfo(), mWallInfo(), mRoofInfo(), _C8(), _158(), _1E8() {
    if (_24 == 0) {
        mPlane = nullptr;
    } else {
        mPlane = new HitInfo[_24];
    }

    clear();
    _1EC._0 = true;
    _1EC._1 = true;
    _1EC._2 = false;
    _1EC._3 = false;
    _1EC._4 = false;
    _1EC._5 = false;
}

void Binder::setTriangleFilter(TriangleFilterBase* pFilter) {
    mTriangleFilter = pFilter;
}

void Binder::setCollisionPartsFilter(CollisionPartsFilterBase* pFilter) {
    mCollisionPartsFilter = pFilter;
}

void Binder::clear() {
    mPlaneNum = 0;
    _C8 = -99999.0f;
    _158 = -99999.0f;
    _1E8 = -99999.0f;
    mFixReactionVector.zero();
}

const HitInfo* Binder::getPlane(int index) const {
    return &mPlane[index];
}

bool Binder::compSensor(HitInfo* pPlane1, HitInfo* pPlane2) {
    return pPlane1->mParentTriangle.mSensor > pPlane2->mParentTriangle.mSensor;
}
