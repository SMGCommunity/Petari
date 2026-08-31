#include "Game/MapObj/MapObjConnector.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"

namespace {
    const f32 sDepth = 500.0f;
    const f32 sOffset = 50.0f;
};  // namespace

MapObjConnector::MapObjConnector(LiveActor* pActor) : mHost(pActor), mParts() {
    _8.identity();
}

bool MapObjConnector::attachToUnder() {
    TVec3f upVec;
    MR::calcUpVec(&upVec, mHost);

    return attach(upVec);
}

bool MapObjConnector::attachToBack() {
    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, mHost);

    return attach(frontVec);
}

bool MapObjConnector::attach(const TVec3f& rVec) {
    Triangle triangle = Triangle();

    TVec3f vec;
    if (!MR::getFirstPolyOnLineToMapExceptActor(&vec, &triangle, mHost->mPosition + rVec * ::sOffset, -rVec * ::sDepth, mHost)) {
        return false;
    }

    mParts = triangle.mParts;

    TPos3f baseMtx;
    baseMtx.set(mHost->getBaseMtx());

    TPos3f invBaseMtx;
    invBaseMtx.set(triangle.mParts->mInvBaseMatrix);
    _8.concat(invBaseMtx, baseMtx);

    return true;
}

void MapObjConnector::connect() {
    connect(mHost);
}

void MapObjConnector::connect(LiveActor* pActor) {
    if (mParts == nullptr) {
        return;
    }

    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.set(mParts->mBaseMatrix);
    baseMtx.concat(baseMtx, _8);
    baseMtx.getTrans(pActor->mPosition);

    MR::setBaseTRMtx(pActor, baseMtx);
}
