#include "Game/Demo/AstroDomeDemoStarter.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

namespace NrvAstroDomeDemoStarter {
    NEW_NERVE(AstroDomeDemoStarterNrvSpinDriverAppear, AstroDomeDemoStarter, SpinDriverAppear);
    NEW_NERVE(AstroDomeDemoStarterNrvSpinDriverStart, AstroDomeDemoStarter, SpinDriverStart);
    NEW_NERVE(AstroDomeDemoStarterNrvSpinDriverShoot, AstroDomeDemoStarter, SpinDriverShoot);
    NEW_NERVE(AstroDomeDemoStarterNrvJumpOut, AstroDomeDemoStarter, JumpOut);
    NEW_NERVE(AstroDomeDemoStarterNrvWhiteOut, AstroDomeDemoStarter, WhiteOut);
}

AstroDomeDemoStarter::AstroDomeDemoStarter(const char* pName) : LiveActor(pName) {
    mSpinDriverShootPath = nullptr;
    mSpinDriverPathDrawer = nullptr;
    mCameraInfo = nullptr;
    _94.identity();
    _C4.identity();
}
AstroDomeDemoStarter::~AstroDomeDemoStarter() {}

void AstroDomeDemoStarter::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("SuperSpinDriver", nullptr, false);
    MR::hideModel(this);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    initEffectKeeper(0, nullptr, false);
    initSound(6, false);
    MR::invalidateClipping(this);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    MR::initAnimCamera(this, mCameraInfo, "AstroDomeDemoCamera.arc", "AstroDomeDemoCamera");
    mSpinDriverShootPath = new SpinDriverShootPath;
    mSpinDriverShootPath->init(rIter);
    mSpinDriverPathDrawer = new SpinDriverPathDrawer(mSpinDriverShootPath);
    mSpinDriverPathDrawer->initWithoutIter();
    mSpinDriverPathDrawer->setColorNormal();
    MR::startBtpAndSetFrameAndStop(this, "SuperSpinDriver", 0.0f);
    MR::startBrk(this, "Yellow");
    initNerve(&NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvSpinDriverAppear::sInstance);
    MR::tryRegisterDemoCast(this, rIter);
    MR::registerDemoActionNerve(this, &NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvSpinDriverStart::sInstance, "スピンドライバ起動");
    MR::registerDemoActionNerve(this, &NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvJumpOut::sInstance, "飛び出す");
    MR::registerDemoActionFunctor(this, MR::Functor(this, &AstroDomeDemoStarter::startJumpOut), nullptr);
    MR::needStageSwitchWriteA(this, rIter);
    makeActorDead();
}

void AstroDomeDemoStarter::startJumpOut() {
    MR::onSwitchA(this);
    mPosition.z = 0;
    mPosition.y = 0;    // a TVec3f.set() here but inverted
    mPosition.x = 0;
    _94.identity();
    MR::startAnimCameraTargetSelf(this, mCameraInfo, "AstroDomeDemoCamera", 0, MR::getAnimCameraFrame(this, mCameraInfo, "AstroDomeDemoCamera") / 180.0f);
    MR::overlayWithPreviousScreen(2);
}

void AstroDomeDemoStarter::calcAndSetBaseMtx() {
    return;
}

void AstroDomeDemoStarter::exeSpinDriverStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Start", nullptr);
        MR::startBckPlayer("SuperSpinDriverStart", (const char*)nullptr);
        MR::setPlayerBaseMtx(_94);
        MR::tryRumblePadMiddle(this, 0);
        MR::startCSSound("CS_SPIN_DRIVE_LONG", "SE_SY_CS_S_SPIN_DRV_START", 0);
        MR::startSoundPlayer("SE_PM_SPIN_ATTACK", -1);
    }
    if (MR::isLessStep(this, 43)) {
        MR::tryRumblePadMiddle(this, 0);
    }
    else {
        MR::setBckRate(this, 0.0f);
        MR::setBckRate(MR::getPlayerDemoActor(), 0.0f);
    }
    MR::setNerveAtStep(this, &NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvSpinDriverShoot::sInstance, 58);
}

/* void AstroDomeDemoStarter::exeSpinDriverShoot() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 1.0f);
        MR::startBckPlayer("SpaceFlyStart", (const char*)nullptr);
        MR::emitEffect(MR::getPlayerDemoActor(), "SuperSpinDriverFlyGlow");
        MR::shakeCameraNormal();
        MR::tryRumblePadVeryStrong(this, 0);
        if (MR::isKoopaFortressAppearInGalaxy()) {
            MR::startSystemSE("SE_SY_ASTRO_DEPERTURE_ME_KP");
        }
        else {
            MR::startSystemSE("SE_SY_ASTRO_DEPERTURE_ME");
        }
        MR::startSound(this, "SE_OJ_S_SPIN_DRV_JUMP");
        MR::startSoundPlayer("SE_PV_JUMP_JOY", -1);
    }
    for (int i = 6; i > 0; i--) {
        
    }
} */

void AstroDomeDemoStarter::exeJumpOut() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::resetPlayerEffect();
        MR::startBckPlayer("SpaceFlyLoop", (const char*)nullptr);
        MR::emitEffect(MR::getPlayerDemoActor(), "SuperSpinDriverFlyGlow");
        mSpinDriverPathDrawer->appear();
    }
    movePlayer();
    if (MR::isStep(this, 108)) {
        MR::startStageBGM("BGM_SCENARIO_SEL", false);
    }
    if (MR::isStep(this, 146)) {
        setNerve(&NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvWhiteOut::sInstance);
    }
}

void AstroDomeDemoStarter::exeWhiteOut() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_DM_SENARIO_SEL_START");
        MR::submitTrigSE();
        MR::closeSystemWipeWhiteFade(-1);
    }
    movePlayer();
}
