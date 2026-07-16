#include "Game/Map/LavaShellTower.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvLavaShellTower {
    NEW_NERVE(HostTypeWait, LavaShellTower, Wait);
    NEW_NERVE(HostTypeDemo, LavaShellTower, Demo);
    NEW_NERVE(HostTypeDone, LavaShellTower, Done);
};  // namespace NrvLavaShellTower

LavaShellTower::LavaShellTower(const char* pName) : MapObjActor(pName) {
}

void LavaShellTower::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupSound(4);
    info.setupGroupClipping(0x40);
    info.setupNerve(&NrvLavaShellTower::HostTypeWait::sInstance);
    initialize(rIter, info);
    MR::registerDemoActionNerve(this, &NrvLavaShellTower::HostTypeDemo::sInstance, "開始");
    makeActorAppeared();
}

void LavaShellTower::exeWait() {
}

void LavaShellTower::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::shakeCameraInfinity(this, 0.1f, 2.2f);
        MR::overlayWithPreviousScreen(2);
    }

    MR::startSystemLevelSE("SE_DM_LV_SHELL_TOWER");
    MR::tryRumblePadStrong(this, WPAD_CHAN0);

    if (MR::isGreaterStep(this, 1) && MR::isDemoLastStep()) {
        MR::stopShakingCamera(this);
        setNerve(&NrvLavaShellTower::HostTypeDone::sInstance);
    }
}

void LavaShellTower::exeDone() {
}
