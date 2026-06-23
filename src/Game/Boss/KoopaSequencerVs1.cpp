#include "Game/Boss/KoopaSequencerVs1.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleMain.hpp"
#include "Game/Boss/KoopaBattleStairsVs1.hpp"
#include "Game/Boss/KoopaDemoJumpToPlanet.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaPlanetShadow.hpp"
#include "Game/Boss/KoopaSubSequenceBattle.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/KoopaBattleMapPlanet.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
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
};  // namespace

namespace NrvKoopaSequencerVs1 {
    NEW_NERVE(KoopaSequencerVs1NrvBattleStairs, KoopaSequencerVs1, BattleStairs);
    NEW_NERVE(KoopaSequencerVs1NrvDemoBattleStart, KoopaSequencerVs1, DemoBattleStart);
    NEW_NERVE(KoopaSequencerVs1NrvDemoJumpToPlanet, KoopaSequencerVs1, DemoJumpToPlanet);
    NEW_NERVE(KoopaSequencerVs1NrvBattle, KoopaSequencerVs1, Battle);
    NEW_NERVE(KoopaSequencerVs1NrvWaitDemoBattleEnd, KoopaSequencerVs1, WaitDemoBattleEnd);
    NEW_NERVE(KoopaSequencerVs1NrvDemoBattleEnd, KoopaSequencerVs1, DemoBattleEnd);
    NEW_NERVE(KoopaSequencerVs1NrvWaitDemo, KoopaSequencerVs1, WaitDemo);
};  // namespace NrvKoopaSequencerVs1

KoopaSequencerVs1::KoopaSequencerVs1() : KoopaSequencer("クッパ戦（Ｖｓ１進行役）", 1), mSubSequenceBattle(), mBattleMain() {
}

void KoopaSequencerVs1::init(Koopa* pKoopa, const JMapInfoIter& rIter) {
    KoopaSequencer::init(pKoopa, rIter);
    initNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvBattleStairs::sInstance);
    KoopaFunction::initKoopaPartsVs1(mKoopa);

    mBattleStairs = new KoopaBattleStairsVs1(mKoopa);

    mBattleMain = new KoopaBattleMain("クッパ戦闘（Ｖｓ１）", mKoopa);
    mBattleMain->init();

    mSubSequenceBattle = new KoopaSubSequenceBattle(mKoopa, this, mBattleMain);
    mSubSequenceBattle->init();

    MR::initDemoSheetTalkAnim(mKoopa, rIter, "クッパＶｓ１本戦開始", "DemoKoopaVs1Start", nullptr);
    MR::initDemoSheetTalkAnim(mKoopa, rIter, "クッパＶｓ１ダウン", "DemoKoopaVs1Down", nullptr);

    MR::declareStarPiece(mKoopa, 30);
}

void KoopaSequencerVs1::exeBattleStairs() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::setKoopaPos(mKoopa, "階段の戦い０（クッパ）");
    }

    mBattleStairs->update();

    if (MR::isOnSwitchA(mKoopa)) {
        mBattleStairs->end();

        setNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvWaitDemo::sInstance);

        KoopaFunction::setKoopaPos(mKoopa, "デモ中心");
        MR::setPlayerPosAndWait("デモ中心");

        MR::requestStartTimeKeepDemoMarioPuppetable(this, mKoopa, "クッパＶｓ１本戦開始",
                                                    &NrvKoopaSequencerVs1::KoopaSequencerVs1NrvDemoBattleStart::sInstance, nullptr, nullptr);

        MR::overlayWithPreviousScreen(2);
    }
}

void KoopaSequencerVs1::exeDemoBattleStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::endFaceCtrl(mKoopa, -1);

        MR::invalidateShadowGroup(mKoopa, "戦闘用");
        MR::validateShadowGroup(mKoopa, "デモ用");
    }

    if (MR::isDemoLastStep()) {
        MR::validateShadowGroup(mKoopa, "戦闘用");
        MR::invalidateShadowGroup(mKoopa, "デモ用");

        KoopaFunction::startFaceCtrl(mKoopa);

        mJumpToPlanet->startReady();

        setNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvDemoJumpToPlanet::sInstance);
    }
}

void KoopaSequencerVs1::exeDemoJumpToPlanet() {
    MR::updateActorStateAndNextNerve(this, mJumpToPlanet, &NrvKoopaSequencerVs1::KoopaSequencerVs1NrvBattle::sInstance);
}

void KoopaSequencerVs1::exeBattle() {
    MR::updateActorStateAndNextNerve(this, mSubSequenceBattle, &NrvKoopaSequencerVs1::KoopaSequencerVs1NrvWaitDemoBattleEnd::sInstance);
}

void KoopaSequencerVs1::exeWaitDemoBattleEnd() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::getKoopaPlanetShadow(mKoopa)->kill();

        KoopaFunction::setKoopaPos(mKoopa, "デモ中心");

        setNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvWaitDemo::sInstance);

        MR::requestStartTimeKeepDemoMarioPuppetable(this, mKoopa, "クッパＶｓ１ダウン",
                                                    &NrvKoopaSequencerVs1::KoopaSequencerVs1NrvDemoBattleEnd::sInstance, nullptr, nullptr);
    }
}

void KoopaSequencerVs1::exeDemoBattleEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAtmosphereSE("SE_AT_KOOPA_FALLDOWN_LV1");
        MR::stopStageBGM(180);

        KoopaFunction::endFaceCtrl(mKoopa, -1);

        MR::startBrk(KoopaFunction::getKoopaPlanet(mKoopa), "Death");
    }

    if (MR::isStep(this, ::sKoopaFallVoiceStep)) {
        MR::startSound(mKoopa, "SE_BV_KOOPA_BATTLE_END_FALL");
    }

    if (MR::isDemoLastStep()) {
        MR::overlayWithPreviousScreen(3);

        TVec3f namePos;
        MR::findNamePos("デモ中心", &namePos, nullptr);

        MR::startAfterBossBGM();

        MR::requestAppearPowerStar(mKoopa, namePos);
        KoopaFunction::startKoopaPlateDamageAnimPowerStarAppear(mKoopa);

        mKoopa->kill();
    }
}

void KoopaSequencerVs1::exeWaitDemo() {
}

void KoopaSequencerVs1::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvBattle::sInstance) && !mSubSequenceBattle->isDemo()) {
        mBattleMain->attackSensor(pSender, pReceiver);
    }
}

bool KoopaSequencerVs1::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvBattleStairs::sInstance)) {
        return mBattleStairs->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    if (isNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvBattle::sInstance) && !mSubSequenceBattle->isDemo()) {
        return mBattleMain->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    return false;
}

bool KoopaSequencerVs1::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool KoopaSequencerVs1::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvBattle::sInstance) && !mSubSequenceBattle->isDemo()) {
        return mBattleMain->receiveOtherMsg(msg, pSender, pReceiver);
    }

    return false;
}

KoopaSequencerVs1::~KoopaSequencerVs1() {
}
