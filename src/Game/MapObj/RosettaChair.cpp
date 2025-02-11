#include "Game/MapObj/RosettaChair.hpp"

RosettaChair::RosettaChair(const char *pName) : LiveActor(pName) {
    _8C.set(0.0f);
    _98.set(0.0f);
    _A0.identity();
}

void RosettaChair::setDefaultPose() {
    mPosition.set<f32>(_8C);
    mRotation.set<f32>(_98);
    MR::startBck(this, "RosettaChair", nullptr);
    MR::validateCollisionParts(this);
}

void RosettaChair::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("RosettaChair", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MtxPtr baseMtx = getBaseMtx();
    _A0.setInline(baseMtx);
    _A0.mMtx[0][0] *= 1.7f;
    _A0.mMtx[0][1] *= 1.7f;
    _A0.mMtx[0][2] *= 1.7f;
    _A0.mMtx[1][0] *= 1.7f;
    _A0.mMtx[1][1] *= 1.7f;
    _A0.mMtx[1][2] *= 1.7f;
    _A0.mMtx[2][0] *= 1.7f;
    _A0.mMtx[2][1] *= 1.7f;
    _A0.mMtx[2][2] *= 1.7f;
    MR::initCollisionPartsAutoEqualScale(this, "RosettaChair", getSensor("body"), _A0.toMtxPtr());
    MR::setClippingTypeSphere(this, 500.0f);
    MR::tryRegisterDemoCast(this, rIter);
    MR::registerDemoActionFunctor(this, MR::Functor(this, &RosettaChair::startDemo), "朗読開始");
    MR::registerDemoActionFunctor(this, MR::Functor(this, &RosettaChair::setDefaultPose), "キャスト入れ換え");
    _8C.set<f32>(mPosition);
    _98.set<f32>(mRotation);
    MR::startBck(this, "RosettaChair", nullptr);
    makeActorAppeared();
}

void RosettaChair::startDemo() {
    MR::startBck(this, "DemoRosettaReading", nullptr);
    MR::invalidateCollisionParts(this);
}

RosettaChair::~RosettaChair() {

}
