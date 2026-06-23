#include "Game/GameAudio/AudCameraWatcher.hpp"
#include "Game/AudioLib/AudEffector.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

AudCameraWatcher::AudCameraWatcher() : NameObj("AudCameraWatcher"), _C(0) {
}

void AudCameraWatcher::playAtmosphereSE() {
    if (MR::isCameraInWater()) {
        MR::startAtmosphereLevelSE("SE_AT_LV_UNDER_WATER");
        AudWrap::getSystem()->mAudEffector->setCutoff(3);
        MR::setStageBGMStateBit(2);
        return;
    }
    AudWrap::getSystem()->mAudEffector->setCutoff(127);
}

void AudCameraWatcher::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_AudCameraWatcher, -1, -1, -1);
}

void AudCameraWatcher::movement() {
    MR::setMicMtx(MR::getCameraViewMtx(), 0);

    // They could've just done something like setFarCamera(MR::getFovy() < 20.0f), but oh well.
    if (MR::getFovy() < 20.0f) {
        AudWrap::getSystem()->setFarCamera(true);
    } else {
        AudWrap::getSystem()->setFarCamera(false);
    }

    playAtmosphereSE();
}
