#include "Game/NPC/TicoReading.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvTicoReading {
    NEW_NERVE(TicoReadingNrvWait, TicoReading, Wait);
};

TicoReading::TicoReading(const char *pName) : LiveActor(pName) {

}

void TicoReading::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("Tico", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    s32 color = 0;
    MR::getJMapInfoArg0NoInit(rIter, &color);
    MR::startBrk(this, "ColorChange");
    MR::setBrkFrameAndStop(this, color);
    initEffectKeeper(0, nullptr, false);
    MR::initShadowFromCSV(this, "Shadow");
    MR::tryRegisterDemoCast(this, rIter);
    initNerve(&NrvTicoReading::TicoReadingNrvWait::sInstance);
    makeActorDead();
}

void TicoReading::appear() {
    LiveActor::appear();
    setNerve(&NrvTicoReading::TicoReadingNrvWait::sInstance);
}

void TicoReading::kill() {
    MR::forceDeleteEffectAll(this);
    LiveActor::kill();
}

TicoReading::~TicoReading() {

}

inline void TicoReading::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DemoRosettaReadingWait", nullptr);
        MR::setBckFrameAtRandom(this);
    }
}
