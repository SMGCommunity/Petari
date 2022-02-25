#include "Game/MapObj/BigFanHolder.h"
#include "Game/MapObj/BigFan.h"
#include "Game/Scene/SceneObjHolder.h"

BigFanHolder::BigFanHolder() : DeriveActorGroup("大型扇風機管理", 0x20) {

}

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