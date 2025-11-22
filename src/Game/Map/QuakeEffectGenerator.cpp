#include "Game/Map/QuakeEffectGenerator.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvQuakeEffectGenerator {
    NEW_NERVE(HostTypeWait, QuakeEffectGenerator, Wait);
    NEW_NERVE(HostTypeWaitInArea, QuakeEffectGenerator, WaitInArea);
    NEW_NERVE(HostTypeQuaking, QuakeEffectGenerator, Quaking);
}; // namespace NrvQuakeEffectGenerator

QuakeEffectGenerator::QuakeEffectGenerator()
    : LiveActor("地震効果生成") {
}

void QuakeEffectGenerator::init(const JMapInfoIter& rIter) {
    initNerve(&NrvQuakeEffectGenerator::HostTypeWait::sInstance);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void QuakeEffectGenerator::exeWaitInArea() {
    if (!MR::isPlayerInAreaObj("QuakeEffectArea")) {
        setNerve(&NrvQuakeEffectGenerator::HostTypeWait::sInstance);
    } else {
        if (!MR::isFirstStep(this)) {
            if (getNerveStep() % 400 == 0) {
                setNerve(&NrvQuakeEffectGenerator::HostTypeQuaking::sInstance);
            }
        }
    }
}

void QuakeEffectGenerator::exeQuaking() {
    if (MR::isFirstStep(this)) {
        MR::shakeCameraInfinity(this, 0.05f, 1.5f);
    }

    MR::startAtmosphereLevelSE("SE_AT_LV_EARTHQUAKE", -1, -1);

    if (MR::isStep(this, 120)) {
        MR::stopShakingCamera(this);
        if (MR::isPlayerInAreaObj("QuakeEffectArea")) {
            setNerve(&NrvQuakeEffectGenerator::HostTypeWaitInArea::sInstance);
        } else {
            setNerve(&NrvQuakeEffectGenerator::HostTypeWait::sInstance);
        }
    }
}

void QuakeEffectGenerator::exeWait() {
    if (MR::isPlayerInAreaObj("QuakeEffectArea")) {
        setNerve(&NrvQuakeEffectGenerator::HostTypeWaitInArea::sInstance);
    }
}

QuakeEffectGenerator::~QuakeEffectGenerator() {
}
