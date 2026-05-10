#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Util/ScreenUtil.hpp"

namespace NrvKoopaDemoPowerUp {
    NEW_NERVE(KoopaDemoPowerUpNrvWaitDemo, KoopaDemoPowerUp, WaitDemo);
    NEW_NERVE(KoopaDemoPowerUpNrvDemo, KoopaDemoPowerUp, Demo);
};  // namespace NrvKoopaDemoPowerUp

KoopaDemoPowerUp::~KoopaDemoPowerUp() {
    return;
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
    if (MR::tryStartDemoMarioPuppetable(getHost(), "パワーアップデモ")) {
        if (KoopaFunction::isKoopaVs1(getHost()) || KoopaFunction::isKoopaVs2(getHost())) {
            KoopaFunction::setKoopaPos(getHost(), "パワーアップデモ（クッパ）");
            MR::setPlayerPosAndWait("パワーアップデモ（マリオ");

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
}

// KoopaDemoPowerUp::exeDemo()
