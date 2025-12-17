#include "Game/NPC/TrickRabbitFreeRun.hpp"
#include "Game/LiveActor/SpotMarkLight.hpp"
#include "Game/NPC/TrickRabbit.hpp"

namespace NrvTrickRabbitFreeRun {
    NEW_NERVE(TrickRabbitFreeRunNrvWaitStart, TrickRabbitFreeRun, WaitStart);
    NEW_NERVE(TrickRabbitFreeRunNrvTryDemo, TrickRabbitFreeRun, TryDemo);
    NEW_NERVE(TrickRabbitFreeRunNrvRunawayStart, TrickRabbitFreeRun, RunawayStart);
    NEW_NERVE(TrickRabbitFreeRunNrvRunaway, TrickRabbitFreeRun, Runaway);
    NEW_NERVE(TrickRabbitFreeRunNrvBlowDamage, TrickRabbitFreeRun, BlowDamage);
    NEW_NERVE(TrickRabbitFreeRunNrvCaught, TrickRabbitFreeRun, Caught);
    NEW_NERVE(TrickRabbitFreeRunNrvPowerStarDemo, TrickRabbitFreeRun, PowerStarDemo);
    NEW_NERVE(TrickRabbitFreeRunNrvGiveUp, TrickRabbitFreeRun, GiveUp);
};  // namespace NrvTrickRabbitFreeRun

TrickRabbitFreeRun::TrickRabbitFreeRun(const char* pName)
    : LiveActor(pName), _8C(0.0f, 0.0f, 0.0f, 1.0f), _9C(0.0f, 0.0f, 1.0f), _A8(0), _AC(0), _B0(0), _B4(0), mTalkMessageCtrl(nullptr),
      mFootPrint(nullptr), mSpotMarkLight(nullptr) {}

void TrickRabbitFreeRun::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TrickRabbit", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&_8C, &_9C, this);

    mSpotMarkLight = new SpotMarkLight(this, 100.0f, 1500.0f, nullptr);
    mSpotMarkLight->initWithoutIter();

    MR::initShadowFromCSV(this, "Shadow");

    mFootPrint = TrickRabbitUtil::createRabbitFootPrint(this);

    initEffectKeeper(0, nullptr, 0);
    MR::onEmitEffectSyncClipping(this, "Light");
    MR::onForceDeleteEffectSyncClipping(this, "Light");
    initSensor();
    initBinder(60.0f, 60.0f, 0);

    mTalkMessageCtrl = MR::createTalkCtrl(this, rIter, "TrickRabbit", TVec3f(0.0f, 120.0f, 0.0f), nullptr);

    initState();
    initSound(4, false);
    MR::onCalcGravity(this);
    MR::declarePowerStar(this);
    initNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvWaitStart::sInstance);
    makeActorAppeared();
}
