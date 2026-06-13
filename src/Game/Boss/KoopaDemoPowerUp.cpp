#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaPowerUpSwitch.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sStepToScreenBlur = 65;
    static const s32 sScreenBlurStep = 135;
    static const s32 sStepToScreenBlurFinal = 50;
    static const s32 sScreenBlurStepFinal = 140;
};  // namespace

namespace NrvKoopaDemoPowerUp {
    NEW_NERVE(KoopaDemoPowerUpNrvWaitDemo, KoopaDemoPowerUp, WaitDemo);
    NEW_NERVE(KoopaDemoPowerUpNrvDemo, KoopaDemoPowerUp, Demo);
};  // namespace NrvKoopaDemoPowerUp

KoopaDemoPowerUp::~KoopaDemoPowerUp() {
}

KoopaDemoPowerUp::KoopaDemoPowerUp(Koopa* pKoopa) : ActorStateBase< Koopa >("Demo[パワーアップ]", pKoopa) {
}

void KoopaDemoPowerUp::init() {
    KoopaFunction::initKoopaCamera(getHost(), "パワーアップデモ");
    KoopaFunction::initKoopaCamera(getHost(), "最終パワーアップデモ");

    MR::createCenterScreenBlur();
    initNerve(&NrvKoopaDemoPowerUp::KoopaDemoPowerUpNrvWaitDemo::sInstance);
}

void KoopaDemoPowerUp::appear() {
    mIsDead = false;

    setNerve(&NrvKoopaDemoPowerUp::KoopaDemoPowerUpNrvWaitDemo::sInstance);
}

void KoopaDemoPowerUp::kill() {
    mIsDead = true;

    KoopaFunction::endKoopaCamera(getHost(), "パワーアップデモ", false, -1);
    KoopaFunction::endKoopaCamera(getHost(), "最終パワーアップデモ", false, -1);
}

void KoopaDemoPowerUp::exeWaitDemo() {
    if (!MR::tryStartDemoMarioPuppetable(getHost(), "パワーアップデモ")) {
        return;
    }

    if (KoopaFunction::isKoopaVs1(getHost()) || KoopaFunction::isKoopaVs2(getHost())) {
        KoopaFunction::setKoopaPos(getHost(), "パワーアップデモ（クッパ）");
        MR::setPlayerPosAndWait("パワーアップデモ（マリオ）");

        if (KoopaFunction::isKoopaLv3(getHost())) {
            MR::startAction(getHost(), "DemoKoopaPowerUpFinal");
            KoopaFunction::startKoopaTargetCamera(getHost(), "最終パワーアップデモ");

            MR::onSwitchB(KoopaFunction::getKoopaPowerUpSwitch(getHost()));
        } else {
            MR::startAction(getHost(), "DemoKoopaPowerUp");
            KoopaFunction::startKoopaTargetCamera(getHost(), "パワーアップデモ");

            MR::onSwitchA(KoopaFunction::getKoopaPowerUpSwitch(getHost()));
        }
    } else {
        KoopaFunction::setKoopaPos(getHost(), "パワーアップデモＬｖ３（クッパ）");
        MR::setPlayerPosAndWait("パワーアップデモＬｖ３（マリオ）");

        MR::startAction(getHost(), "DemoKoopaPowerUpFinal");
        KoopaFunction::startKoopaTargetCamera(getHost(), "最終パワーアップデモ");

        MR::onSwitchA(KoopaFunction::getKoopaPowerUpSwitch(getHost()));
    }

    MR::startBckPlayer("BattleWait", static_cast< const char* >(nullptr));
    KoopaFunction::startRecoverKoopaArmor(getHost());
    KoopaFunction::startRecoverKoopaTailThorn(getHost());

    KoopaFunction::endFaceCtrl(getHost(), -1);

    setNerve(&NrvKoopaDemoPowerUp::KoopaDemoPowerUpNrvDemo::sInstance);
}

void KoopaDemoPowerUp::exeDemo() {
    if (KoopaFunction::isKoopaLv3(mHost)) {
        if (MR::isStep(this, ::sStepToScreenBlurFinal)) {
            MR::startCenterScreenBlur(::sScreenBlurStepFinal, 15.0f, 80, 5, 30);
        }
    } else {
        if (MR::isStep(this, ::sStepToScreenBlur)) {
            MR::startCenterScreenBlur(::sScreenBlurStep, 15.0f, 80, 5, 30);
        }
    }

    if (!MR::isActionEnd(mHost)) {
        return;
    }

    if (KoopaFunction::isKoopaVs3(mHost) || KoopaFunction::isKoopaLv3(mHost)) {
        KoopaFunction::endKoopaCamera(mHost, "最終パワーアップデモ", false, -1);
    } else {
        KoopaFunction::endKoopaCamera(mHost, "パワーアップデモ", false, -1);
    }

    MR::endDemo(mHost, "パワーアップデモ");

    TVec3f gravity;
    gravity.negate(mHost->mGravity);
    MR::appearStarPieceToDirection(mHost, mHost->mPosition, gravity, 10, 50.0f, 60.0f, false);

    MR::startSound(mHost, "SE_OJ_STAR_PIECE_BURST");

    kill();
}
