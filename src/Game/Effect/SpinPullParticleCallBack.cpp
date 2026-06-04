#include "Game/Effect/SpinPullParticleCallBack.hpp"

#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JParticle/JPAEmitter.hpp"
#include "JSystem/JParticle/JPAParticle.hpp"

SpinPullParticleCallBack::SpinPullParticleCallBack() : MultiEmitterParticleCallBack() {
    _4 = 1.0f;
}

void SpinPullParticleCallBack::execute(JPABaseEmitter* emitter, JPABaseParticle* baseparticle) {
    TVec3f baseParticlePosition;
    baseParticlePosition.set< f32 >(baseparticle->mPosition);
    TVec3f pullvel;
    MR::calcPlayerSpinPullVelocity(&pullvel, baseParticlePosition);
    TVec3f baseParticleOffsetPosition;
    baseParticleOffsetPosition.set< f32 >(baseparticle->mOffsetPosition);
    f32 time = baseparticle->mTime;
    baseparticle->mOffsetPosition.set< f32 >(baseParticleOffsetPosition + (((pullvel * emitter->mLocalScl.y) * time) * (time)));
}

void MerameraParticleCallBack::execute(JPABaseEmitter* emitter, JPABaseParticle* baseparticle) {
    TVec3f baseParticlePosition;
    baseParticlePosition.set< f32 >(baseparticle->mPosition);
    TVec3f pullvel;
    MR::calcPlayerSpinPullVelocity(&pullvel, baseParticlePosition);
    TVec3f baseParticleOffsetPosition;
    baseParticleOffsetPosition.set< f32 >(baseparticle->mOffsetPosition);
    // mTime
    // mLocalScl.y
    baseparticle->mOffsetPosition.set< f32 >(baseParticleOffsetPosition + (pullvel * emitter->mLocalScl.y));
}