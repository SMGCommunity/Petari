#include "Game/MapObj/IceVolcanoUpDownPlane.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

void IceVolcanoUpDownPlane_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)JGeometry::TUtil< f32 >::epsilon();
    (void)-380.0f;
}

IceVolcanoUpDownPlane::IceVolcanoUpDownPlane(const char* pName) : RailMoveObj(pName), mNearestPosToPlayer(0.0f, 0.0f, 0.0f) {
}

void IceVolcanoUpDownPlane::move() {
    bool success = tryCalcNearestPosToPlayer(&mNearestPosToPlayer);

    if (isMoving() && success) {
        MR::StageEffect::tryStageEffectMoving(this, mObjectName);
    }
}

bool IceVolcanoUpDownPlane::tryCalcNearestPosToPlayer(TVec3f* pDst) const {
    TVec3f dirVec;
    dirVec.sub(*MR::getPlayerPos(), mPosition);

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    if (dirVec.dot(upVec) <= -380.0f) {
        return false;
    }

    TVec3f offset = dirVec.killElement(upVec);

    f32 radius;
    MR::calcModelBoundingRadius(&radius, this);

    if (offset.length() > radius) {
        offset.setLength(radius);
    }

    pDst->add(offset, mPosition);
    return true;
}

TVec3f* IceVolcanoUpDownPlane::getSoundCalcPos() {
    return &mNearestPosToPlayer;
}

IceVolcanoUpDownPlane::~IceVolcanoUpDownPlane() {
}

void RailMoveObj::setupInitInfo(const JMapInfoIter& rIter, MapObjActorInitInfo* pActorInitInfo) {
}
