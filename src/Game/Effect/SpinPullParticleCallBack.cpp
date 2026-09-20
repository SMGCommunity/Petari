#include "Game/Effect/SpinPullParticleCallBack.hpp"

#include "Game/Util/PlayerUtil.hpp"
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>

SpinPullParticleCallBack::SpinPullParticleCallBack() : MultiEmitterParticleCallBack() {
    _4 = 1.0f;
}

void SpinPullParticleCallBack::execute(JPABaseEmitter* pEmitter, JPABaseParticle* pParticle) {
    TVec3f baseParticlePosition;
    baseParticlePosition.set< f32 >(pParticle->mPosition);
    TVec3f pullvel;
    MR::calcPlayerSpinPullVelocity(&pullvel, baseParticlePosition);
    TVec3f baseParticleOffsetPosition;
    baseParticleOffsetPosition.set< f32 >(pParticle->mOffsetPosition);

    f32 time = pParticle->mTime;
    pParticle->mOffsetPosition.set< f32 >(baseParticleOffsetPosition + (((pullvel * _4) * time) * (time)));
}

void MerameraParticleCallBack::execute(JPABaseEmitter* pEmitter, JPABaseParticle* pParticle) {
    TVec3f baseParticlePosition;
    baseParticlePosition.set< f32 >(pParticle->mPosition);
    TVec3f pullvel;
    MR::calcPlayerSpinPullVelocity(&pullvel, baseParticlePosition);
    TVec3f baseParticleOffsetPosition;
    baseParticleOffsetPosition.set< f32 >(pParticle->mOffsetPosition);
    pParticle->mOffsetPosition.set< f32 >(baseParticleOffsetPosition + (pullvel * _4));
}