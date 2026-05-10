#include "Game/Boss/DinoPackunDemo.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunDemoPosition.hpp"
#include "Game/Boss/DinoPackunEggShell.hpp"
#include "Game/Boss/DinoPackunSequencer.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"
#include "Game/Util/CameraUtil.hpp"

namespace NrvDinoPackunDemo {
    NEW_NERVE(DinoPackunDemoNrvTryStartDemo, DinoPackunDemo, TryStartDemo);
    NEW_NERVE(DinoPackunDemoNrvOpeningDemo, DinoPackunDemo, OpeningDemo);
    NEW_NERVE(DinoPackunDemoNrvCryDemo, DinoPackunDemo, CryDemo);
    NEW_NERVE(DinoPackunDemoNrvAngryDemo, DinoPackunDemo, AngryDemo);
    NEW_NERVE(DinoPackunDemoNrvDownDemo, DinoPackunDemo, DownDemo);
    NEW_NERVE(DinoPackunDemoNrvAppearPowerStar, DinoPackunDemo, AppearPowerStar);
};  // namespace NrvDinoPackunDemo

DinoPackunDemo::DinoPackunDemo(DinoPackun* pBoss) : DinoPackunAction("ディノパックンデモ", pBoss) {
    _14 = nullptr;
    mDemoName = nullptr;
    MR::createCenterScreenBlur();
}

void DinoPackunDemo::init() {
    initNerve(&NrvDinoPackunDemo::DinoPackunDemoNrvOpeningDemo::sInstance);
}

void DinoPackunDemo::control() {
    return;
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
        MR::startBckPlayer("BattleWait", static_cast< const char* >(0));
    }

    getHost()->mDemoPos->movement();

    if (MR::isStep(this, 28)) {
        MR::startSystemSE("SE_DM_MEET_BOSS", -1, -1);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_CREAK", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_EGG_CREAK", -1, -1);
    }

    if (MR::isStep(this, 118)) {
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_EGG_DIG_OUT", -1, -1);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_GROAN", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    if (MR::isStep(this, 245)) {
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_TAIL_ATTACK", -1, -1);
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

    if (MR::isStep(this, 100)) {
        MR::startCenterScreenBlur(20, 12.0f, 0x50, 5, 10);
    }

    if (MR::isBckStopped(getHost())) {
        MR::startBossBGM(4);
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
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "AngryDemo", 0, 1.0);
        MR::startAction(getHost(), "AngryDemo");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_EGG_GROAN", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
        MR::startBckPlayer("BattleWait", static_cast< const char* >(0));
    }
    if ((getHost()->mSequence->getVsCount()) == 1) {
        MR::startLevelSound(getHost(), "SE_BM_LV_D_PAKKUN_ANGRY", -1, -1, -1);
        if (MR::isStep(this, 99)) {
            MR::startCenterScreenBlur(80, 18.0, 0x50u, 5, 30);
        }
    }
    if ((getHost()->mSequence->getVsCount()) == 2) {
        if (MR::isLessStep(this, 205)) {
            MR::startLevelSound(getHost(), "SE_BM_LV_D_PAKKUN_ANGRY", -1, -1, -1);
        }
        if (MR::isStep(this, 220)) {
            MR::startCenterScreenBlur(80, 18.0, 0x50u, 5, 30);
        }
    }
    if (MR::isBckStopped(getHost())) {
        MR::startBossBGM(5);
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
        MR::startAnimCameraTargetSelf(getHost(), getHost()->mCameraInfo, "DownDemo", 0, 1.0);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_DIE", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
        MR::startBckPlayer("BattleWait", static_cast< const char* >(0));
    }
    if (MR::isStep(this, 20)) {
        MR::startCenterScreenBlur(130, 18.0, 0x50u, 15, 40);
    }
    if (MR::isStep(this, 319)) {
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_FALL_DOWN", -1, -1);
    }
    if (MR::isBckStopped(getHost())) {
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_EXPLODE", -1, -1);
        MR::tryRumblePadVeryStrong(getHost(), 0);
        MR::hideModel(getHost());
        setNerve(&NrvDinoPackunDemo::DinoPackunDemoNrvAppearPowerStar::sInstance);
    }
}

void DinoPackunDemo::exeAppearPowerStar() {
    if (MR::isStep(this, 60)) {
        MR::startAfterBossBGM();
        MR::appearPowerStarContinueCurrentDemo(getHost(), getHost()->mPosition);
    }

    if (MR::isGreaterStep(this, 60)) {
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
