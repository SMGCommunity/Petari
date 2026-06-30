#include "Game/Effect/EffectObjGravityDust.hpp"
#include "Game/Map/GravityDust.hpp"
#include "Game/Util/EffectUtil.hpp"

void EffectObjGravityDust_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

EffectObjGravityDust::EffectObjGravityDust(const char* pName) : SimpleEffectObj(pName), _9C(nullptr), _A0(1.0f) {
}

void EffectObjGravityDust::init(const JMapInfoIter& rIter) {
    SimpleEffectObj::init(rIter);
    _9C = new GravityDustParticleCallBack(this);
    MR::emitEffectWithParticleCallBack(this, _8C, _9C);
    MR::setEffectLocalScale(this, _8C, mScale);
    _A0 = (mScale.x + mScale.y + mScale.z) / 3.0f;
}

void EffectObjGravityDust::endClipped() {
    SimpleEffectObj::endClipped();
    MR::emitEffectWithParticleCallBack(this, _8C, _9C);
}

f32 EffectObjGravityDust::getClippingRadius() const {
    return mScale.length() * 500.0f;
}

TVec3f* EffectObjGravityDust::getClippingCenterOffset() const {
    return &(TVec3f(0.0f, 500.0f * mScale.y, 0.0f));
}

EffectObjGravityDust::~EffectObjGravityDust() {
}
