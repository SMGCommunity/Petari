#include "Game/Boss/DinoPackunStateAwake.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunTail.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvDinoPackunStateAwake {
    NEW_NERVE(DinoPackunStateAwakeNrvFind, DinoPackunStateAwake, Find);
};

DinoPackunStateAwake::DinoPackunStateAwake(DinoPackun* pHost) : ActorStateBase< DinoPackun >("ディノパックン気付く", pHost) {
    initNerve(&NrvDinoPackunStateAwake::DinoPackunStateAwakeNrvFind::sInstance);
}

void DinoPackunStateAwake::appear() {
    mIsDead = false;
    setNerve(&NrvDinoPackunStateAwake::DinoPackunStateAwakeNrvFind::sInstance);
}

void DinoPackunStateAwake::kill() {
    mIsDead = true;
    getHost()->mTail->_18 = 0.0f;
}

void DinoPackunStateAwake::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Find", nullptr);
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_FIND", -1, -1);
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    f32 rate = MR::calcNerveEaseOutRate(this, 30);
    getHost()->mTail->_18 = 0.015f * rate;
    MR::turnDirectionToPlayerDegree(getHost(), &getHost()->_E8, 4.0f);
    getHost()->updateNormalVelocity();

    if (MR::isBckStopped(getHost())) {
        update();
    }
}

DinoPackunStateAwake::~DinoPackunStateAwake() {
}
