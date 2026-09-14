#include "Game/Map/GravityDust.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace {
    const f32 sGravity = 1.0f;
    const f32 sFrictionRate = 0.995f;
}  // namespace

void GravityDust_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

GravityDustParticleCallBack::GravityDustParticleCallBack(const LiveActor* pActor) : MultiEmitterParticleCallBack(), mActor(pActor) {
}

void GravityDustParticleCallBack::execute(JPABaseEmitter* pEmitter, JPABaseParticle* pParticle) {
    TVec3f offsetPosition;
    TVec3f baseAxis;

    if (pParticle->checkStatus(4)) {
        return;
    }

    offsetPosition.set(pParticle->mOffsetPosition);
    u32 userWork = pParticle->getUserWork();
    f32 speed = reinterpret_cast< f32& >(userWork);
    baseAxis.set(pParticle->mBaseAxis);
    baseAxis.scale(speed);
    TVec3f gravityVector(0.0f, 0.0f, 0.0f);
    TVec3f currentPosition;
    currentPosition.set< f32 >(pParticle->getCalcCurrentPositionX(pEmitter), pParticle->getCalcCurrentPositionY(pEmitter),
                               pParticle->getCalcCurrentPositionZ(pEmitter));
    MR::calcGravityVectorOrZero(mActor, currentPosition, &gravityVector, nullptr, 0);

    if (MR::isNearZero(gravityVector)) {
        return;
    }

    gravityVector.scale(sGravity);
    baseAxis.scale(sFrictionRate);
    baseAxis.add(gravityVector);
    offsetPosition.add(baseAxis);
    pParticle->mOffsetPosition.set(offsetPosition);
    speed = baseAxis.length();

    if (MR::isNearZero(speed)) {
        return;
    }

    userWork = reinterpret_cast< u32& >(speed);
    pParticle->setUserWork(userWork);

    if (MR::isNearZero(baseAxis)) {
        return;
    }

    MR::normalize(&baseAxis);
    pParticle->mBaseAxis.set(baseAxis);
}

GravityDustParticleCallBack::~GravityDustParticleCallBack() {
}
