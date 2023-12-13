#include "Game/Effect/ParticleEmitter.hpp"

ParticleEmitter::ParticleEmitter() {
    mManager = nullptr;
    mPaused = false;
    mStopped = false;
}

void ParticleEmitter::invalidate() {
    mManager = nullptr;
}

void ParticleEmitter::init(u16 unused) {
    mPaused = false;
    mStopped = false;
    mManager->flags |= JPA_EMITTER_INIT_FLAG;
}

void ParticleEmitter::pauseOn() {
    bool paused = false;
    if (mManager && mManager->flags & JPA_EMITTER_PAUSE_FLAG) {
        paused = true;
    }
    if (paused) {
        return;
    }
    mManager->flags |= JPA_EMITTER_PAUSE_FLAG;
    mPaused = true;
}

void ParticleEmitter::pauseOff() {
    if (mPaused) {
        mManager->flags &= ~JPA_EMITTER_PAUSE_FLAG;
        mPaused = false;
    }
}