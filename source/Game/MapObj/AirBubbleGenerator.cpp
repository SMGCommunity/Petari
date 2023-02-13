#include "Game/MapObj/AirBubbleGenerator.h"
#include "Game/MapObj/AirBubbleHolder.h"

AirBubbleGenerator::~AirBubbleGenerator() {

}

AirBubbleGenerator::AirBubbleGenerator(const char *pName) : LiveActor(pName) {
    _8C = 0;
    mSpawnDelay = 180;
    mBubbleDuration = -1;
}

void AirBubbleGenerator::init(const JMapInfoIter &rIter) {
    MR::createAirBubbleHolder();
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("AirBubbleGenerator", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObj(this);
    initEffectKeeper(0, nullptr, false);
    initNerve(&NrvAirBubbleGenerator::AirBubbleGenerateNrvWait::sInstance);
    initHitSensor(1);
    MR::addBodyMessageSensorReceiver(this);
    MR::initCollisionParts(this, "AirBubbleGenerator", getSensor(nullptr), nullptr);
    MR::getJMapInfoArg0NoInit(rIter, &mSpawnDelay);
    MR::getJMapInfoArg1NoInit(rIter, &mBubbleDuration);
    initSound(2, false);
    
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void AirBubbleGenerator::control() {

}

void AirBubbleGenerator::exeWait() {
    s32 spawnDelay = mSpawnDelay;
    if (spawnDelay - 1 == getNerveStep() % spawnDelay) {
        setNerve(&NrvAirBubbleGenerator::AirBubbleGeneratorNrvGenerate::sInstance);
    }
}

#ifdef NON_MATCHING
// inlined assembly math is not matching, deal with this later
void AirBubbleGenerator::exeGenerate() {
    TVec3f axisY;

    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_AIR_BUBBLE_APPEAR", -1, -1);
        MR::startBck(this, "Generate", nullptr);
    }

    if (MR::isGreaterStep(this, 6)) {
        MR::calcActorAxisY(&axisY, this);
        MR::appearAirBubble(MR::createVecAndScaleByAndAdd_2(axisY, mPosition), mBubbleDuration);
        setNerve(&NrvAirBubbleGenerator::AirBubbleGenerateNrvWait::sInstance);
    }
}
#endif

namespace NrvAirBubbleGenerator {
    INIT_NERVE(AirBubbleGenerateNrvWait);
    INIT_NERVE(AirBubbleGeneratorNrvGenerate);
};
