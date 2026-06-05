#include "Game/Boss/KoopaDemoFallToPlanetVs3Lv3.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaPlanetShadow.hpp"
#include "Game/Boss/KoopaSwitchKeeper.hpp"
#include "Game/Map/KoopaBattleMapPlanet.hpp"

namespace {
    MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 0.98f, 0.0f};
    MR::ActorMoveParam sWaitParam = {0.0f, 1.0f, 0.9f, 1.0f};
}  // namespace

namespace MR {
    void moveAndTurnToPlayer(LiveActor*, TVec3f*, const MR::ActorMoveParam&);
}  // namespace MR

namespace NrvKoopaDemoFallToPlanetVs3Lv3 {
    NEW_NERVE(KoopaDemoFallToPlanetVs3Lv3NrvWaitFallDemo, KoopaDemoFallToPlanetVs3Lv3, WaitFallDemo);
    NEW_NERVE(KoopaDemoFallToPlanetVs3Lv3NrvFallOutside, KoopaDemoFallToPlanetVs3Lv3, FallOutside);
    NEW_NERVE(KoopaDemoFallToPlanetVs3Lv3NrvFallInside, KoopaDemoFallToPlanetVs3Lv3, FallInside);
    NEW_NERVE(KoopaDemoFallToPlanetVs3Lv3NrvLand, KoopaDemoFallToPlanetVs3Lv3, Land);
    NEW_NERVE(KoopaDemoFallToPlanetVs3Lv3NrvWaitPlayer, KoopaDemoFallToPlanetVs3Lv3, WaitPlayer);
}  // namespace NrvKoopaDemoFallToPlanetVs3Lv3

KoopaDemoFallToPlanetVs3Lv3::KoopaDemoFallToPlanetVs3Lv3(Koopa* pKoopa) : ActorStateBase< Koopa >("Demo[Ｌｖ３惑星まで落下]", pKoopa) {
}

void KoopaDemoFallToPlanetVs3Lv3::init() {
    KoopaFunction::initKoopaCamera(mHost, "惑星Ｌｖ３内側を落下");
    KoopaFunction::initKoopaAnimCamera(mHost, "DemoKoopaFallSun");
    initNerve(&NrvKoopaDemoFallToPlanetVs3Lv3::KoopaDemoFallToPlanetVs3Lv3NrvWaitFallDemo::sInstance);
}

void KoopaDemoFallToPlanetVs3Lv3::appear() {
    mIsDead = false;

    KoopaFunction::getKoopaPlanetShadow(mHost)->kill();
    KoopaFunction::endFaceCtrl(mHost, -1);
}

void KoopaDemoFallToPlanetVs3Lv3::kill() {
    mIsDead = true;

    KoopaFunction::endKoopaCamera(mHost, "惑星Ｌｖ３内側を落下", false, -1);
}

void KoopaDemoFallToPlanetVs3Lv3::exeWaitFallDemo() {
    if (KoopaFunction::tryStartKoopaAndMarioCameraDemo(mHost, "クッパＶｓ３惑星移動（Ｌｖ２からＬｖ３）", "DemoKoopaFallSun", "デモ中心")) {
        setNerve(&NrvKoopaDemoFallToPlanetVs3Lv3::KoopaDemoFallToPlanetVs3Lv3NrvFallOutside::sInstance);
    }
}

void KoopaDemoFallToPlanetVs3Lv3::exeFallOutside() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(KoopaFunction::getKoopaPlanetLv2(mHost), "Death");
        MR::showModel(KoopaFunction::getKoopaPlanetLv3(mHost));
    }

    if (MR::isStep(this, 250)) {
        MR::startStageBGM("MBGM_BOSS_KOOPA_FINAL", false);
    }

    if (MR::isStep(this, 266)) {
        KoopaFunction::appearKoopaHoleSunPlanetInside(mHost);
        MR::onSwitchDead(KoopaFunction::getKoopaSwitchKeeper(mHost));
    }

    if (MR::isStep(this, 267)) {
        KoopaFunction::getKoopaPlanet(mHost)->kill();
        KoopaFunction::getKoopaPlanetLv2(mHost)->kill();

        KoopaFunction::killKoopaHoleSunPlanetOutside(mHost);

        MR::offSwitchA(KoopaFunction::getKoopaSwitchKeeper(mHost));
        MR::onSwitchB(KoopaFunction::getKoopaSwitchKeeper(mHost));
        MR::offSwitchDead(KoopaFunction::getKoopaSwitchKeeper(mHost));
    }

    if (MR::isLessStep(this, 214)) {
        MR::startAtmosphereLevelSE("SE_BM_LV_KOOPA_FALL_SUN_1", -1, -1);
    } else if (MR::isLessStep(this, 328)) {
        MR::startAtmosphereLevelSE("SE_BM_LV_KOOPA_FALL_SUN_2", -1, -1);
    } else {
        MR::startAtmosphereLevelSE("SE_BM_LV_KOOPA_FALL_SUN_3", -1, -1);
    }

    if (MR::isStep(this, 328)) {
        MR::startAtmosphereSE("SE_BM_KOOPA_FALL_SUN_3_WIND", -1, -1);
    }

    if (KoopaFunction::tryEndKoopaCameraDemo(mHost, "クッパＶｓ３惑星移動（Ｌｖ２からＬｖ３）", "DemoKoopaFallSun")) {
        KoopaFunction::setKoopaPos(mHost, "Ｌｖ３内側（クッパ）");
        MR::setPlayerPosAndWait("Ｌｖ３内側（マリオ）");

        KoopaFunction::startKoopaCamera(mHost, "惑星Ｌｖ３内側を落下");
        MR::startAction(mHost, "JumpSoon");

        KoopaFunction::getKoopaPlanetShadow(mHost)->appear();

        setNerve(&NrvKoopaDemoFallToPlanetVs3Lv3::KoopaDemoFallToPlanetVs3Lv3NrvFallInside::sInstance);
    }
}

void KoopaDemoFallToPlanetVs3Lv3::exeFallInside() {
    MR::startAtmosphereLevelSE("SE_BM_LV_KOOPA_FALL_SUN_1", -1, -1);
    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sFallParam);

    if (MR::isBindedGround(mHost)) {
        MR::zeroVelocity(mHost);
        setNerve(&NrvKoopaDemoFallToPlanetVs3Lv3::KoopaDemoFallToPlanetVs3Lv3NrvLand::sInstance);
    }
}

void KoopaDemoFallToPlanetVs3Lv3::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Land");
        MR::startSound(mHost, "SE_BM_KOOPA_FLIP_RECOVER_END", -1, -1);
        MR::startSound(mHost, "SE_BV_KOOPA_LAND_HEAVY", -1, -1);

        MR::tryRumblePadStrong(mHost, 0);
        MR::shakeCameraNormalStrong();
    }

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvKoopaDemoFallToPlanetVs3Lv3::KoopaDemoFallToPlanetVs3Lv3NrvWaitPlayer::sInstance);
    }
}

void KoopaDemoFallToPlanetVs3Lv3::exeWaitPlayer() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::startFaceCtrl(mHost);
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sWaitParam);

    if (MR::isOnGroundPlayer()) {
        kill();
    }
}

KoopaDemoFallToPlanetVs3Lv3::~KoopaDemoFallToPlanetVs3Lv3() {
}
