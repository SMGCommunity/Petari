#include "Game/MapObj/RosettaChair.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

RosettaChair::RosettaChair(const char* pName) : LiveActor(pName), mDefaultPosition(0.0f, 0.0f, 0.0f), mDefaultRotation(0.0f, 0.0f, 0.0f) {
    mScaleMtx.identity();
}

void RosettaChair::setDefaultPose() {
    mPosition.set(mDefaultPosition);
    mRotation.set(mDefaultRotation);
    MR::startBck(this, "RosettaChair", nullptr);
    MR::validateCollisionParts(this);
}

void RosettaChair::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("RosettaChair", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);

    mScaleMtx.set(getBaseMtx());
    mScaleMtx.scaleXYZ(1.7f);
    MR::initCollisionPartsAutoEqualScale(this, "RosettaChair", getSensor("body"), mScaleMtx);

    MR::setClippingTypeSphere(this, 500.0f);
    MR::tryRegisterDemoCast(this, rIter);
    MR::registerDemoActionFunctor(this, MR::Functor(this, &RosettaChair::startDemo), "朗読開始");
    MR::registerDemoActionFunctor(this, MR::Functor(this, &RosettaChair::setDefaultPose), "キャスト入れ換え");
    mDefaultPosition.set(mPosition);
    mDefaultRotation.set(mRotation);
    MR::startBck(this, "RosettaChair", nullptr);
    makeActorAppeared();
}

void RosettaChair::startDemo() {
    MR::startBck(this, "DemoRosettaReading", nullptr);
    MR::invalidateCollisionParts(this);
}
