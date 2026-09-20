#include "Game/MapObj/WhirlPool.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

WhirlPool::WhirlPool(const char* pName) : LiveActor(pName), _8C(100.0f), _90(100.0f), _C4(), _C8(), _94(0.0f, 1.0f, 0.0f), _CC(0.0f, 0.0f, 0.0f) {
}

void WhirlPool::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_WhirlPoolAccelerator);
    mPosition.set(*MR::getPlayerPos());
    mPosition.y = 1500.0f;
    MR::initDefaultPos(this, rIter);
    _8C = mScale.x * 100.0f;
    _90 = mScale.y * 100.0f;
    MR::calcActorAxisY(&_94, this);
    initPoints();
    _C4 = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "Water.bti"), 0);
    _C8 = new JUTTexture(MR::loadTexFromArc("WaterWave.arc", "WaterIndirect.bti"), 0);
    initSound(4, false);
    _CC.set(mPosition);
    _CC.y -= _8C / 2.0f;
    MR::setClippingTypeSphere(this, _8C, &_CC);
    WaterAreaFunction::entryWhirlPool(this);
    makeActorAppeared();
}

void WhirlPool::movement() {
    MR::startLevelSound(this, "SE_AT_LV_WHIRL_POOL");
}
