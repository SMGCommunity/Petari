#include "Game/Effect/EffectSystem.hpp"
#include "Game/Effect/SingleEmitter.hpp"
#include "Game/Util/EffectUtil.hpp"

SingleEmitter::SingleEmitter() {
    mEmitter = nullptr;
    _4 = 0;
    mGroupId = 0;
    mSpawn = SINGLE_EMITTER_UNDEF_SPAWN;
}

void SingleEmitter::init(u16 unk) {
    _4 = unk;
}

void SingleEmitter::deleteEmitter() {
    if (isValid()) {
        MR::Effect::deleteParticleEmitter(mEmitter);
    }
}

void SingleEmitter::scanParticleEmitter(EffectSystem *pSystem) {
    ParticleEmitter* emitter;
    if (mSpawn == SINGLE_EMITTER_UNDEF_SPAWN) {
        emitter = pSystem->createEmitter(_4, mGroupId, 0);
        mSpawn = emitter->isContinuousParticle() ? SINGLE_EMITTER_CONTINUOUS_SPAWN : SINGLE_EMITTER_ONE_TIME_SPAWN;
        pSystem->forceDeleteEmitter(emitter);
    }
}

bool SingleEmitter::isOneTime() const {
    if (mEmitter) {
        return !mEmitter->isContinuousParticle();
    } else {
        return !mSpawn;
    }
}

bool SingleEmitter::isValid() const {
    if (mEmitter == nullptr) {
        return false;
    } else {
        return mEmitter->mManager;
    }
}

void SingleEmitter::link(ParticleEmitter *pEmitter) {
    mEmitter = pEmitter;
    MR::Effect::setLinkSingleEmitter(pEmitter, this);
}

void SingleEmitter::unlink() {
    MR::Effect::setLinkSingleEmitter(mEmitter, nullptr);
    mEmitter = nullptr;
}