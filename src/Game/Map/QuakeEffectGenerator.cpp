#include "Game/Map/QuakeEffectGenerator.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sQuakeInterval = 400;
    static const f32 sCamShakeIntensity = 0.05f;
    static const f32 sCamShakeSpeed = 1.5f;
    static const s32 sCamShakeFrame = 120;
};  // namespace

namespace NrvQuakeEffectGenerator {
    NEW_NERVE(HostTypeWait, QuakeEffectGenerator, Wait);
    NEW_NERVE(HostTypeWaitInArea, QuakeEffectGenerator, WaitInArea);
    NEW_NERVE(HostTypeQuaking, QuakeEffectGenerator, Quaking);
};  // namespace NrvQuakeEffectGenerator

QuakeEffectGenerator::QuakeEffectGenerator() : LiveActor("地震効果生成") {
}

void QuakeEffectGenerator::init(const JMapInfoIter& rIter) {
    initNerve(&NrvQuakeEffectGenerator::HostTypeWait::sInstance);
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void QuakeEffectGenerator::exeWait() {
    if (MR::isPlayerInAreaObj("QuakeEffectArea")) {
        setNerve(&NrvQuakeEffectGenerator::HostTypeWaitInArea::sInstance);
    }
}

void QuakeEffectGenerator::exeWaitInArea() {
    if (!MR::isPlayerInAreaObj("QuakeEffectArea")) {
        setNerve(&NrvQuakeEffectGenerator::HostTypeWait::sInstance);
    } else if (!MR::isFirstStep(this)) {
        if (getNerveStep() % ::sQuakeInterval == 0) {
            setNerve(&NrvQuakeEffectGenerator::HostTypeQuaking::sInstance);
        }
    }
}

void QuakeEffectGenerator::exeQuaking() {
    if (MR::isFirstStep(this)) {
        MR::shakeCameraInfinity(this, ::sCamShakeIntensity, ::sCamShakeSpeed);
    }

    MR::startAtmosphereLevelSE("SE_AT_LV_EARTHQUAKE");

    if (MR::isStep(this, ::sCamShakeFrame)) {
        MR::stopShakingCamera(this);

        if (MR::isPlayerInAreaObj("QuakeEffectArea")) {
            setNerve(&NrvQuakeEffectGenerator::HostTypeWaitInArea::sInstance);
        } else {
            setNerve(&NrvQuakeEffectGenerator::HostTypeWait::sInstance);
        }
    }
}
