#include "Game/MapObj/AirBubbleGenerator.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/AirBubbleHolder.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sGenerateTime = 6;
    static const f32 sGenerateOffsetY = 120.0f;
};  // namespace

namespace NrvAirBubbleGenerator {
    NEW_NERVE(AirBubbleGeneratorNrvWait, AirBubbleGenerator, Wait);
    NEW_NERVE(AirBubbleGeneratorNrvGenerate, AirBubbleGenerator, Generate);
};  // namespace NrvAirBubbleGenerator

AirBubbleGenerator::~AirBubbleGenerator() {
}

AirBubbleGenerator::AirBubbleGenerator(const char* pName) : LiveActor(pName), _8C(), mWaitTime(180), mBubbleLifeTime(-1) {
}

void AirBubbleGenerator::init(const JMapInfoIter& rIter) {
    MR::createAirBubbleHolder();
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("AirBubbleGenerator", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObj(this);
    initEffectKeeper(0, nullptr, false);
    initNerve(&NrvAirBubbleGenerator::AirBubbleGeneratorNrvWait::sInstance);
    initHitSensor(1);
    MR::addBodyMessageSensorReceiver(this);
    MR::initCollisionParts(this, "AirBubbleGenerator", getSensor(nullptr), nullptr);
    MR::getJMapInfoArg0NoInit(rIter, &mWaitTime);
    MR::getJMapInfoArg1NoInit(rIter, &mBubbleLifeTime);
    initSound(2, false);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void AirBubbleGenerator::control() {
}

void AirBubbleGenerator::exeWait() {
    if (mWaitTime - 1 == getNerveStep() % mWaitTime) {
        setNerve(&NrvAirBubbleGenerator::AirBubbleGeneratorNrvGenerate::sInstance);
    }
}

void AirBubbleGenerator::exeGenerate() {
    TVec3f axisY;

    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_AIR_BUBBLE_APPEAR");
        MR::startBck(this, "Generate", nullptr);
    }

    if (MR::isGreaterStep(this, ::sGenerateTime)) {
        MR::calcActorAxisY(&axisY, this);
        MR::appearAirBubble(mPosition + axisY * ::sGenerateOffsetY, mBubbleLifeTime);
        setNerve(&NrvAirBubbleGenerator::AirBubbleGeneratorNrvWait::sInstance);
    }
}
