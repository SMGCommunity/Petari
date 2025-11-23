#include "Game/MapObj/WormEatenPlanet.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/GreenCaterpillarBig.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "cstdio"

namespace NrvWormEatenPlanet {
    NEW_NERVE(WormEatenPlanetNrvFirstWait, WormEatenPlanet, FirstWait);
    NEW_NERVE(WormEatenPlanetNrvSecondWait, WormEatenPlanet, SecondWait);
    NEW_NERVE(WormEatenPlanetNrvThirdWait, WormEatenPlanet, ThirdWait);
    NEW_NERVE(WormEatenPlanetNrvFourthWait, WormEatenPlanet, FourthWait);
};  // namespace NrvWormEatenPlanet

WormEatenPlanet::WormEatenPlanet(const char* pName) : LiveActor(pName) {
    mCaterpillar = nullptr;
    mEatenHills = nullptr;
    mSecondHill = nullptr;
    mThirdHill = nullptr;
    mFourthHill = nullptr;
    mLodCtrl = nullptr;
}

void WormEatenPlanet::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WormEatenPlanet", nullptr, false);
    MR::connectToScenePlanet(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "First", getSensor(nullptr), nullptr);
    mSecondHill = MR::createCollisionPartsFromLiveActor(this, "Second", getSensor(nullptr), (MR::CollisionScaleType)2);
    mThirdHill = MR::createCollisionPartsFromLiveActor(this, "Third", getSensor(nullptr), (MR::CollisionScaleType)2);
    mFourthHill = MR::createCollisionPartsFromLiveActor(this, "Fourth", getSensor(nullptr), (MR::CollisionScaleType)2);
    MR::invalidateCollisionParts(mSecondHill);
    MR::invalidateCollisionParts(mThirdHill);
    MR::invalidateCollisionParts(mFourthHill);
    MR::tryCreateCollisionMoveLimit(this, getSensor("body"));
    initEffectKeeper(0, nullptr, false);
    f32 radius;
    MR::calcModelBoundingRadius(&radius, this);
    radius += 1000.0f;
    MR::setClippingTypeSphere(this, radius);
    MR::setClippingFarMax(this);
    MR::needStageSwitchReadA(this, rIter);
    MR::needStageSwitchReadB(this, rIter);
    MR::listenStageSwitchOnA(this, MR::Functor(this, &WormEatenPlanet::startSecondDemo));
    MR::listenStageSwitchOnB(this, MR::Functor(this, &WormEatenPlanet::startThirdDemo));
    mCaterpillar = new GreenCaterpillarBig("虫食い惑星オオムイムイ");
    mCaterpillar->init(rIter);
    initWormEatenHill();
    mLodCtrl = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mCaterpillar, rIter);
        MR::registerDemoActionNerve(this, &NrvWormEatenPlanet::WormEatenPlanetNrvSecondWait::sInstance, "オオムイムイ出現１回目");
        MR::registerDemoActionNerve(this, &NrvWormEatenPlanet::WormEatenPlanetNrvThirdWait::sInstance, "オオムイムイ出現２回目");
        MR::registerDemoActionNerve(this, &NrvWormEatenPlanet::WormEatenPlanetNrvFourthWait::sInstance, "オオムイムイ出現３回目");
    }

    initNerve(&NrvWormEatenPlanet::WormEatenPlanetNrvFirstWait::sInstance);
    makeActorAppeared();
}

void WormEatenPlanet::exeFirstWait() {}

void WormEatenPlanet::exeSecondWait() {
    if (MR::isFirstStep(this)) {
        mCaterpillar->startWriggle();
        MR::validateCollisionParts(mSecondHill);
        MR::invalidateCollisionParts(this);
    }

    if (mCaterpillar->_9C) {
        tryGenerateWormEatenHill();
    }

    if (mCaterpillar->_9D) {
        tryGenerateWormEatenHill();
    }
}

void WormEatenPlanet::exeThirdWait() {
    if (MR::isFirstStep(this)) {
        mCaterpillar->startWriggle();
        MR::validateCollisionParts(mThirdHill);
        MR::invalidateCollisionParts(mSecondHill);
    }

    if (mCaterpillar->_9C) {
        tryGenerateWormEatenHill();
    }

    if (mCaterpillar->_9D) {
        tryGenerateWormEatenHill();
    }
}

void WormEatenPlanet::exeFourthWait() {
    if (MR::isFirstStep(this)) {
        mCaterpillar->startWriggle();
        MR::validateCollisionParts(mFourthHill);
        MR::invalidateCollisionParts(mThirdHill);
    }

    if (mCaterpillar->_9D) {
        tryGenerateWormEatenHill();
    }
}

void WormEatenPlanet::control() {
    mLodCtrl->update();
}

void WormEatenPlanet::startClipped() {
    MR::validateClipping(mCaterpillar);
    LiveActor::startClipped();
}

void WormEatenPlanet::endClipped() {
    MR::invalidateClipping(mCaterpillar);
    LiveActor::endClipped();
}

void WormEatenPlanet::initWormEatenHill() {
    mEatenHills = new PartsModel*[5];
    for (s32 i = 0; i < 5; i++) {
        char jointName[256];
        snprintf(jointName, sizeof(jointName), "ConnectParts%c", i + 0x41);
        MtxPtr jointMtx = MR::getJointMtx(this, jointName);
        mEatenHills[i] = new PartsModel(this, "食い破り塚", "WormEatenHill", jointMtx, -1, false);
        mEatenHills[i]->initWithoutIter();
        mEatenHills[i]->makeActorDead();
    }
}

bool WormEatenPlanet::tryGenerateWormEatenHill() {
    for (s32 i = 0; i < 5; i++) {
        if (MR::isDead(mEatenHills[i])) {
            mEatenHills[i]->appear();
            MR::emitEffect(mEatenHills[i], "Appear");
            return true;
        }
    }

    return false;
}

void WormEatenPlanet::startSecondDemo() {
    MR::requestStartTimeKeepDemo(this, "ムイムイ出現", nullptr, nullptr, "オオムイムイ出現２回目");
}

void WormEatenPlanet::startThirdDemo() {
    MR::requestStartTimeKeepDemo(this, "ムイムイ出現", nullptr, nullptr, "オオムイムイ出現３回目");
}
