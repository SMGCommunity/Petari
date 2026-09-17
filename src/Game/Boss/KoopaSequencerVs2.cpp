#include "Game/Boss/KoopaSequencerVs2.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleMain.hpp"
#include "Game/Boss/KoopaDemoJumpToPlanet.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaSubSequenceBattle.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/KoopaBattleMapPlanet.hpp"
#include "Game/Util/ActorStateUtil.hpp"
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

namespace NrvKoopaSequencerVs2 {
    NEW_NERVE(KoopaSequencerVs2NrvWaitDemoBattleStart, KoopaSequencerVs2, WaitDemoBattleStart);
    NEW_NERVE(KoopaSequencerVs2NrvDemoBattleStart, KoopaSequencerVs2, DemoBattleStart);
    NEW_NERVE(KoopaSequencerVs2NrvDemoJumpToPlanet, KoopaSequencerVs2, DemoJumpToPlanet);
    NEW_NERVE(KoopaSequencerVs2NrvBattle, KoopaSequencerVs2, Battle);
    NEW_NERVE(KoopaSequencerVs2NrvWaitDemoDown, KoopaSequencerVs2, WaitDemoDown);
    NEW_NERVE(KoopaSequencerVs2NrvDemoDown, KoopaSequencerVs2, DemoDown);
    NEW_NERVE(KoopaSequencerVs2NrvWaitDemo, KoopaSequencerVs2, WaitDemo);
};  // namespace NrvKoopaSequencerVs2

KoopaSequencerVs2::KoopaSequencerVs2() : KoopaSequencer("クッパ戦（Ｖｓ２進行役）", 2), mSubSequenceBattle(), mBattleMain() {
}

void KoopaSequencerVs2::init(Koopa* pKoopa, const JMapInfoIter& rIter) {
    KoopaSequencer::init(pKoopa, rIter);
    KoopaFunction::setKoopaPos(mKoopa, "デモ中心");
    MR::startBck(mKoopa, "DemoKoopaVs2Start", nullptr);

    initNerve(GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvWaitDemoBattleStart));
    KoopaFunction::initKoopaPartsVs2(mKoopa);

    mBattleMain = new KoopaBattleMain("クッパ戦闘（Ｖｓ２）", mKoopa);
    mBattleMain->init();

    mSubSequenceBattle = new KoopaSubSequenceBattle(mKoopa, this, mBattleMain);
    mSubSequenceBattle->init();

    MR::initDemoSheetTalkAnim(mKoopa, rIter, "クッパＶｓ２本戦開始", "DemoKoopaVs2Start", nullptr);
    MR::initDemoSheetTalkAnimWithMessageName(mKoopa, rIter, "クッパＶｓ２ダウン", "DemoKoopaVs1Down", "DemoKoopaVs2Down");

    MR::declareStarPiece(mKoopa, 30);
}

void KoopaSequencerVs2::exeWaitDemoBattleStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::setKoopaPos(mKoopa, "デモ中心");
    }

    if (MR::tryStartTimeKeepDemoMarioPuppetable(mKoopa, "クッパＶｓ２本戦開始", nullptr)) {
        setNerve(GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvDemoBattleStart));
    }
}

void KoopaSequencerVs2::exeDemoBattleStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::endFaceCtrl(mKoopa, -1);
    }

    if (MR::isDemoLastStep()) {
        KoopaFunction::startFaceCtrl(mKoopa);
        mJumpToPlanet->startReady();

        setNerve(GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvDemoJumpToPlanet));
    }
}

void KoopaSequencerVs2::exeDemoJumpToPlanet() {
    MR::updateActorStateAndNextNerve(this, mJumpToPlanet, GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvBattle));
}

void KoopaSequencerVs2::exeBattle() {
    MR::updateActorStateAndNextNerve(this, mSubSequenceBattle, GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvWaitDemoDown));
}

void KoopaSequencerVs2::exeWaitDemoDown() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::setKoopaPos(mKoopa, "デモ中心");

        setNerve(GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvWaitDemo));

        MR::requestStartTimeKeepDemoMarioPuppetable(this, mKoopa, "クッパＶｓ２ダウン", GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvDemoDown),
                                                    nullptr, nullptr);
    }
}

void KoopaSequencerVs2::exeDemoDown() {
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

        MR::setPlayerPosAndWait("ダウンデモ後（マリオ）");

        TVec3f namePos;
        MR::findNamePos("デモ中心", &namePos, nullptr);

        MR::startAfterBossBGM();

        MR::requestAppearPowerStar(mKoopa, namePos);
        KoopaFunction::startKoopaPlateDamageAnimPowerStarAppear(mKoopa);

        mKoopa->kill();
    }
}

void KoopaSequencerVs2::exeWaitDemo() {
}

void KoopaSequencerVs2::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvBattle)) && !mSubSequenceBattle->isDemo()) {
        mBattleMain->attackSensor(pSender, pReceiver);
    }
}

bool KoopaSequencerVs2::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvBattle)) && !mSubSequenceBattle->isDemo()) {
        return mBattleMain->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    return false;
}

bool KoopaSequencerVs2::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool KoopaSequencerVs2::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaSequencerVs2, KoopaSequencerVs2NrvBattle)) && !mSubSequenceBattle->isDemo()) {
        return mBattleMain->receiveOtherMsg(msg, pSender, pReceiver);
    }

    return false;
}

KoopaSequencerVs2::~KoopaSequencerVs2() {
}
