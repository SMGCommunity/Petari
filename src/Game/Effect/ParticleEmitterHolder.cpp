#include "Game/Effect/ParticleEmitterHolder.hpp"
#include "Game/Effect/EffectSystem.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/ParticleEmitter.hpp"

#include <JSystem/JParticle/JPAEmitter.hpp>
#include <algorithm>

ParticleEmitterHolder::ParticleEmitterHolder(const EffectSystem* pEffectSystem, int numEmitters) : mEffectSystem(pEffectSystem) {
    mEmitters.init(numEmitters);
}

void ParticleEmitterHolder::update(bool param1) {
    for (ParticleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        JPABaseEmitter* pBaseEmitter = pEmitter->mEmitter;
        if (pBaseEmitter == nullptr) {
            continue;
        }

        if (param1 != (pBaseEmitter->getGroupID() == 1)) {
            continue;
        }

        const bool shouldDelete = pBaseEmitter->checkStatus(0x8) && pBaseEmitter->getParticleNumber() == 0;

        if (shouldDelete) {
            mEffectSystem->forceDeleteEmitter(pEmitter);
        } else if (!pEmitter->mStopped) {
            if (MR::Effect::getLinkSingleEmitter(pBaseEmitter)) {
                pBaseEmitter->getEmitterCallBackPtr()->init(pBaseEmitter);
            }
            pEmitter->mStopped = true;
        }
    }
}

void ParticleEmitterHolder::forceDeleteAllOneTimeEmitters() {
    for (ParticleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (pEmitter->mEmitter == nullptr) {
            continue;
        }

        if (pEmitter->isContinuousParticle()) {
            continue;
        }

        mEffectSystem->forceDeleteEmitter(pEmitter);
    }
}

void ParticleEmitterHolder::forceDeleteAllEmitters() {
    for (ParticleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        mEffectSystem->forceDeleteEmitter(pEmitter);
    }
}

void ParticleEmitterHolder::requestMovementOnAllEmitters() {
    for (ParticleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        JPABaseEmitter* pBaseEmitter = pEmitter->mEmitter;
        if (pBaseEmitter == nullptr) {
            continue;
        }

        if (pBaseEmitter->getGroupID() == 1 || pBaseEmitter->getGroupID() == 7) {
            continue;
        }

        pEmitter->pauseOff();
    }
}

ParticleEmitter* ParticleEmitterHolder::findAvailableParticleEmitter() {
    ParticleEmitter* res = std::find_if_array(mEmitters.begin(), mEmitters.end(), std::not1(std::mem_func(&ParticleEmitter::isValid)));
    if (res == mEmitters.end()) {
        return nullptr;
    }
    return res;
}

void ParticleEmitterHolder::requestMovementOffAllLoopEmitters() {
    for (ParticleEmitter* pEmitter = mEmitters.begin(); pEmitter != mEmitters.end(); pEmitter++) {
        if (pEmitter->mEmitter == nullptr) {
            continue;
        }

        if (!pEmitter->isContinuousParticle()) {
            continue;
        }

        pEmitter->pauseOn();
    }
}
