#include "Game/MapObj/ItemAppearStone.hpp"

ItemAppearStone::ItemAppearStone(const char *pName) : LiveActor(pName) {

}

void ItemAppearStone::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    const char* model_name = 0;
    MR::getObjectName(&model_name, rIter);
    initModelManagerWithAnm(model_name, 0, false);
    MR::connectToSceneMapObj(this);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    initHitSensor(1);
    HitSensor* sensor = MR::addBodyMessageSensorMapObjMoveCollision(this);
    MR::initCollisionParts(this, model_name, sensor, MR::getJointMtx(this, "Move"));
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::needStageSwitchReadB(this, rIter);
    initNerve(&NrvItemAppearStone::HostTypeWait::sInstance);
    makeActorAppeared();
    MR::startBck(this, model_name, 0);
}

void ItemAppearStone::kill() {
    LiveActor::kill();
}

void ItemAppearStone::calcAndSetBaseMtx() {

}

void ItemAppearStone::exeWait() {
    MR::startLevelSound(this, "SE_OJ_LV_ITEM_APR_STONE_MV", -1, -1, -1);

    if (MR::isOnSwitchB(this)) {
        MR::startSound(this, "SE_OJ_ITEM_APR_STONE_BREAK", -1, -1);
        kill();
    }
}

ItemAppearStone::~ItemAppearStone() {

}

namespace NrvItemAppearStone {
    INIT_NERVE(HostTypeWait);

    void HostTypeWait::execute(Spine *pSpine) const {
        ItemAppearStone* stone = reinterpret_cast<ItemAppearStone*>(pSpine->mExecutor);
        stone->exeWait();
    }
};