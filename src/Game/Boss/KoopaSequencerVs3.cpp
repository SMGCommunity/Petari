#include "Game/Boss/KoopaSequencerVs3.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleMain.hpp"
#include "Game/Boss/KoopaBattleStairsVs3.hpp"
#include "Game/Boss/KoopaBattleVs3Lv1.hpp"
#include "Game/Boss/KoopaBattleVs3Lv2.hpp"
#include "Game/Boss/KoopaDemoFallToPlanetVs3Lv2.hpp"
#include "Game/Boss/KoopaDemoFallToPlanetVs3Lv3.hpp"
#include "Game/Boss/KoopaDemoJumpToPlanet.hpp"
#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaPlanetShadow.hpp"
#include "Game/Boss/KoopaSwitchKeeper.hpp"
#include "Game/Boss/KoopaViewSwitchKeeper.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/KoopaBattleMapPlanet.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sKoopaFallVoiceStep = 20;
    static const s32 sLastImpactSeStep = 70;
    static const s32 sFallSeStartStep = 471;
    static const s32 sFallAmbientSeStartStep = 544;
    static const s32 sFallBurnStartStep = 930;
    static const s32 sFallEndStep = 1049;
    static const s32 sDownCameraCoverStep = 640;
};  // namespace

namespace NrvKoopaSequencerVs3 {
    NEW_NERVE(KoopaSequencerVs3NrvWaitPlayer, KoopaSequencerVs3, WaitPlayer);
    NEW_NERVE(KoopaSequencerVs3NrvBattleStairs, KoopaSequencerVs3, BattleStairs);
    NEW_NERVE(KoopaSequencerVs3NrvDemoBattleStart, KoopaSequencerVs3, DemoBattleStart);
    NEW_NERVE(KoopaSequencerVs3NrvDemoJumpToPlanet, KoopaSequencerVs3, DemoJumpToPlanet);
    NEW_NERVE(KoopaSequencerVs3NrvBattleVs3Lv1, KoopaSequencerVs3, BattleVs3Lv1);
    NEW_NERVE(KoopaSequencerVs3NrvFallToPlanetLv2, KoopaSequencerVs3, FallToPlanetLv2);
    NEW_NERVE(KoopaSequencerVs3NrvBattleVs3Lv2, KoopaSequencerVs3, BattleVs3Lv2);
    NEW_NERVE(KoopaSequencerVs3NrvFallToPlanetLv3, KoopaSequencerVs3, FallToPlanetLv3);
    NEW_NERVE(KoopaSequencerVs3NrvBattleVs3Lv3, KoopaSequencerVs3, BattleVs3Lv3);
    NEW_NERVE(KoopaSequencerVs3NrvDemoPowerUpFinal, KoopaSequencerVs3, DemoPowerUpFinal);
    NEW_NERVE(KoopaSequencerVs3NrvBattleFinal, KoopaSequencerVs3, BattleFinal);
    NEW_NERVE(KoopaSequencerVs3NrvWaitDemoDown, KoopaSequencerVs3, WaitDemoDown);
    NEW_NERVE(KoopaSequencerVs3NrvDemoDown, KoopaSequencerVs3, DemoDown);
    NEW_NERVE(KoopaSequencerVs3NrvDead, KoopaSequencerVs3, Dead);
    NEW_NERVE(KoopaSequencerVs3NrvWaitDemo, KoopaSequencerVs3, WaitDemo);
};  // namespace NrvKoopaSequencerVs3

KoopaSequencerVs3::KoopaSequencerVs3()
    : KoopaSequencer("クッパ戦（Ｖｓ３進行役）", 3), mBattleLv1(), mBattleLv2(), mBattleMain(), mDemoFallToPlanetLv2(), mDemoFallToPlanetLv3() {
}

