#include "Game/Effect/ParticleEmitter.hpp"
#include <JSystem/JParticle/JPAEmitter.hpp>

ParticleEmitter::ParticleEmitter() :
    mEmitter(nullptr),
    mPaused(false),
    mStopped(false)
{
    
}

void ParticleEmitter::invalidate() {
    mEmitter = nullptr;
}

void ParticleEmitter::init(u16 unused) {
    mPaused = false;
    mStopped = false;
    mEmitter->mFlag |= JPA_EMITTER_INIT_FLAG;
}

void ParticleEmitter::pauseOn() {
    bool paused = mEmitter && mEmitter->mFlag & JPA_EMITTER_PAUSE_FLAG;

    if (paused) {
        return;
    }

    mEmitter->mFlag |= JPA_EMITTER_PAUSE_FLAG;
    mPaused = true;
}

void ParticleEmitter::pauseOff() {
    if (mPaused) {
        mEmitter->mFlag &= ~JPA_EMITTER_PAUSE_FLAG;
        mPaused = false;
    }
}
