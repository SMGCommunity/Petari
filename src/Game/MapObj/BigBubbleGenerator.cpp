#include "Game/MapObj/BigBubbleGenerator.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BigBubbleHolder.hpp"
#include "Game/Ride/BigBubble.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static const f32 sActiveDistance = 1500.0f;
    static const f32 sNonActiveDistance = 2000.0f;
    static const f32 sGenerateOffsetY = 60.0f;
};  // namespace

namespace NrvBigBubbleGenerator {
    NEW_NERVE(BigBubbleGeneratorNrvNonActive, BigBubbleGenerator, NonActive);
    NEW_NERVE(BigBubbleGeneratorNrvActive, BigBubbleGenerator, Active);
};  // namespace NrvBigBubbleGenerator

BigBubbleGenerator::BigBubbleGenerator(const char* pName)
    : LiveActor(pName), mSpawnTimer(-1), mMinSize(1.0f), mMaxSize(1.0f), mIsObstruct(0), mShape(-1), mLimitterID(-1), mAttachedBubble(nullptr) {
    MR::createBigBubbleHolder();
}

void BigBubbleGenerator::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("AirBubbleGenerator", nullptr, false);
    MR::connectToSceneMapObj(this);

    initHitSensor(1);
    MR::addBodyMessageSensorReceiver(this);

    MR::initCollisionParts(this, "AirBubbleGenerator", getSensor(nullptr), nullptr);
    initNerve(&NrvBigBubbleGenerator::BigBubbleGeneratorNrvNonActive::sInstance);
    initFromJMapParam(rIter);
    makeActorAppeared();
}

void BigBubbleGenerator::initFromJMapParam(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &mSpawnTimer);
    if (MR::getJMapInfoArg1NoInit(rIter, &mMinSize)) {
        mMinSize /= 100.0f;
    }
    if (MR::getJMapInfoArg2NoInit(rIter, &mMaxSize)) {
        mMaxSize /= 100.0f;
    }
    MR::getJMapInfoArg3WithInit(rIter, &mLimitterID);
    MR::getJMapInfoArg4WithInit(rIter, &mShape);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnOffAppear(this, MR::Functor(this, &BigBubbleGenerator::onActive), MR::Functor(this, &BigBubbleGenerator::offActive));
    }
}

void BigBubbleGenerator::initAfterPlacement() {
    if (mLimitterID != -1) {
        MR::limitBigBubblePosition(&mPosition, 0.0f, mLimitterID);
    }
}

void BigBubbleGenerator::control() {
    if (mAttachedBubble == nullptr) {
        return;
    }

    if (MR::isDead(mAttachedBubble) || mAttachedBubble->mHost != this) {
        mAttachedBubble = nullptr;
    }
}

void BigBubbleGenerator::onActive() {
    if (isNerve(&NrvBigBubbleGenerator::BigBubbleGeneratorNrvNonActive::sInstance)) {
        MR::invalidateClipping(this);
        setNerve(&NrvBigBubbleGenerator::BigBubbleGeneratorNrvActive::sInstance);
    }
}

void BigBubbleGenerator::offActive() {
    if (isNerve(&NrvBigBubbleGenerator::BigBubbleGeneratorNrvActive::sInstance)) {
        MR::validateClipping(this);
        setNerve(&NrvBigBubbleGenerator::BigBubbleGeneratorNrvNonActive::sInstance);
    }
}

bool BigBubbleGenerator::isActiveRange() {
    return MR::isNearPlayerAnyTime(this, ::sActiveDistance);
}

bool BigBubbleGenerator::isNonActiveRange() {
    return !MR::isNearPlayerAnyTime(this, ::sNonActiveDistance);
}

bool BigBubbleGenerator::tryActive() {
    if (!MR::isValidSwitchAppear(this) && isActiveRange()) {
        MR::invalidateClipping(this);
        setNerve(&NrvBigBubbleGenerator::BigBubbleGeneratorNrvActive::sInstance);
        return true;
    }
    return false;
}

bool BigBubbleGenerator::tryNonActive() {
    if (!MR::isValidSwitchAppear(this) && isNonActiveRange()) {
        MR::validateClipping(this);
        setNerve(&NrvBigBubbleGenerator::BigBubbleGeneratorNrvNonActive::sInstance);
        return true;
    }
    return false;
}

void BigBubbleGenerator::exeNonActive() {
    tryActive();
}

void BigBubbleGenerator::exeActive() {
    bool spawn = false;
    bool attach = false;
    if (mSpawnTimer < 0) {
        if (mAttachedBubble == nullptr) {
            spawn = true;
            attach = true;
        }
    } else {
        if (getNerveStep() % mSpawnTimer == 0) {
            spawn = true;
            attach = false;
        }
    }

    if (spawn) {
        TVec3f up;
        MR::calcActorAxisY(&up, this);
        TVec3f pos = mPosition + up.multInLine(::sGenerateOffsetY);
        MR::startBck(this, "Generate", nullptr);
        BigBubble* bubble = MR::generateBigBubble(this, pos, up, MR::getRandom(mMinSize, mMaxSize), attach, mIsObstruct, mShape, mLimitterID);

        if (attach) {
            mAttachedBubble = bubble;
        }
    }

    tryNonActive();
}

NameObj* MR::createBigBubbleGenerator(const char* pName) {
    BigBubbleGenerator* generator = new BigBubbleGenerator(pName);
    generator->mIsObstruct = false;
    return generator;
}

NameObj* MR::createBigObstructBubbleGenerator(const char* pName) {
    BigBubbleGenerator* generator = new BigBubbleGenerator(pName);
    generator->mIsObstruct = true;
    return generator;
}