void KoopaSequencerVs3::init(Koopa* pKoopa, const JMapInfoIter& rIter) {
    KoopaSequencer::init(pKoopa, rIter);
    initNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvWaitPlayer::sInstance);
    KoopaFunction::initKoopaVs3(mKoopa);

    mBattleStairs = new KoopaBattleStairsVs3(mKoopa);

    mDemoFallToPlanetLv2 = new KoopaDemoFallToPlanetVs3Lv2(mKoopa);
    mDemoFallToPlanetLv2->init();

    mDemoFallToPlanetLv3 = new KoopaDemoFallToPlanetVs3Lv3(mKoopa);
    mDemoFallToPlanetLv3->init();

    mBattleLv1 = new KoopaBattleVs3Lv1(mKoopa);
    mBattleLv1->init();

    mBattleLv2 = new KoopaBattleVs3Lv2(mKoopa);
    mBattleLv2->init();

    mBattleMain = new KoopaBattleMain("クッパ戦闘（Ｖｓ３Ｌｖ３）", mKoopa);
    mBattleMain->init();

    MR::initDemoSheetTalkAnim(KoopaFunction::getKoopaDemoKoopaJr(mKoopa), rIter, "クッパＶｓ３ピーチ登場", "DemoKoopaMeetPeach", nullptr);

    MR::initTalkAnimDemoCast(mKoopa, rIter, "クッパＶｓ３ピーチ登場", "DemoKoopaMeetPeach");
    MR::initTalkAnimDemoCast(KoopaFunction::getKoopaDemoPeach(mKoopa), rIter, "クッパＶｓ３ピーチ登場", "DemoKoopaMeetPeach");
    MR::initTalkAnimDemoCast(KoopaFunction::getKoopaDemoKoopaJrShip(mKoopa), rIter, "クッパＶｓ３ピーチ登場", "DemoKoopaMeetPeach");

    KoopaFunction::initKoopaAnimCamera(mKoopa, "DemoKoopaBattleStairsVs3Start");

    MR::initDemoSheetTalkAnim(mKoopa, rIter, "クッパＶｓ３本戦開始", "DemoKoopaVs3Start", nullptr);

    MR::initTalkAnimDemoCast(KoopaFunction::getKoopaDemoKoopaJr(mKoopa), rIter, "クッパＶｓ３本戦開始", "DemoKoopaVs3Start");
    MR::initTalkAnimDemoCast(KoopaFunction::getKoopaDemoPeach(mKoopa), rIter, "クッパＶｓ３本戦開始", "DemoKoopaVs3Start");
    MR::initTalkAnimDemoCast(KoopaFunction::getKoopaDemoKoopaJrShip(mKoopa), rIter, "クッパＶｓ３本戦開始", "DemoKoopaVs3Start");

    KoopaFunction::initKoopaAnimCamera(mKoopa, "DemoKoopaVs3Down");

    MR::declareStarPiece(mKoopa, 30);
}

void KoopaSequencerVs3::exeWaitPlayer() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::setKoopaPos(mKoopa, "デモ中心");

        MR::startAction(mKoopa, "Wait");

        MR::hideModel(KoopaFunction::getKoopaPlanetLv3(mKoopa));
    }

    if (MR::isOnSwitchAppear(KoopaFunction::getKoopaViewSwitchKeeper(mKoopa)) && MR::isHiddenModel(mKoopa)) {
        MR::showModel(mKoopa);
    }

    if (MR::isOnSwitchA(KoopaFunction::getKoopaViewSwitchKeeper(mKoopa))) {
        if (!MR::isHiddenModel(KoopaFunction::getKoopaPlanet(mKoopa))) {
            MR::hideModel(KoopaFunction::getKoopaPlanet(mKoopa));
            MR::hideModel(KoopaFunction::getKoopaPlanetLv2(mKoopa));
            KoopaFunction::killKoopaHoleSunPlanetOutside(mKoopa);
        }
    } else {
        if (MR::isHiddenModel(KoopaFunction::getKoopaPlanet(mKoopa))) {
            MR::showModel(KoopaFunction::getKoopaPlanet(mKoopa));
            MR::showModel(KoopaFunction::getKoopaPlanetLv2(mKoopa));
            KoopaFunction::appearKoopaHoleSunPlanetOutside(mKoopa);
        }
    }

    if (MR::isDemoActive("クッパＶｓ３ピーチ登場") && MR::isDemoLastStep()) {
        KoopaFunction::endKoopaDemoMeetPeach(mKoopa);
    }

    if (MR::isOnSwitchAppear(KoopaFunction::getKoopaSwitchKeeper(mKoopa))) {
        MR::overlayWithPreviousScreen(2);

        setNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleStairs::sInstance);
    }
}

