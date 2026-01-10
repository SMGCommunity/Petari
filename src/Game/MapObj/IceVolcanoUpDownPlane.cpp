#include "Game/MapObj/IceVolcanoUpDownPlane.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/RailMoveObj.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"

IceVolcanoUpDownPlane::IceVolcanoUpDownPlane(const char* pName) : RailMoveObj(pName), _D4(0.0f, 0.0f, 0.0f) {}

void IceVolcanoUpDownPlane::move() {
    bool success = tryCalcNearestPosToPlayer(&_D4);

    if (isMoving() && success) {
        MR::StageEffect::tryStageEffectMoving(this, mObjectName);
    }
}

bool IceVolcanoUpDownPlane::tryCalcNearestPosToPlayer(TVec3f* pArg) const {
    TVec3f* playerPos = MR::getPlayerPos();

    // f1 f2 paired single regswap
    TVec3f vec;
    JMathInlineVEC::PSVECSubtract2(playerPos, &mPosition, &vec);

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    if (vec.dot(upVec) <= -380.0f) {
        return false;
    }

    TVec3f scaledAdded;
    JMAVECScaleAdd(&upVec, &vec, &scaledAdded, -upVec.dot(vec));

    f32 radius;
    MR::calcModelBoundingRadius(&radius, this);

    if (scaledAdded.length() > radius) {
        scaledAdded.setLength2(radius);
    }

    // f1 f2 paired single regswap
    JMathInlineVEC::PSVECAdd3(&scaledAdded, &mPosition, pArg);
    return true;
}

TVec3f* IceVolcanoUpDownPlane::getSoundCalcPos() {
    return &_D4;
}

void RailMoveObj::setupInitInfo(const JMapInfoIter& rIter, MapObjActorInitInfo* pActorInitInfo) {}
