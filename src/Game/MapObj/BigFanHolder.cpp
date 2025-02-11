#include "Game/MapObj/BigFanHolder.hpp"
#include "Game/MapObj/BigFan.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

BigFanHolder::BigFanHolder() : DeriveActorGroup("大型扇風機管理", 0x20) {

}

/*
void BigFanHolder::calcWindInfo(TVec3f *pWindInfo, const TVec3f &a2, f32 *a3) {
    s32 count = mObjectCount;
    TVec3f stack_2C;
    stack_2C.zero();

    for (int i = 0; i < count; i++) {
        TVec3f stack_20;
        stack_20.zero();
        BigFan* fan = reinterpret_cast<BigFan*>(getActor(i));
        fan->calcWindInfo(&stack_20, a2);
        f32 val = reinterpret_cast<BigFan*>(getActor(i))->_A0;
        TVec3f stack_14 = stack_20 * (val / 100.0f);
        stack_2C.addInline3(stack_14); 
    }

    f32 mag = PSVECMag(&stack_2C);
    if (MR::isNearZero(mag, 0.001f)) {
        pWindInfo->zero();  

        if (a3) {
            *a3 = 0.0f;
        }
    }
    else {
        TVec3f stack_8 = stack_2C * (1.0f / mag);
        pWindInfo->x = stack_8.x;
        pWindInfo->y = stack_8.y;
        pWindInfo->z = stack_8.z;

        if (a3) {
            *a3 = mag;
        }
    }
}
*/

void BigFanFunction::createBigFanHolder() {
    MR::createSceneObj(SceneObj_BigFanHolder);
}

void BigFanFunction::calcWindInfo(TVec3f *a1, const TVec3f &a2, f32 *a3) {
    if (!MR::isExistSceneObj(SceneObj_BigFanHolder)) {
        a1->zero();

        if (a3) {
            *a3 = 0.0f;
        }
    }
    else {
        reinterpret_cast<BigFanHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_BigFanHolder))->calcWindInfo(a1, a2, a3);
    }
}

void BigFanFunction::registerBigFan(BigFan *pFan) {
    reinterpret_cast<BigFanHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_BigFanHolder))->registerActor(pFan);
}

BigFanHolder::~BigFanHolder() {

}
