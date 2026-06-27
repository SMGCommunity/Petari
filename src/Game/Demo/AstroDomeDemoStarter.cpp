#include "Game/Demo/AstroDomeDemoStarter.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/Util.hpp"

namespace {
    const char* const cJumpOutPartName = "飛び出す";
    const Vec cAppearRotate = {90.0f, -5.0f, 0.0f};
    // const s32 cJumpOutStopStep = _;
    // const s32 cJumpOutStopFrame = _;
    // const f32 cJumpOutStartSpeed = _;
    // const f32 cJumpOutRotateSpeed = _;
    // const f32 cJumpOutRotateEndAngle = _;
    const f32 cJumpOutNearClipDistance = 250.0f;
    const s32 cWhiteOutFrame = 146;
    const s32 cBgmStartFrame = 108;
};  // namespace

namespace NrvAstroDomeDemoStarter {
    NEW_NERVE(AstroDomeDemoStarterNrvSpinDriverAppear, AstroDomeDemoStarter, SpinDriverAppear);
    NEW_NERVE(AstroDomeDemoStarterNrvSpinDriverStart, AstroDomeDemoStarter, SpinDriverStart);
    NEW_NERVE(AstroDomeDemoStarterNrvSpinDriverShoot, AstroDomeDemoStarter, SpinDriverShoot);
    NEW_NERVE(AstroDomeDemoStarterNrvJumpOut, AstroDomeDemoStarter, JumpOut);
    NEW_NERVE(AstroDomeDemoStarterNrvWhiteOut, AstroDomeDemoStarter, WhiteOut);
};  // namespace NrvAstroDomeDemoStarter

AstroDomeDemoStarter::AstroDomeDemoStarter(const char* pName) : LiveActor(pName), _8C(), _90(), _F4() {
    _94.identity();
    _C4.identity();
}

void AstroDomeDemoStarter::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("SuperSpinDriver", nullptr, false);
    MR::hideModel(this);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    initEffectKeeper(0, nullptr, false);
    initSound(6, false);

    MR::invalidateClipping(this);

    MR::initActorCamera(this, rIter, &_F4);
    MR::initAnimCamera(this, _F4, "AstroDomeDemoCamera.arc", "AstroDomeDemoCamera");

    _8C = new SpinDriverShootPath();
    _8C->init(rIter);

    _90 = new SpinDriverPathDrawer(_8C);
    _90->initWithoutIter();
    _90->setColorNormal();

    MR::startBtpAndSetFrameAndStop(this, "SuperSpinDriver", 0.0f);
    MR::startBrk(this, "Yellow");

    initNerve(&NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvSpinDriverAppear::sInstance);

    MR::tryRegisterDemoCast(this, rIter);
    MR::registerDemoActionNerve(this, &NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvSpinDriverStart::sInstance, "スピンドライバ起動");
    MR::registerDemoActionNerve(this, &NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvJumpOut::sInstance, "飛び出す");
    MR::registerDemoActionFunctor(this, MR::Functor_Inline(this, &AstroDomeDemoStarter::startJumpOut), nullptr);
    MR::needStageSwitchWriteA(this, rIter);

    makeActorDead();
}

void AstroDomeDemoStarter::startJumpOut() {
    MR::onSwitchA(this);
    mPosition.set(0.0f);
    _94.identity();

    MR::startAnimCameraTargetSelf(this, _F4, "AstroDomeDemoCamera", 0, MR::getAnimCameraFrame(this, _F4, "AstroDomeDemoCamera") / 180.0f);
    MR::overlayWithPreviousScreen(2);
}

void AstroDomeDemoStarter::calcAndSetBaseMtx() {
}

void AstroDomeDemoStarter::movePlayer() {
    if (!MR::isDemoPartActive(::cJumpOutPartName)) {
        return;
    }

    f32 demoPartStepRate = MR::calcDemoPartStepRate(::cJumpOutPartName);

    TVec3f direction;

    _8C->calcDirection(&direction, demoPartStepRate, 0.01f);

    // something weird is going on with these floats here which i can't seem to fix
    f32 flt = (int)(-18.0f / TWO_PI) * TWO_PI * MR::getEaseInOutValue(demoPartStepRate, 0.0f, 1.0f, 1.0f);

    TPos3f mtx;
    mtx.identity();
    mtx.setRotateInline(flt, 1.0f);

    TVec3f position;

    _8C->calcPosition(&position, demoPartStepRate);

    TPos3f mtx2;
    MR::makeMtxUpFrontPos(&mtx2, direction, TVec3f(0.0f, -1.0f, 0.0f), position);

    mtx2.concat(mtx);

    MR::setPlayerBaseMtx(mtx2);

    if (MR::isJudgedToNearClip(position, ::cJumpOutNearClipDistance)) {
        MR::hidePlayer();
    } else {
        MR::showPlayer();
    }

    _90->setCoord(demoPartStepRate);
    MR::startLevelSoundPlayer("SE_PM_LV_SPIN_DRV_FLY", -1);
}

