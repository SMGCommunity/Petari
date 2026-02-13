#include "Game/Boss/DinoPackunStateFire.hpp"
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

void DinoPackunStateFire::exeFire() {
    return;
}

DinoPackunStateFire::~DinoPackunStateFire() {
}