void KoopaSequencerVs3::exeBattleStairs() {
    mBattleStairs->update();

    if (MR::isOnSwitchA(mKoopa)) {
        mBattleStairs->end();

        setNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvWaitDemo::sInstance);

        MR::requestStartTimeKeepDemoMarioPuppetable(this, mKoopa, "クッパＶｓ３本戦開始",
                                                    &NrvKoopaSequencerVs3::KoopaSequencerVs3NrvDemoBattleStart::sInstance, nullptr, nullptr);
    }
}

void KoopaSequencerVs3::exeDemoBattleStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::endFaceCtrl(mKoopa, -1);
    }

    if (MR::isDemoLastStep()) {
        KoopaFunction::startFaceCtrl(mKoopa);
        KoopaFunction::invalidateKoopaNpcLod(mKoopa);

        mJumpToPlanet->startReady();

        setNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvDemoJumpToPlanet::sInstance);
    }
}

void KoopaSequencerVs3::exeDemoJumpToPlanet() {
    MR::updateActorStateAndNextNerve(this, mJumpToPlanet, &NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv1::sInstance);
}

void KoopaSequencerVs3::exeBattleVs3Lv1() {
    if (MR::isFirstStep(this)) {
        mBattleLv1->appear();
    }

    if (mBattleLv1->update()) {
        setNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvFallToPlanetLv2::sInstance);
    }
}

void KoopaSequencerVs3::exeFallToPlanetLv2() {
    if (MR::isFirstStep(this)) {
        MR::startAtmosphereSE("SE_AT_KOOPA_FALLDOWN_LV1");
    }

    if (MR::isStep(this, ::sKoopaFallVoiceStep)) {
        MR::startSound(mKoopa, "SE_BV_KOOPA_BATTLE_END_FALL");
    }

    MR::updateActorStateAndNextNerve(this, mDemoFallToPlanetLv2, &NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv2::sInstance);
}

void KoopaSequencerVs3::exeBattleVs3Lv2() {
    if (MR::isFirstStep(this)) {
        startBattleLevel2();
        mBattleLv2->appear();
    }

    if (mBattleLv2->update()) {
        setNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvFallToPlanetLv3::sInstance);
    }
}

void KoopaSequencerVs3::exeFallToPlanetLv3() {
    if (MR::isFirstStep(this)) {
        MR::startAtmosphereSE("SE_AT_KOOPA_FALLDOWN_LV1");
    }

    if (MR::isStep(this, ::sKoopaFallVoiceStep)) {
        MR::startSound(mKoopa, "SE_BV_KOOPA_BATTLE_END_FALL");
    }

    MR::updateActorStateAndNextNerve(this, mDemoFallToPlanetLv3, &NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv3::sInstance);
}

void KoopaSequencerVs3::exeBattleVs3Lv3() {
    if (MR::isFirstStep(this)) {
        startBattleLevel3();
        mBattleMain->appear();
    }

    if (mBattleMain->update()) {
        setNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvDemoPowerUpFinal::sInstance);
    }
}

void KoopaSequencerVs3::exeDemoPowerUpFinal() {
    MR::updateActorStateAndNextNerve(this, mPowerUp, &NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleFinal::sInstance);
}

void KoopaSequencerVs3::exeBattleFinal() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::startKoopaAngry(mKoopa);
        mBattleMain->appear();
    }

    if (mBattleMain->update()) {
        KoopaFunction::getKoopaPlanetShadow(mKoopa)->kill();

        setNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvWaitDemoDown::sInstance);
    }
}

void KoopaSequencerVs3::exeWaitDemoDown() {
    if (KoopaFunction::tryStartKoopaAndMarioCameraDemo(mKoopa, "クッパＶｓ３ダウン", "DemoKoopaVs3Down", "ダウンデモ")) {
        setNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvDemoDown::sInstance);
    }
}

