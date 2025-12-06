#include "Game/MapObj/LavaHomeVolcanoFlow.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

namespace NrvLavaHomeVolcanoFlow {
    NEW_NERVE(HostTypeWait, LavaHomeVolcanoFlow, Wait);
    NEW_NERVE(HostTypeMove, LavaHomeVolcanoFlow, Move);
    NEW_NERVE(HostTypeDone, LavaHomeVolcanoFlow, Wait);
};  // namespace NrvLavaHomeVolcanoFlow

LavaHomeVolcanoFlow::LavaHomeVolcanoFlow(const char* pName) : LiveActor(pName) {}

void LavaHomeVolcanoFlow::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("LavaHomeVolcanoFlow", nullptr, false);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    initNerve(&NrvLavaHomeVolcanoFlow::HostTypeWait::sInstance);
    MR::needStageSwitchReadA(this, rIter);
    MR::connectToSceneMapObj(this);
    makeActorAppeared();
    MR::startBtk(this, "LavaHomeVolcanoFlow");
}

void LavaHomeVolcanoFlow::startClipped() {
    LiveActor::startClipped();
    if (isNerve(&NrvLavaHomeVolcanoFlow::HostTypeMove::sInstance) || isNerve(&NrvLavaHomeVolcanoFlow::HostTypeDone::sInstance)) {
        MR::deleteEffect(this, "Loop");
    }
}

void LavaHomeVolcanoFlow::endClipped() {
    LiveActor::endClipped();
    if (isNerve(&NrvLavaHomeVolcanoFlow::HostTypeMove::sInstance) || isNerve(&NrvLavaHomeVolcanoFlow::HostTypeDone::sInstance)) {
        MR::emitEffect(this, "Loop");
    }
}

void LavaHomeVolcanoFlow::exeWait() {
    if (isNerve(&NrvLavaHomeVolcanoFlow::HostTypeWait::sInstance) && MR::isOnSwitchA(this)) {
        setNerve(&NrvLavaHomeVolcanoFlow::HostTypeMove::sInstance);
        return;
    }
    if (isNerve(&NrvLavaHomeVolcanoFlow::HostTypeDone::sInstance)) {
        MR::startLevelSound(this, "SE_OJ_LV_VOLCANO_ERUPTION", -1, -1, -1);
    }
}

void LavaHomeVolcanoFlow::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "LavaHomeVolcanoFlow", nullptr);
    }
    MR::startLevelSound(this, "SE_OJ_LV_VOLCANO_EXPL_SIGN", -1, -1, -1);
    if (MR::isStep(this, 60)) {
        MR::emitEffect(this, "Active");
        MR::emitEffect(this, "Loop");
        MR::startSound(this, "SE_OJ_VOLCANO_EXPLODE", -1, -1);
        MR::tryRumblePadVeryStrong(this, 0);
        setNerve(&NrvLavaHomeVolcanoFlow::HostTypeDone::sInstance);
    }
}

LavaHomeVolcanoFlow::~LavaHomeVolcanoFlow() {}
