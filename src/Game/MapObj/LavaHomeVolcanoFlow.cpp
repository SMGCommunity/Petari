#include "Game/MapObj/LavaHomeVolcanoFlow.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvLavaHomeVolcanoFlow {
    NEW_NERVE(HostTypeWait, LavaHomeVolcanoFlow, Wait);
    NEW_NERVE(HostTypeMove, LavaHomeVolcanoFlow, Move);
    NEW_NERVE(HostTypeDone, LavaHomeVolcanoFlow, Wait);
};  // namespace NrvLavaHomeVolcanoFlow

LavaHomeVolcanoFlow::LavaHomeVolcanoFlow(const char* pName) : LiveActor(pName) {
}

void LavaHomeVolcanoFlow::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("LavaHomeVolcanoFlow", nullptr, false);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    initNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeWait));
    MR::needStageSwitchReadA(this, rIter);
    MR::connectToSceneMapObj(this);
    makeActorAppeared();
    MR::startBtk(this, "LavaHomeVolcanoFlow");
}

void LavaHomeVolcanoFlow::startClipped() {
    LiveActor::startClipped();
    if (isNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeMove)) || isNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeDone))) {
        MR::deleteEffect(this, "Loop");
    }
}

void LavaHomeVolcanoFlow::endClipped() {
    LiveActor::endClipped();
    if (isNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeMove)) || isNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeDone))) {
        MR::emitEffect(this, "Loop");
    }
}

void LavaHomeVolcanoFlow::exeWait() {
    if (isNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeWait)) && MR::isOnSwitchA(this)) {
        setNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeMove));
        return;
    }
    if (isNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeDone))) {
        MR::startLevelSound(this, "SE_OJ_LV_VOLCANO_ERUPTION");
    }
}

void LavaHomeVolcanoFlow::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "LavaHomeVolcanoFlow");
    }
    MR::startLevelSound(this, "SE_OJ_LV_VOLCANO_EXPL_SIGN");
    if (MR::isStep(this, 60)) {
        MR::emitEffect(this, "Active");
        MR::emitEffect(this, "Loop");
        MR::startSound(this, "SE_OJ_VOLCANO_EXPLODE");
        MR::tryRumblePadVeryStrong(this, WPAD_CHAN0);
        setNerve(GET_NERVE(LavaHomeVolcanoFlow, HostTypeDone));
    }
}

LavaHomeVolcanoFlow::~LavaHomeVolcanoFlow() {
}
