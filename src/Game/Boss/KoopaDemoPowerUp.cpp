#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/Boss/KoopaFunction.hpp"
namespace NrvKoopaDemoPowerUp {
    NEW_NERVE(KoopaDemoPowerUpNrvWaitDemo, KoopaDemoPowerUp, WaitDemo);
    NEW_NERVE(KoopaDemoPowerUpNrvDemo, KoopaDemoPowerUp, Demo);
}  // namespace NrvKoopaDemoPowerUp

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
    if (KoopaFunction::isKoopaVs1(mParent) || KoopaFunction::isKoopaVs2(mParent)) {
        KoopaFunction::setKoopaPos(mParent, "パワーアップデモ（クッパ）");
        MR::setPlayerPosAndWait("パワーアップデモ（マリオ）");
        if (KoopaFunction::isKoopaLv3(mParent)) {
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
    MR::startBckPlayer("BattleWait", static_cast< const char* >(nullptr));
    KoopaFunction::startRecoverKoopaArmor(mParent);
    KoopaFunction::startRecoverKoopaTailThorn(mParent);
    KoopaFunction::endFaceCtrl(mParent, -1);
    setNerve(&NrvKoopaDemoPowerUp::KoopaDemoPowerUpNrvDemo::sInstance);
}

void KoopaDemoPowerUp::exeDemo() {
    if (KoopaFunction::isKoopaLv3(mParent)) {
        if (MR::isStep(this, 2)) {
            MR::startCenterScreenBlur(140, 15.0f, 80, 5, 30);
        }
    } else {
        if (MR::isStep(this, 65)) {
            MR::startCenterScreenBlur(135, 15.0f, 80, 5, 30);
        }
    }
    if (MR::isActionEnd(mParent)) {
        if (KoopaFunction::isKoopaVs3(mParent) || KoopaFunction::isKoopaLv3(mParent)) {
            KoopaFunction::endKoopaCamera(mParent, "最終パワーアップデモ", false, -1);
        } else {
            KoopaFunction::endKoopaCamera(mParent, "パワーアップデモ", false, -1);
        }
        MR::endDemo(mParent, "パワーアップデモ");
    }
    TVec3f v1 = mParent->mGravity.negateInline();
    MR::appearStarPieceToDirection(mParent, mParent->mPosition, v1, 10, 50.0f, 60.0f, false);
    MR::startSound(mParent, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    kill();
}
