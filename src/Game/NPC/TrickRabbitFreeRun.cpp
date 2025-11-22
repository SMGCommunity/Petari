#include "Game/NPC/TrickRabbitFreeRun.hpp"

TrickRabbitFreeRun::TrickRabbitFreeRun(const char* pName)
    : LiveActor(pName),
      _8C(0.0f, 0.0f, 0.0f, 1.0f),
      _9C(0.0f, 0.0f, 1.0f),
      _A8(0),
      _AC(0),
      _B0(0),
      _B4(0),
      mTalkCtrl(0),
      mFootPrint(0),
      mSpotMarkLight(0) {
}

void TrickRabbitFreeRun::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TrickRabbit", 0, 0);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    MR::makeQuatAndFrontFromRotate(&_8C, &_9C, this);
    mSpotMarkLight = new SpotMarkLight(this, 100.0f, 1500.0f, 0);
    mSpotMarkLight->initWithoutIter();
    MR::initShadowFromCSV(this, "Shadow");
    mFootPrint = TrickRabbitUtil::createRabbitFootPrint(this);
    initEffectKeeper(0, 0, 0);
    MR::onEmitEffectSyncClipping(this, "Light");
    MR::onForceDeleteEffectSyncClipping(this, "Light");
    TrickRabbitFreeRun::initSensor();
    initBinder(60.0f, 60.0f, 0);
    MR::createTalkCtrl(this, rIter, "TrickRabbit", TVec3f(0.0f, 120.0f, 0.0f), 0);
    TrickRabbitFreeRun::initState();
    initSound(4, false);
    MR::onCalcGravity(this);
    MR::declarePowerStar(this);
    initNerve(&NrvTrickRabbitFreeRun::TrickRabbitFreeRunNrvWaitStart::sInstance);
    makeActorAppeared();
}
