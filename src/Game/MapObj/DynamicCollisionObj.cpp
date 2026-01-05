#include "Game/MapObj/DynamicCollisionObj.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/KCollision.hpp"
#include "Game/Scene/SceneFunction.hpp"

DynamicCollisionObj::DynamicCollisionObj(const char* pName) : LiveActor(pName) {
    _A4 = 0;
    _9C = 0;
    _11C = 0;
    _A8 = 0;
    _AC = 0;
}

void DynamicCollisionObj::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, MR::MovementType_Enemy, MR::CalcAnimType_Enemy, -1, MR::DrawType_FlexibleSphere);

    _8C = nullptr;
}

void DynamicCollisionObj::syncCollision() {
    updateCollisionHeader();
    updateTriangle();
    mParts->mServer->calcFarthestVertexDistance();
    mParts->updateBoundingSphereRange(mScale);
}
