#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/Boss/KoopaFunction.hpp"

namespace NrvKoopaDemoPowerUp {
    NEW_NERVE(KoopaDemoPowerUpNrvWaitDemo, KoopaDemoPowerUp, WaitDemo);
    NEW_NERVE(KoopaDemoPowerUpNrvDemo, KoopaDemoPowerUp, Demo);
}

KoopaDemoPowerUp::KoopaDemoPowerUp(Koopa* pKoopa) : NerveExecutor("Demo[パワーアップ]"), mParent(pKoopa) {}

void KoopaDemoPowerUp::init() {
    KoopaFunction::initKoopaCamera(mParent, "パワーアップデモ");
    KoopaFunction::initKoopaCamera(mParent, "最終パワーアップデモ");
    MR::createCenterScreenBlur();
    initNerve(&NrvKoopaDemoPowerUp::KoopaDemoPowerUpNrvWaitDemo::sInstance);
}

void KoopaDemoPowerUp::appear() {
    _8 = false;
    setNerve(&NrvKoopaDemoPowerUp::KoopaDemoPowerUpNrvWaitDemo::sInstance);
}

void KoopaDemoPowerUp::kill() {
    _8 = true;
    KoopaFunction::endKoopaCamera(mParent, "パワーアップデモ", false, -1);
    KoopaFunction::endKoopaCamera(mParent, "最終パワーアップデモ", false, -1);
}

void KoopaDemoPowerUp::exeWaitDemo() {
    bool v1 = MR::tryStartDemoMarioPuppetable(mParent, "パワーアップデモ");
    if (!v1) {
        return;
    }
    if(KoopaFunction::isKoopaVs1(mParent) || KoopaFunction::isKoopaVs2(mParent)) {
        KoopaFunction::setKoopaPos(mParent, "パワーアップデモ（クッパ）");
        MR::setPlayerPosAndWait("パワーアップデモ（マリオ）");
        if(KoopaFunction::isKoopaLv3(mParent)) {
            MR::startAction(mParent, "DemoKoopaPowerUpFinal");
            KoopaFunction::startKoopaTargetCamera(mParent, "最終パワーアップデモ");
            MR::onSwitchB(KoopaFunction::getKoopaPowerUpSwitch(mParent));
        } else {
            MR::startAction(mParent, "DemoKoopaPowerUp");
            KoopaFunction::startKoopaTargetCamera(mParent, "パワーアップデモ");
            MR::onSwitchA(KoopaFunction::getKoopaPowerUpSwitch(mParent));
        }
    } else {
        KoopaFunction::setKoopaPos(mParent, "パワーアップデモＬｖ３（クッパ）");
        MR::setPlayerPosAndWait("パワーアップデモＬｖ３（マリオ）");
        MR::startAction(mParent, "DemoKoopaPowerUpFinal");
        KoopaFunction::startKoopaTargetCamera(mParent, "最終パワーアップデモ");
        MR::onSwitchA(KoopaFunction::getKoopaPowerUpSwitch(mParent));
    }
    MR::startBckPlayer("BattleWait", static_cast<const char*>(nullptr));
    KoopaFunction::startRecoverKoopaArmor(mParent);
    KoopaFunction::startRecoverKoopaTailThorn(mParent);
    KoopaFunction::endFaceCtrl(mParent, -1);
    setNerve(&NrvKoopaDemoPowerUp::KoopaDemoPowerUpNrvDemo::sInstance);
}
