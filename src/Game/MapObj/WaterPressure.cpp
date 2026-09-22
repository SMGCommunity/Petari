#include "Game/MapObj/WaterPressure.hpp"
#include "Game/MapObj/WaterPressureBullet.hpp"
#include "Game/MapObj/WaterPressureBulletHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

namespace {
    static const f32 sGunPointOffset = 200.0f;
};  // namespace

WaterPressure::WaterPressure(const char* pName) : PressureBase(pName), mCameraInfo(), mIsInvalidSpinKill() {
}

void WaterPressure::init(const JMapInfoIter& rIter) {
    PressureBase::init(rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    MR::getJMapInfoArg7NoInit(rIter, &mIsInvalidSpinKill);
}

void WaterPressure::initBullet(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_WaterPressureBulletHolder);
}

bool WaterPressure::shotBullet(f32 speed) {
    WaterPressureBullet* pBullet = MR::getSceneObj< WaterPressureBulletHolder >(SceneObj_WaterPressureBulletHolder)->callEmptyBullet();
    if (pBullet == nullptr) {
        return false;
    }
    TPos3f point;
    calcGunPointFromCannon(&point);
    pBullet->shotWaterBullet(this, point, speed, !isShotTypeOnGravity(), false, mIsInvalidSpinKill, &mCameraInfo);
    return true;
}

void WaterPressure::calcGunPointFromCannon(TPos3f* pMtx) {
    TPos3f jointMtx = MR::getJointMtx(this, "Cannon1");
    TVec3f sideVec, pos;
    jointMtx.getXDir(sideVec);
    jointMtx.getTrans(pos);
    pos.scaleAdd(::sGunPointOffset, sideVec, pos);
    pMtx->set(jointMtx);
    TVec3f upVec, frontVec;
    jointMtx.getYDir(upVec);
    jointMtx.getZDir(frontVec);
    MR::makeMtxFrontUpPos(pMtx, sideVec, upVec, pos);
}
