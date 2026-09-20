#include "Game/Boss/DinoPackunStateAwake.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/Boss/DinoPackunTail.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvDinoPackunStateAwake {
    NEW_NERVE(DinoPackunStateAwakeNrvFind, DinoPackunStateAwake, Find);
};  // namespace NrvDinoPackunStateAwake

DinoPackunStateAwake::DinoPackunStateAwake(DinoPackun* pHost) : ActorStateBase< DinoPackun >("ディノパックン気付く", pHost) {
    initNerve(GET_NERVE(DinoPackunStateAwake, DinoPackunStateAwakeNrvFind));
}

void DinoPackunStateAwake::appear() {
    mIsDead = false;
    setNerve(GET_NERVE(DinoPackunStateAwake, DinoPackunStateAwakeNrvFind));
}

void DinoPackunStateAwake::kill() {
    mIsDead = true;
    getHost()->mTail->_18 = 0.0f;
}

void DinoPackunStateAwake::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startBck(getHost(), "Find");
        MR::startSound(getHost(), "SE_BV_D_PAKKUN_FIND");
        MR::startSound(getHost(), "SE_BM_D_PAKKUN_SLAVER");
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
