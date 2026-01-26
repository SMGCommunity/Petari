#include "Game/Effect/ParticleEmitter.hpp"
#include <JSystem/JParticle/JPAEmitter.hpp>

ParticleEmitter::ParticleEmitter() : mEmitter(nullptr), mPaused(false), mStopped(false) {}

void ParticleEmitter::invalidate() {
    mEmitter = nullptr;
}

void ParticleEmitter::init(u16 unused) {
    mPaused = false;
    mStopped = false;
    mEmitter->mStatus |= JPAEmtrStts_Immortal;
}

void ParticleEmitter::pauseOn() {
    bool paused = mEmitter && mEmitter->mStatus & JPAEmtrStts_StopCalc;

    if (paused) {
        return;
    }

    mEmitter->mStatus |= JPAEmtrStts_StopCalc;
    mPaused = true;
}

void ParticleEmitter::pauseOff() {
    if (mPaused) {
        mEmitter->mStatus &= ~JPAEmtrStts_StopCalc;
        mPaused = false;
    }
}
