#include "Game/MapObj/WormEatenPlanet.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/GreenCaterpillarBig.hpp"
#include "Game/Util.hpp"
#include <cstdio>

namespace {
    static const s32 sNumWormEatenHill = 5;
};  // namespace

namespace NrvWormEatenPlanet {
    NEW_NERVE(WormEatenPlanetNrvFirstWait, WormEatenPlanet, FirstWait);
    NEW_NERVE(WormEatenPlanetNrvSecondWait, WormEatenPlanet, SecondWait);
    NEW_NERVE(WormEatenPlanetNrvThirdWait, WormEatenPlanet, ThirdWait);
    NEW_NERVE(WormEatenPlanetNrvFourthWait, WormEatenPlanet, FourthWait);
};  // namespace NrvWormEatenPlanet

WormEatenPlanet::WormEatenPlanet(const char* pName)
    : LiveActor(pName), mCaterpillar(), mWormEatenHill(), mSecondHill(), mThirdHill(), mFourthHill(), mLodCtrl() {
}

void WormEatenPlanet::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WormEatenPlanet", nullptr, false);
    MR::connectToScenePlanet(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, "First", getSensor(nullptr), nullptr);
    mSecondHill = MR::createCollisionPartsFromLiveActor(this, "Second", getSensor(nullptr), MR::CollisionScaleType_Unk2);
    mThirdHill = MR::createCollisionPartsFromLiveActor(this, "Third", getSensor(nullptr), MR::CollisionScaleType_Unk2);
    mFourthHill = MR::createCollisionPartsFromLiveActor(this, "Fourth", getSensor(nullptr), MR::CollisionScaleType_Unk2);
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

void WormEatenPlanet::exeFirstWait() {
}

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
    mWormEatenHill = new PartsModel*[::sNumWormEatenHill];

    for (s32 i = 0; i < ::sNumWormEatenHill; i++) {
        char jointName[256];
        snprintf(jointName, sizeof(jointName), "ConnectParts%c", i + 'A');
        MtxPtr jointMtx = MR::getJointMtx(this, jointName);

        mWormEatenHill[i] = new PartsModel(this, "食い破り塚", "WormEatenHill", jointMtx, -1, false);
        mWormEatenHill[i]->initWithoutIter();
        mWormEatenHill[i]->makeActorDead();
    }
}

bool WormEatenPlanet::tryGenerateWormEatenHill() {
    for (s32 i = 0; i < ::sNumWormEatenHill; i++) {
        if (!MR::isDead(mWormEatenHill[i])) {
            continue;
        }

        mWormEatenHill[i]->appear();
        MR::emitEffect(mWormEatenHill[i], "Appear");

        return true;
    }

    return false;
}

void WormEatenPlanet::startSecondDemo() {
    MR::requestStartTimeKeepDemo(this, "ムイムイ出現", nullptr, nullptr, "オオムイムイ出現２回目");
}

void WormEatenPlanet::startThirdDemo() {
    MR::requestStartTimeKeepDemo(this, "ムイムイ出現", nullptr, nullptr, "オオムイムイ出現３回目");
}
