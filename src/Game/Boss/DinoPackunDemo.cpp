#include "Game/Boss/DinoPackunDemo.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunDemoPosition.hpp"
#include "Game/Boss/DinoPackunEggShell.hpp"
#include "Game/Boss/DinoPackunSequencer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    // static const _ sMairoHitEgg_ = _;
    // static const _ sGroundRumbleStart = _;
    // static const _ sGroundRumbleEnd = _;
    // static const _ sSmashGroundTail = _;
    // static const _ sEggBrokenTime = _;
    // static const _ sEggBrokenStopSceneTime = _;
    // static const _ sEggBrokenTailStopTime = _;
    static const s32 sCryBlurStart = 100;
    static const s32 sCryBlurTime = 20;
    static const f32 sCryBlurOffset = 12.0f;
    static const u8 sCryBlurAlpha = 80;
    static const s32 sCryBlurFadeIn = 5;
    static const s32 sCryBlurFadeOut = 10;
    static const s32 sAngryLv1BlurStart = 99;
    static const s32 sAngryLv1BlurTime = 80;
    static const f32 sAngryLv1BlurOffset = 18.0f;
    static const u8 sAngryLv1BlurAlpha = 80;
    static const s32 sAngryLv1BlurFadeIn = 5;
    static const s32 sAngryLv1BlurFadeOut = 30;
    static const s32 sAngryLv2BlurStart = 205;
    static const s32 sAngryLv2BlurTime = 80;
    static const f32 sAngryLv2BlurOffset = 18.0f;
    static const u8 sAngryLv2BlurAlpha = 80;
    static const s32 sAngryLv2BlurFadeIn = 5;
    static const s32 sAngryLv2BlurFadeOut = 30;
    static const s32 sDeathBlurStart = 20;
    static const s32 sDeathBlurTime = 130;
    static const f32 sDeathBlurOffset = 18.0f;
    static const u8 sDeathBlurAlpha = 80;
    static const s32 sDeathBlurFadeIn = 15;
    static const s32 sDeathBlurFadeOut = 40;
    // static const _ sDownGroundRumbleTimeStart = _;
    // static const _ sDownGroundRumbleTimeEnd = _;
    static const s32 sAppearPowerStarTime = 60;
};  // namespace

namespace NrvDinoPackunDemo {
    NEW_NERVE(DinoPackunDemoNrvTryStartDemo, DinoPackunDemo, TryStartDemo);
    NEW_NERVE(DinoPackunDemoNrvOpeningDemo, DinoPackunDemo, OpeningDemo);
    NEW_NERVE(DinoPackunDemoNrvCryDemo, DinoPackunDemo, CryDemo);
    NEW_NERVE(DinoPackunDemoNrvAngryDemo, DinoPackunDemo, AngryDemo);
    NEW_NERVE(DinoPackunDemoNrvDownDemo, DinoPackunDemo, DownDemo);
    NEW_NERVE(DinoPackunDemoNrvAppearPowerStar, DinoPackunDemo, AppearPowerStar);
};  // namespace NrvDinoPackunDemo

DinoPackunDemo::DinoPackunDemo(DinoPackun* pBoss) : DinoPackunAction("ディノパックンデモ", pBoss), _14(), mDemoName() {
    MR::createCenterScreenBlur();
}

void DinoPackunDemo::init() {
    initNerve(&NrvDinoPackunDemo::DinoPackunDemoNrvOpeningDemo::sInstance);
}

void DinoPackunDemo::control() {
}

void DinoPackunDemo::startOpeningDemo() {
    startTryDemo("ディノパックン開始デモ", &NrvDinoPackunDemo::DinoPackunDemoNrvOpeningDemo::sInstance);
}

void DinoPackunDemo::startCryDemo() {
    startTryDemo("ディノパックン泣きデモ", &NrvDinoPackunDemo::DinoPackunDemoNrvCryDemo::sInstance);
}

void DinoPackunDemo::startAngryDemo() {
    startTryDemo("ディノパックン怒りデモ", &NrvDinoPackunDemo::DinoPackunDemoNrvAngryDemo::sInstance);
}

void DinoPackunDemo::startDownDemo() {
    startTryDemo("ディノパックンダウン", &NrvDinoPackunDemo::DinoPackunDemoNrvDownDemo::sInstance);
}

void DinoPackunDemo::startTryDemo(const char* pName, const Nerve* pNerve) {
    appear();

    if (MR::tryStartDemoMarioPuppetable(getHost(), pName)) {
        MR::overlayWithPreviousScreen(2);
        setNerve(pNerve);
    } else {
        _14 = pNerve;
        mDemoName = pName;
        setNerve(&NrvDinoPackunDemo::DinoPackunDemoNrvTryStartDemo::sInstance);
    }
}

void DinoPackunDemo::exeTryStartDemo() {
    if (MR::tryStartDemoMarioPuppetable(getHost(), mDemoName)) {
        setNerve(_14);
        MR::overlayWithPreviousScreen(2);
        mDemoName = nullptr;
        _14 = nullptr;
    }
}

