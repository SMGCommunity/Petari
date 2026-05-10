#include "Game/Boss/DinoPackunStateFire.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunBall.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util.hpp"

namespace NrvDinoPackunStateFire {
    NEW_NERVE(DinoPackunStateFireNrvNormal, DinoPackunStateFire, Normal);
    NEW_NERVE(DinoPackunStateFireNrvFireSign, DinoPackunStateFire, FireSign);
    NEW_NERVE(DinoPackunStateFireNrvFire, DinoPackunStateFire, Fire);
    NEW_NERVE(DinoPackunStateFireNrvCool, DinoPackunStateFire, Cool);
};  // namespace NrvDinoPackunStateFire

DinoPackunStateFire::DinoPackunStateFire(DinoPackun* pBoss) : ActorStateBase< DinoPackun >("ディノパックンの炎状態管理", pBoss) {
}

void DinoPackunStateFire::init() {
    initNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvNormal::sInstance);
    appear();
}

void DinoPackunStateFire::appear() {
    mIsDead = false;
    setNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvNormal::sInstance);
}

void DinoPackunStateFire::exeNormal() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(getHost(), "Normal");
        MR::startBrk(getHost()->getBallModel(), "Normal");
        getHost()->mBall->setDamageNone();
    }
}

void DinoPackunStateFire::exeFireSign() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(getHost()->getBallModel(), "TailFire");
        getHost()->mBall->setDamageNone();
    }
}

void DinoPackunStateFire::exeCool() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(getHost(), "Cool");
        MR::startBrk(getHost()->getBallModel(), "Cool");
        MR::deleteEffect(getHost(), "HeadFire");
        MR::deleteEffect(getHost(), "BodyFire");
        MR::deleteEffect(getHost()->getBallModel(), "TailFire");
        MR::emitEffect(getHost()->getBallModel(), "TailFireMisFire");
        getHost()->mBall->setDamageNone();
    }

    if (MR::isBrkStopped(getHost())) {
        setNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvNormal::sInstance);
    }
}

void DinoPackunStateFire::requestFire() {
    if (!isFire()) {
        setFire();
        setNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvFire::sInstance);
    }
}

void DinoPackunStateFire::requestFireDirect() {
    setFireDirect();
    setNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvFire::sInstance);
}

void DinoPackunStateFire::requestFireSign() {
    if (isEnableFire()) {
        setNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvFireSign::sInstance);
    }
}

void DinoPackunStateFire::requestCool() {
    if (isEnableCool()) {
        MR::startSound(getHost()->getBallModel(), "SE_BM_D_PAKKUN_TAIL_FIRE_OFF", -1, -1);
        setNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvCool::sInstance);
    }
}

void DinoPackunStateFire::setFire() {
    MR::emitEffect(getHost()->getBallModel(), "TailFire");
    MR::emitEffect(getHost(), "HeadFire");
    MR::emitEffect(getHost(), "BodyFire");
    MR::startBrk(getHost(), "Awake");
    MR::startBrk(getHost()->getBallModel(), "Heat");
    MR::startSound(getHost()->getBallModel(), "SE_BM_D_PAKKUN_TAIL_FIRE_ON", -1, -1);
    getHost()->mBall->setDamageFire();
}

void DinoPackunStateFire::setFireDirect() {
    setFire();
    MR::setBrkFrameEndAndStop(getHost());
    MR::setBrkFrameEndAndStop(getHost()->getBallModel());
}

bool DinoPackunStateFire::isFire() const {
    return isNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvFire::sInstance);
}

bool DinoPackunStateFire::isEnableFire() const {
    if (isNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvNormal::sInstance) ||
        isNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvCool::sInstance)) {
        return true;
    }

    return false;
}

bool DinoPackunStateFire::isEnableCool() const {
    if (isNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvFireSign::sInstance) ||
        isNerve(&NrvDinoPackunStateFire::DinoPackunStateFireNrvFire::sInstance)) {
        return true;
    }

    return false;
}

void DinoPackunStateFire::exeFire() {
    return;
}

DinoPackunStateFire::~DinoPackunStateFire() {
}