void AstroDomeDemoStarter::exeSpinDriverAppear() {
    if (MR::isFirstStep(this)) {
        MtxPtr playerDemobaseMtx = MR::getPlayerDemoActor()->getBaseMtx();
        _C4.set(playerDemobaseMtx);
        TPos3f rotateMtx;
        MR::makeMtxRotate(rotateMtx, TVec3f(cAppearRotate));
        _C4.getTransInline(mPosition);
        MR::makeMtxUpFrontPos(&_94, MR::getCamYdir(), MR::getCamZdir(), mPosition);
        _94.concat(rotateMtx);
        MR::setBaseTRMtx(this, _94);
        MR::showModel(this);
        MR::startBck(this, "Appear", nullptr);
        MR::startBckPlayer("SpinDriverWait", 20);
        MR::startSound(this, "SE_OJ_S_SPIN_DRV_APPEAR");
    }

    MR::startLevelSound(this, "SE_OJ_LV_S_SPIN_DRV_SHINE");
    MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_CAPTURE");

    TPos3f slerpMtx;
    MR::blendMtxRotateSlerp(_C4, _94, MR::calcDemoPartStepRate("スピンドライバ出現"), slerpMtx);
    slerpMtx.setTrans(mPosition);
    MR::setPlayerBaseMtx(slerpMtx);
}

void AstroDomeDemoStarter::exeSpinDriverStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Start", nullptr);
        MR::startBckPlayer("SuperSpinDriverStart", static_cast< const char* >(nullptr));
        MR::setPlayerBaseMtx(_94);
        MR::tryRumblePadMiddle(this, 0);
        MR::startCSSound("CS_SPIN_DRIVE_LONG", "SE_SY_CS_S_SPIN_DRV_START", 0);
        MR::startSoundPlayer("SE_PM_SPIN_ATTACK", -1);
    }

    if (MR::isLessStep(this, 43)) {
        MR::tryRumblePadMiddle(this, 0);
    } else {
        MR::setBckRate(this, 0.0f);
        MR::setBckRate(MR::getPlayerDemoActor(), 0.0f);
    }

    MR::setNerveAtStep(this, &NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvSpinDriverShoot::sInstance, 58);
}

void AstroDomeDemoStarter::exeSpinDriverShoot() {
    if (MR::isFirstStep(this)) {
        MR::setBckRate(this, 1.0f);
        MR::startBckPlayer("SpaceFlyStart", static_cast< const char* >(nullptr));
        MR::emitEffect(MR::getPlayerDemoActor(), "SuperSpinDriverFlyGlow");
        MR::shakeCameraNormal();
        MR::tryRumblePadVeryStrong(this, 0);

        if (MR::isKoopaFortressAppearInGalaxy()) {
            MR::startSystemSE("SE_SY_ASTRO_DEPERTURE_ME_KP");
        } else {
            MR::startSystemSE("SE_SY_ASTRO_DEPERTURE_ME");
        }

        MR::startSound(this, "SE_OJ_S_SPIN_DRV_JUMP");
        MR::startSoundPlayer("SE_PV_JUMP_JOY", -1);
    }

    TPos3f playerBaseMtx = _94;
    TVec3f mult;
    _94.mult(TVec3f(0.0f, getNerveStep() * 50.0f, 0.0f), mult);

    playerBaseMtx.setTrans(mult);
    MR::setPlayerBaseMtx(playerBaseMtx);
}

void AstroDomeDemoStarter::exeJumpOut() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::resetPlayerEffect();
        MR::startBckPlayer("SpaceFlyLoop", static_cast< const char* >(nullptr));
        MR::emitEffect(MR::getPlayerDemoActor(), "SuperSpinDriverFlyGlow");
        _90->appear();
    }

    movePlayer();

    if (MR::isStep(this, ::cBgmStartFrame)) {
        MR::startStageBGM("BGM_SENARIO_SEL", false);
    }

    if (MR::isStep(this, ::cWhiteOutFrame)) {
        setNerve(&NrvAstroDomeDemoStarter::AstroDomeDemoStarterNrvWhiteOut::sInstance);
    }
}

void AstroDomeDemoStarter::exeWhiteOut() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_DM_SENARIO_SEL_START");
        MR::submitTrigSE();
        MR::closeSystemWipeWhiteFade();
    }

    movePlayer();
}
