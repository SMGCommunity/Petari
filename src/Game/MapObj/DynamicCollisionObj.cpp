#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/KCollision.hpp"
#include "Game/MapObj/DynamicCollisionObj.hpp"

DynamicCollisionObj::DynamicCollisionObj(const char *pName) : LiveActor(pName) {
    _A4 = 0;
    _9C = 0;
    _11C = 0;
    _A8 = 0;
    _AC = 0;
}

void DynamicCollisionObj::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, 42, 8, -1, 25);

    _8C = nullptr;
}

void DynamicCollisionObj::syncCollision() {
    updateCollisionHeader();
    updateTriangle();
    mParts->mServer->calcFarthestVertexDistance();
    mParts->updateBoundingSphereRange(mScale);
}