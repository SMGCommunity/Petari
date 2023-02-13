#include "Game/MapObj/FirePressure.h"
#include "Game/MapObj/FirePressureBulletHolder.h"
#include "Game/Scene/SceneObjHolder.h"
#include <JSystem/JMath/JMath.h>

FirePressure::~FirePressure() {

}

FirePressure::FirePressure(const char *pName) : PressureBase(pName) {
    mDisableCollsion = false;
}

void FirePressure::init(const JMapInfoIter &rIter) {
    PressureBase::init(rIter);
    MR::getJMapInfoArg7NoInit(rIter, &mDisableCollsion);
}

void FirePressure::initBullet(const JMapInfoIter &rIter) {
    MR::createSceneObj(SceneObj_FirePressureBulletHolder);
}

bool FirePressure::shotBullet(f32 a1) {
    FirePressureBulletHolder* hldr = MR::getSceneObj<FirePressureBulletHolder*>(SceneObj_FirePressureBulletHolder);
    FirePressureBullet* bullet = hldr->callEmptyBullet();

    // do we have an empty bullet slot available?
    if (bullet == nullptr) {
        return false;
    }
    else {
        TPos3f gunPoint;
        calcGunPointFromCannon(&gunPoint);
        bullet->shotFireBullet(this, gunPoint, a1, isShotTypeFollow(), mDisableCollsion);
        return true;
    }
}

void FirePressure::calcGunPointFromCannon(TPos3f *pOutPoint) {
    Mtx jointMtx;
    JMath::gekko_ps_copy12(jointMtx, MR::getJointMtx(this, "Cannon1"));
    TVec3f stack_2C;
    f32 z = jointMtx[2][0];
    f32 y = jointMtx[1][0];
    f32 x = jointMtx[0][0];
    stack_2C.set(x, y, z);
    TVec3f stack_20;
    z = jointMtx[2][3];
    y = jointMtx[1][3];
    x = jointMtx[0][3];
    stack_20.set(x, y, z);
    JMAVECScaleAdd(stack_2C.toCVec(), stack_20.toCVec(), stack_20.toVec(), 200.0f);
    JMath::gekko_ps_copy12(pOutPoint, jointMtx);
    TVec3f stack_14;
    z = jointMtx[2][1];
    y = jointMtx[1][1];
    x = jointMtx[0][1];
    stack_14.set(x, y, z);
    TVec3f stack_8;
    z = jointMtx[2][2];
    y = jointMtx[1][2];
    x = jointMtx[0][2];
    stack_8.set(x, y, z);
    MR::makeMtxFrontUpPos(pOutPoint, stack_2C, stack_14, stack_20);
}