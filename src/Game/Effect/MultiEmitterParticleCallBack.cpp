#include "Game/Effect/MultiEmitterParticleCallBack.hpp"

MultiEmitterParticleCallBack::MultiEmitterParticleCallBack() {
}

void MultiEmitterParticleCallBack_FORCE_MATCH(MultiEmitterParticleCallBack* pCallBack) {
    pCallBack->MultiEmitterParticleCallBack::~MultiEmitterParticleCallBack();
}
