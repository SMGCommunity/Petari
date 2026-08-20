#include "Game/MapObj/BigFanHolder.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BigFan.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sMaxBigFan = 32;

    BigFanHolder* getBigFanHolder() {
        return MR::getSceneObj< BigFanHolder >(SceneObj_BigFanHolder);
    }
};  // namespace

BigFanHolder::BigFanHolder() : DeriveActorGroup("大型扇風機管理", ::sMaxBigFan) {
}

void BigFanHolder::calcWindInfo(TVec3f* pWindInfo, const TVec3f& rPos, f32* pWindSpeed) {
    s32 count = mObjectCount;
    TVec3f stack_2C;
    stack_2C.zero();

    for (int i = 0; i < count; i++) {
        TVec3f windVec;
        windVec.zero();
        BigFan* fan = static_cast< BigFan* >(getActor(i));
        fan->calcWindInfo(&windVec, rPos);
        f32 speed = static_cast< BigFan* >(getActor(i))->mWindSpeed;
        stack_2C.add(windVec * (speed / 100.0f));
    }

    f32 mag = stack_2C.length();
    if (MR::isNearZero(mag)) {
        pWindInfo->zero();

        if (pWindSpeed != nullptr) {
            *pWindSpeed = 0.0f;
        }
    } else {
        pWindInfo->set(stack_2C * (1.0f / mag));

        if (pWindSpeed != nullptr) {
            *pWindSpeed = mag;
        }
    }
}

void BigFanFunction::createBigFanHolder() {
    MR::createSceneObj(SceneObj_BigFanHolder);
}

void BigFanFunction::calcWindInfo(TVec3f* pWindInfo, const TVec3f& rPos, f32* pWindSpeed) {
    if (!MR::isExistSceneObj(SceneObj_BigFanHolder)) {
        pWindInfo->zero();

        if (pWindSpeed != nullptr) {
            *pWindSpeed = 0.0f;
        }
    } else {
        ::getBigFanHolder()->calcWindInfo(pWindInfo, rPos, pWindSpeed);
    }
}

void BigFanFunction::registerBigFan(BigFan* pFan) {
    ::getBigFanHolder()->registerActor(pFan);
}
