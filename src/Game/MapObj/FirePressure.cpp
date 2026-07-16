#include "Game/MapObj/FirePressure.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/FirePressureBullet.hpp"
#include "Game/MapObj/FirePressureBulletHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include <JSystem/JMath/JMath.hpp>

FirePressure::~FirePressure() {
}

FirePressure::FirePressure(const char* pName) : PressureBase(pName) {
    mDisableCollsion = false;
}

void FirePressure::init(const JMapInfoIter& rIter) {
    PressureBase::init(rIter);
    MR::getJMapInfoArg7NoInit(rIter, &mDisableCollsion);
}

void FirePressure::initBullet(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_FirePressureBulletHolder);
}

bool FirePressure::shotBullet(f32 a1) {
    FirePressureBulletHolder* hldr = MR::getSceneObj< FirePressureBulletHolder >(SceneObj_FirePressureBulletHolder);
    FirePressureBullet* bullet = hldr->callEmptyBullet();

    // do we have an empty bullet slot available?
    if (bullet == nullptr) {
        return false;
    } else {
        TPos3f gunPoint;
        calcGunPointFromCannon(&gunPoint);
        bullet->shotFireBullet(this, gunPoint, a1, isShotTypeFollow(), mDisableCollsion);
        return true;
    }
}

void FirePressure::calcGunPointFromCannon(TPos3f* pOutPoint) {
    TPos3f jointMtx(MR::getJointMtx(this, "Cannon1"));
    TVec3f stack_2C;
    jointMtx.getXDir(stack_2C);
    TVec3f stack_20;
    jointMtx.getTrans(stack_20);
    stack_20.scaleAdd(200.0f, stack_2C, stack_20);
    pOutPoint->setInline(jointMtx);
    TVec3f stack_14;
    jointMtx.getYDir(stack_14);
    TVec3f stack_8;
    jointMtx.getZDir(stack_8);
    MR::makeMtxFrontUpPos(pOutPoint, stack_2C, stack_14, stack_20);
}
