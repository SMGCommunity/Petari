#include "Game/Boss//KoopaDemoJumpToPlanet.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaPlanetShadow.hpp"

namespace {
    MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 0.98f, 0.0f};
    MR::ActorMoveParam sWaitParam = {0.0f, 1.0f, 0.9f, 1.0f};
};  // namespace

namespace MR {
    void moveAndTurnToPlayer(LiveActor*, TVec3f*, const MR::ActorMoveParam&);
};  // namespace MR

namespace NrvKoopaDemoJumpToPlanet {
    NEW_NERVE(KoopaDemoJumpToPlanetNrvStart, KoopaDemoJumpToPlanet, Start);
    NEW_NERVE(KoopaDemoJumpToPlanetNrvFall, KoopaDemoJumpToPlanet, Fall);
    NEW_NERVE(KoopaDemoJumpToPlanetNrvLand, KoopaDemoJumpToPlanet, Land);
    NEW_NERVE(KoopaDemoJumpToPlanetNrvWaitPlayer, KoopaDemoJumpToPlanet, WaitPlayer);
};  // namespace NrvKoopaDemoJumpToPlanet

KoopaDemoJumpToPlanet::KoopaDemoJumpToPlanet(Koopa* pKoopa) : ActorStateBase< Koopa >("Demo[惑星までジャンプ]", pKoopa) {
}

void KoopaDemoJumpToPlanet::init() {
    KoopaFunction::initKoopaCamera(mHost, "ウェイト（惑星までジャンプ）");
    KoopaFunction::initKoopaCamera(mHost, "落下（惑星までジャンプ）");
    KoopaFunction::initKoopaCamera(mHost, "ワープ後（惑星までジャンプ）");
    initNerve(&NrvKoopaDemoJumpToPlanet::KoopaDemoJumpToPlanetNrvStart::sInstance);
}

void KoopaDemoJumpToPlanet::kill() {
    mIsDead = true;

    KoopaFunction::endKoopaCamera(mHost, "ウェイト（惑星までジャンプ）", false, -1);
    KoopaFunction::endKoopaCamera(mHost, "落下（惑星までジャンプ）", false, -1);
    KoopaFunction::endKoopaCamera(mHost, "ワープ後（惑星までジャンプ）", false, -1);
}

void KoopaDemoJumpToPlanet::startReady() {
    MR::onSwitchB(mHost);
}

void KoopaDemoJumpToPlanet::exeStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::setKoopaPos(mHost, "戦闘開始（クッパ）");
        MR::setPlayerPosAndWait("戦闘開始（マリオ）");

        KoopaFunction::startKoopaCamera(mHost, "落下（惑星までジャンプ）");
        MR::startAction(mHost, "JumpSoon");
        KoopaFunction::startFaceCtrl(mHost);

        setNerve(&NrvKoopaDemoJumpToPlanet::KoopaDemoJumpToPlanetNrvFall::sInstance);
    }
}

void KoopaDemoJumpToPlanet::exeFall() {
    if (MR::isFirstStep(this) && KoopaFunction::getKoopaPlanetShadow(mHost)) {
        KoopaFunction::getKoopaPlanetShadow(mHost)->appear();
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sFallParam);

    if (KoopaFunction::isKoopaVs3(mHost) && MR::isStep(this, 90)) {
        KoopaFunction::setKoopaPos(mHost, "Ｌｖ１開始（クッパ）");
        MR::setPlayerPosAndWait("Ｌｖ１開始（マリオ）");

        KoopaFunction::endKoopaCamera(mHost, "落下（惑星までジャンプ）", false, -1);
        KoopaFunction::startKoopaCamera(mHost, "ワープ後（惑星までジャンプ）");
    }

    MR::startLevelSound(mHost, "SE_BM_LV_KOOPA_FALL_TO_PLANET", -1, -1, -1);

    if (MR::isBindedGround(mHost)) {
        MR::zeroVelocity(mHost);

        setNerve(&NrvKoopaDemoJumpToPlanet::KoopaDemoJumpToPlanetNrvLand::sInstance);
    }
}

void KoopaDemoJumpToPlanet::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Land");
        MR::startSound(mHost, "SE_BM_KOOPA_FLIP_RECOVER_END", -1, -1);
        MR::startSound(mHost, "SE_BV_KOOPA_LAND_HEAVY", -1, -1);

        MR::tryRumblePadStrong(mHost, 0);
        MR::shakeCameraNormalStrong();
    }

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvKoopaDemoJumpToPlanet::KoopaDemoJumpToPlanetNrvWaitPlayer::sInstance);
    }
}

void KoopaDemoJumpToPlanet::exeWaitPlayer() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::startFaceCtrl(mHost);
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sWaitParam);

    if (MR::isOnGroundPlayer()) {
        KoopaFunction::startFaceCtrl(mHost);

        kill();
    }
}

KoopaDemoJumpToPlanet::~KoopaDemoJumpToPlanet() {
}
