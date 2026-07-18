#include "Game/Map/GravityDust.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MathUtil.hpp"

GravityDustParticleCallBack::GravityDustParticleCallBack(const LiveActor* pActor) : MultiEmitterParticleCallBack(), mActor(pActor) {
}

// https://decomp.me/scratch/hyC0Q
void GravityDustParticleCallBack::execute(JPABaseEmitter* pEmitter, JPABaseParticle* pParticle) {
    TVec3f offsetPosition;
    TVec3f baseAxis;
    TVec3f currentPosition;

    if (pParticle->checkStatus(4)) {
        return;
    }

    offsetPosition.set(pParticle->mOffsetPosition);
    f32 scaleFac = *pParticle->field_0x78;
    baseAxis.set(pParticle->mBaseAxis);
    baseAxis.scale(scaleFac);
    TVec3f gravityVector(0.0f, 0.0f, 0.0f);
    currentPosition.set(pParticle->getCalcCurrentPositionX(pEmitter), pParticle->getCalcCurrentPositionY(pEmitter),
                        pParticle->getCalcCurrentPositionZ(pEmitter));
    MR::calcGravityVectorOrZero(mActor, currentPosition, &gravityVector, nullptr, 0);

    if (MR::isNearZero(gravityVector)) {
        return;
    }
    gravityVector.scale(1.0f);
    baseAxis.scale(0.995f);
    baseAxis.add(gravityVector);
    offsetPosition.add(baseAxis);
    pParticle->mOffsetPosition.set(offsetPosition);
    f32 baseAxisLength = baseAxis.length();

    if (MR::isNearZero(baseAxisLength)) {
        return;
    }

    *pParticle->field_0x78 = baseAxisLength;
    if (MR::isNearZero(baseAxis)) {
        return;
    }

    MR::normalize(&baseAxis);
    pParticle->mBaseAxis.set(baseAxis);
}
