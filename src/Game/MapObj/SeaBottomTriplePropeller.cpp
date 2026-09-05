#include "Game/MapObj/SeaBottomTriplePropeller.hpp"
#include "Game/GameAudio/AudSeKeeper.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util.hpp"

namespace NrvSeaBottomTriplePropeller {
    NEW_NERVE(SeaBottomTriplePropellerNrvWait, SeaBottomTriplePropeller, Wait);
    NEW_NERVE(SeaBottomTriplePropellerNrvBreak, SeaBottomTriplePropeller, Break);
};  // namespace NrvSeaBottomTriplePropeller

SeaBottomTriplePropeller::SeaBottomTriplePropeller(const char* pName) : LiveActor(pName), mAudSeKeeper() {
    for (u16 i = 0; i < 3; i++) {
        mPropellerCollision[i] = nullptr;
    }
}

void SeaBottomTriplePropeller::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SeaBottomTriplePropeller", nullptr, false);
    MR::connectToSceneCollisionMapObj(this);

    initHitSensor(1);
    MR::addMessageSensorMapObj(this, "body");

    MR::initCollisionParts(this, "PropellerCap", getSensor("body"), nullptr);
    mPropellerCollision[0] =
        MR::createCollisionPartsFromLiveActor(this, "Propeller", getSensor("body"), MR::getJointMtx(this, "Propeller1"), MR::CollisionScaleType_Unk2);
    mPropellerCollision[1] =
        MR::createCollisionPartsFromLiveActor(this, "Propeller", getSensor("body"), MR::getJointMtx(this, "Propeller2"), MR::CollisionScaleType_Unk2);
    mPropellerCollision[2] =
        MR::createCollisionPartsFromLiveActor(this, "Propeller", getSensor("body"), MR::getJointMtx(this, "Propeller3"), MR::CollisionScaleType_Unk2);

    MR::validateCollisionParts(mPropellerCollision[0]);
    MR::validateCollisionParts(mPropellerCollision[1]);
    MR::validateCollisionParts(mPropellerCollision[2]);

    initEffectKeeper(0, nullptr, false);
    initSound(4, false);

    mAudSeKeeper = new AudSeKeeper(this, 3);

    MR::addJointToSeKeeper(mAudSeKeeper, "Propeller1", nullptr);
    MR::addJointToSeKeeper(mAudSeKeeper, "Propeller2", nullptr);
    MR::addJointToSeKeeper(mAudSeKeeper, "Propeller3", nullptr);

    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, GET_NERVE(SeaBottomTriplePropeller, SeaBottomTriplePropellerNrvBreak), nullptr);
    }
    initNerve(GET_NERVE(SeaBottomTriplePropeller, SeaBottomTriplePropellerNrvWait));
    makeActorAppeared();
}

void SeaBottomTriplePropeller::control() {
    mAudSeKeeper->update();
}

void SeaBottomTriplePropeller::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SeaBottomTriplePropeller", nullptr);
    }

    MR::startLevelSoundSeKeeper(mAudSeKeeper, "Propeller1", "SE_OJ_LV_O_TRIPLE_PROP_MOVE");
    MR::startLevelSoundSeKeeper(mAudSeKeeper, "Propeller2", "SE_OJ_LV_O_TRIPLE_PROP_MOVE");
    MR::startLevelSoundSeKeeper(mAudSeKeeper, "Propeller3", "SE_OJ_LV_O_TRIPLE_PROP_MOVE");
}

void SeaBottomTriplePropeller::exeBreak() {
    kill();
}

void SeaBottomTriplePropeller::calcAnim() {
    LiveActor::calcAnim();
    for (u16 i = 0; i < 3; i++) {
        mPropellerCollision[i]->setMtx();
    }
}