void DinoPackunDemo::exeOpeningDemo() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(30);
        getHost()->startDemoAndReset();
        getHost()->mDemoPos->startOpeningDemo();
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "OpeningDemo", 0, 1.0f);
        MR::startAction(getHost(), "OpeningDemo");
        MR::startBva(getHost()->getEggShell(), "OpeningDemo");
        MR::invalidateCollisionParts(getHost()->getEggShell());
        MR::startBckPlayer("BattleWait", static_cast< const char* >(nullptr));
    }

    getHost()->mDemoPos->movement();

    if (MR::isStep(this, 28)) {
        MR::startSystemSE("SE_DM_MEET_BOSS");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_CREAK");
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER");
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_EGG_CREAK");
    }

    if (MR::isStep(this, 118)) {
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_EGG_DIG_OUT");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_GROAN");
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER");
    }

    if (MR::isStep(this, 245)) {
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_TAIL_ATTACK");
    }

    if (MR::isBckStopped(getHost())) {
        MR::startStageBGM("MBGM_GALAXY_INTER", false);
        getHost()->mDemoPos->endDemo();
        getHost()->resetPosition();
        getHost()->endDemo("ディノパックン開始デモ");
        MR::endAnimCamera(getHost(), getHost()->mCameraInfo, "OpeningDemo", -1, true);
        getHost()->activateParts();
        MR::validateCollisionParts(getHost()->getEggShell());
        kill();
    }
}

void DinoPackunDemo::exeCryDemo() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(getHost());
        MR::stopStageBGM(30);
        getHost()->startDemoAndReset();
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "CryDemo", 0, 1.0f);
        MR::startAction(getHost(), "CryDemo");
        MR::startBckPlayer("BattleWait", static_cast< const char* >(0));
    }

    if (MR::isStep(this, ::sCryBlurStart)) {
        MR::startCenterScreenBlur(::sCryBlurTime, ::sCryBlurOffset, ::sCryBlurAlpha, ::sCryBlurFadeIn, ::sCryBlurFadeOut);
    }

    if (MR::isBckStopped(getHost())) {
        MR::startBossBGM(MR::BossBgmID_DinoPackunA);
        getHost()->endDemo("ディノパックン泣きデモ");
        MR::endAnimCamera(getHost(), getHost()->mCameraInfo, "CryDemo", -1, -1);
        getHost()->activateParts();
        kill();
    }
}

void DinoPackunDemo::exeAngryDemo() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(30);
        getHost()->startDemoAndReset();
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "AngryDemo", 0, 1.0f);
        MR::startAction(getHost(), "AngryDemo");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_GROAN");
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER");
        MR::startBckPlayer("BattleWait", static_cast< const char* >(nullptr));
    }

    if (getHost()->mSequence->getVsCount() == 1) {
        MR::startLevelSound(getHost(), "SE_BM_LV_D_PAKKUN_ANGRY");
        if (MR::isStep(this, ::sAngryLv1BlurStart)) {
            MR::startCenterScreenBlur(::sAngryLv1BlurTime, ::sAngryLv1BlurOffset, ::sAngryLv1BlurAlpha, ::sAngryLv1BlurFadeIn,
                                      ::sAngryLv1BlurFadeOut);
        }
    }
    if (getHost()->mSequence->getVsCount() == 2) {
        if (MR::isLessStep(this, 205)) {
            MR::startLevelSound(getHost(), "SE_BM_LV_D_PAKKUN_ANGRY");
        }

        if (MR::isStep(this, ::sAngryLv2BlurStart)) {
            MR::startCenterScreenBlur(::sAngryLv2BlurTime, ::sAngryLv1BlurOffset, ::sAngryLv2BlurAlpha, ::sAngryLv1BlurFadeIn,
                                      ::sAngryLv1BlurFadeOut);
        }
    }

    if (MR::isBckStopped(getHost())) {
        MR::startBossBGM(MR::BossBgmID_DinoPackunB);
        getHost()->endDemo("ディノパックン怒りデモ");
        MR::endAnimCamera(getHost(), getHost()->mCameraInfo, "AngryDemo", -1, 1);
        getHost()->activateParts();
        kill();
    }
}

void DinoPackunDemo::exeDownDemo() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(30);
        getHost()->startDemoAndReset();
        MR::startAction(getHost(), "DownDemo");
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "DownDemo", 0, 1.0f);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_DIE");
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER");
        MR::startBckPlayer("BattleWait", static_cast< const char* >(nullptr));
    }

    if (MR::isStep(this, ::sDeathBlurStart)) {
        MR::startCenterScreenBlur(::sDeathBlurTime, ::sDeathBlurOffset, ::sDeathBlurAlpha, ::sDeathBlurFadeIn, ::sDeathBlurFadeOut);
    }

    if (MR::isStep(this, 319)) {
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_FALL_DOWN");
    }

    if (MR::isBckStopped(getHost())) {
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_EXPLODE");
        MR::tryRumblePadVeryStrong(getHost(), WPAD_CHAN0);
        MR::hideModel(getHost());
        setNerve(&NrvDinoPackunDemo::DinoPackunDemoNrvAppearPowerStar::sInstance);
    }
}

void DinoPackunDemo::exeAppearPowerStar() {
    if (MR::isStep(this, ::sAppearPowerStarTime)) {
        MR::startAfterBossBGM();
        MR::appearPowerStarContinueCurrentDemo(getHost(), getHost()->mPosition);
    }

    if (MR::isGreaterStep(this, ::sAppearPowerStarTime)) {
        if (MR::isEndPowerStarAppearDemo(getHost())) {
            getHost()->endDemo("ディノパックンダウン");
            MR::endAnimCamera(getHost(), getHost()->mCameraInfo, "DownDemo", -1, true);
            getHost()->kill();
            kill();
        }
    }
}

DinoPackunDemo::~DinoPackunDemo() {
}
