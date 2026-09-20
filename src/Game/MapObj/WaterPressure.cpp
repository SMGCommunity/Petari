#include "Game/MapObj/WaterPressure.hpp"
#include "Game/MapObj/WaterPressureBullet.hpp"
#include "Game/MapObj/WaterPressureBulletHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

namespace {
    const f32 sGunPointOffset = 200.0f;
}

WaterPressure::WaterPressure(const char* pName) : PressureBase(pName), mCamInfo(), mDisablePopping() {
}

void WaterPressure::init(const JMapInfoIter& rIter) {
    PressureBase::init(rIter);
    MR::initActorCamera(this, rIter, &mCamInfo);
    MR::getJMapInfoArg7NoInit(rIter, &mDisablePopping);
}

void WaterPressure::initBullet(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_WaterPressureBulletHolder);
}

bool WaterPressure::shotBullet(f32 f) {
    WaterPressureBullet* pBullet = MR::getSceneObj< WaterPressureBulletHolder >(SceneObj_WaterPressureBulletHolder)->callEmptyBullet();
    if (pBullet == nullptr) {
        return false;
    }
    TPos3f point;
    calcGunPointFromCannon(&point);
    pBullet->shotWaterBullet(this, point, f, !isShotTypeOnGravity(), false, mDisablePopping, &mCamInfo);
    return true;
}

void WaterPressure::calcGunPointFromCannon(TPos3f* pPos) {
    TPos3f jointMtx = MR::getJointMtx(this, "Cannon1");
    TVec3f vec1, vec2;
    jointMtx.getXDir(vec1);
    jointMtx.getTrans(vec2);
    vec2.scaleAdd(::sGunPointOffset, vec1, vec2);
    pPos->set(jointMtx);
    TVec3f vec3, vec4;
    jointMtx.getYDir(vec3);
    jointMtx.getZDir(vec4);
    MR::makeMtxFrontUpPos(pPos, vec1, vec3, vec2);
}
