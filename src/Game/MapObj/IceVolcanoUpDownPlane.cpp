#include "Game/MapObj/IceVolcanoUpDownPlane.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/RailMoveObj.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

IceVolcanoUpDownPlane::IceVolcanoUpDownPlane(const char* pName) : RailMoveObj(pName), mNearestPosToPlayer(0.0f, 0.0f, 0.0f) {
}

void IceVolcanoUpDownPlane::move() {
    bool success = tryCalcNearestPosToPlayer(&mNearestPosToPlayer);

    if (isMoving() && success) {
        MR::StageEffect::tryStageEffectMoving(this, mObjectName);
    }
}

bool IceVolcanoUpDownPlane::tryCalcNearestPosToPlayer(TVec3f* pArg) const {
    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), mPosition);

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    if (vec.dot(upVec) <= -380.0f) {
        return false;
    }

    TVec3f scaledAdded = vec.killElement(upVec);

    f32 radius;
    MR::calcModelBoundingRadius(&radius, this);

    if (scaledAdded.length() > radius) {
        scaledAdded.setLength(radius);
    }

    pArg->add(scaledAdded, mPosition);
    return true;
}

TVec3f* IceVolcanoUpDownPlane::getSoundCalcPos() {
    return &mNearestPosToPlayer;
}

void RailMoveObj::setupInitInfo(const JMapInfoIter& rIter, MapObjActorInitInfo* pActorInitInfo) {
}
