#include "Game/Boss/BossKameck.hpp"

void BossKameck::updatePose() {
    TVec3f v17(mGravity);
    TVec3f v19;
    v19.set(v17);
    TVec3f v16(mVelocity);
    v16.sub(_C8);
    f32 dot = mGravity.dot(v16);
    TVec3f v18;
    JMAVECScaleAdd(mGravity.toCVec(), v16.toCVec(), v18.toVec(), -dot);
    f32 mag = PSVECMag(v18.toCVec());

    if (!MR::isNearZero(mag, 0.001f)) {
        f32 v4 = MR::normalize(mag, 0.0f, 3.0f);
        TVec3f v13(v18);

        TVec3f stack_44;
        stack_44.addInline(v13);

        if (MR::isNearZero(stack_44, 0.001f)) {
            TVec3f v12 = -mGravity;
            v19.set(v12);
        }
        else {
            MR::normalize(&v19);
        }
    }

    MR::blendQuatUpFront(&_90, v19, _A0, 0.039999999f, 0.2f);
}