void KoopaSequencerVs3::exeDemoDown() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(10);
        KoopaFunction::endFaceCtrl(mKoopa, -1);

        MR::offCalcGravity(mKoopa);

        MR::startBrk(KoopaFunction::getKoopaPlanetLv3(mKoopa), "Death");
    }

    if (MR::isStep(this, ::sLastImpactSeStep)) {
        MR::startSound(mKoopa, "SE_BM_KOOPA_DOWN_LAST_IMPACT");
    }

    if (MR::isStep(this, ::sFallSeStartStep)) {
        MR::startSound(mKoopa, "SE_BM_KOOPA_DOWN_TO_SUN_TRG");
    }

    if (MR::isGreaterStep(this, ::sFallSeStartStep) && MR::isLessStep(this, ::sFallEndStep)) {
        MR::startLevelSound(mKoopa, "SE_BM_LV_KOOPA_DOWN_TO_SUN");

        if (MR::isGreaterStep(this, ::sFallAmbientSeStartStep)) {
            MR::startLevelSound(mKoopa, "SE_BM_LV_KOOPA_DOWN_LAVA");
        }
    }

    if (MR::isGreaterStep(this, ::sFallBurnStartStep) && MR::isLessStep(this, ::sFallEndStep)) {
        MR::startLevelSound(mKoopa, "SE_BM_LV_KOOPA_FALL_BURN");
    }

    if (MR::isStep(this, ::sFallEndStep)) {
        MR::startSound(mKoopa, "SE_BM_KOOPA_DOWN_INTO_SUN");
    }

    if (MR::isStep(this, ::sDownCameraCoverStep)) {
        MR::overlayWithPreviousScreen(3);
    }

    if (KoopaFunction::tryEndKoopaCameraDemo(mKoopa, "クッパＶｓ３ダウン", "DemoKoopaVs3Down")) {
        MR::setPlayerPosAndWait("ダウンデモ後（マリオ）");

        MR::requestAppearPowerStar(mKoopa, KoopaFunction::getKoopaPlanetLv3(mKoopa)->mPosition);
        KoopaFunction::startKoopaPlateDamageAnimPowerStarAppear(mKoopa);

        mKoopa->kill();
    }
}

void KoopaSequencerVs3::exeDead() {
}

void KoopaSequencerVs3::exeWaitDemo() {
}

void KoopaSequencerVs3::calcAndSetBaseMtx() {
    if (!(((isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv1::sInstance) && mBattleLv1->tryCalcAndSetBaseMtx()) ||
           (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv2::sInstance) && mBattleLv2->tryCalcAndSetBaseMtx())) ||
          ((isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv3::sInstance) ||
            isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleFinal::sInstance)) &&
           mBattleMain->tryCalcAndSetBaseMtx()))) {
        KoopaSequencer::calcAndSetBaseMtx();
    }
}

void KoopaSequencerVs3::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv1::sInstance)) {
        mBattleLv1->attackSensor(pSender, pReceiver);
    } else if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv2::sInstance)) {
        mBattleLv2->attackSensor(pSender, pReceiver);
    } else if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv3::sInstance) ||
               isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleFinal::sInstance)) {
        mBattleMain->attackSensor(pSender, pReceiver);
    }
}

bool KoopaSequencerVs3::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleStairs::sInstance)) {
        return mBattleStairs->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv1::sInstance)) {
        return mBattleLv1->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv2::sInstance)) {
        return mBattleLv2->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv3::sInstance) ||
        isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleFinal::sInstance)) {
        return mBattleMain->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    return false;
}

bool KoopaSequencerVs3::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv2::sInstance)) {
        return mBattleLv2->receiveMsgEnemyAttack(msg, pSender, pReceiver);
    }

    return false;
}

bool KoopaSequencerVs3::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv1::sInstance)) {
        return false;
    }

    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv2::sInstance)) {
        return false;
    }

    if (isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleVs3Lv3::sInstance) ||
        isNerve(&NrvKoopaSequencerVs3::KoopaSequencerVs3NrvBattleFinal::sInstance)) {
        return mBattleMain->receiveOtherMsg(msg, pSender, pReceiver);
    }

    return false;
}

KoopaSequencerVs3::~KoopaSequencerVs3() {
}
