#include "Game/Enemy/MogucchiRefuseTerritory.hpp"
#include "Game/Util.hpp"

MogucchiRefuseTerritory::MogucchiRefuseTerritory(const char* pName) : LiveActor(pName) {
}
MogucchiRefuseTerritory::~MogucchiRefuseTerritory() {
}

void MogucchiRefuseTerritory::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneEnemyMovement(this);
    initHitSensor(1);
    MR::addHitSensor(this, "body", 0x20, 8, (100.0f * mScale.x), TVec3f(0.0f));
    makeActorAppeared();
}
