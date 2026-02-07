#include "Game/Boss/KoopaDemoJumpToPlanet.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace NrvKoopaDemoJumpToPlanet {
    NEW_NERVE(KoopaDemoJumpToPlanetNrvStart, KoopaDemoJumpToPlanet, Start);
    NEW_NERVE(KoopaDemoJumpToPlanetNrvFall, KoopaDemoJumpToPlanet, Fall);
}

KoopaDemoJumpToPlanet::KoopaDemoJumpToPlanet(Koopa* pKoopa) : NerveExecutor("Demo[惑星までジャンプ]"), mParent(pKoopa) {}

void KoopaDemoJumpToPlanet::init() {
    KoopaFunction::initKoopaCamera(mParent, "ウェイト（惑星までジャンプ）");
    KoopaFunction::initKoopaCamera(mParent, "落下（惑星までジャンプ）");
    KoopaFunction::initKoopaCamera(mParent, "ワープ後（惑星までジャンプ）");
    initNerve(&NrvKoopaDemoJumpToPlanet::KoopaDemoJumpToPlanetNrvStart::sInstance);
}

void KoopaDemoJumpToPlanet::kill() {
    _8 = true;
    KoopaFunction::endKoopaCamera(mParent, "ウェイト（惑星までジャンプ）", false, -1);
    KoopaFunction::endKoopaCamera(mParent, "落下（惑星までジャンプ）", false, -1);
    KoopaFunction::endKoopaCamera(mParent, "ワープ後（惑星までジャンプ）", false, -1);
}

void KoopaDemoJumpToPlanet::startReady() {
    MR::onSwitchB(mParent);
}

void KoopaDemoJumpToPlanet::exeStart() {
    if(MR::isFirstStep(this)) {
        return;
    }
    KoopaFunction::setKoopaPos(mParent, "戦闘開始（クッパ）");
    MR::setPlayerPosAndWait("戦闘開始（マリオ）");
    KoopaFunction::startKoopaCamera(mParent, "落下（惑星までジャンプ）");
    MR::startAction(mParent, "JumpSoon");
    KoopaFunction::startFaceCtrl(mParent);
    setNerve(&NrvKoopaDemoJumpToPlanet::KoopaDemoJumpToPlanetNrvFall::sInstance);
}

void KoopaDemoJumpToPlanet::exeFall() {

}
