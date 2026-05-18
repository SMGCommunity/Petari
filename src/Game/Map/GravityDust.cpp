#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JParticle/JPAParticle.hpp"
#include "Game/Map/GravityDust.hpp"
#include "Game/Effect/MultiEmitterParticleCallBack.hpp"

GravityDustParticleCallBack::GravityDustParticleCallBack(const LiveActor* pActor) : MultiEmitterParticleCallBack(), mActor((LiveActor*)pActor) {
}

//https://decomp.me/scratch/hyC0Q
void GravityDustParticleCallBack::execute(JPABaseEmitter* pEmitter, JPABaseParticle* pParticle) {
    TVec3f offsetPosition;
    TVec3f baseAxis;
    TVec3f currentPosition;
    TVec3f gravityVector;

    if (pParticle->checkStatus(4) == 0) {
        offsetPosition.set(pParticle->mOffsetPosition);
        f32 scaleFac = *pParticle->field_0x78;
        baseAxis.set(pParticle->mBaseAxis);
        baseAxis.scale(scaleFac);
        TVec3f gravityVector(0.0f, 0.0f, 0.0f);
        currentPosition.set(pParticle->getCalcCurrentPositionX(pEmitter), pParticle->getCalcCurrentPositionY(pEmitter), pParticle->getCalcCurrentPositionZ(pEmitter));
        MR::calcGravityVectorOrZero(mActor, currentPosition, &gravityVector, nullptr, 0);

        if (!MR::isNearZero(gravityVector)) {
            gravityVector.scale(1.0f);
            baseAxis.scale(0.995f);
            baseAxis.add(gravityVector);
            offsetPosition.add(baseAxis);
            pParticle->mOffsetPosition.set(offsetPosition);
            f32 baseAxisLength = baseAxis.length();

            if (!MR::isNearZero(baseAxisLength)) {
                *pParticle->field_0x78 = baseAxisLength;
                if (!MR::isNearZero(baseAxis)) {
                    MR::normalize(&baseAxis);
                    pParticle->mBaseAxis.set(baseAxis);
                }
            }
        }
    }
}
