#include "Game/Camera/CamKarikariEffector.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"

namespace {
    static const f32 sKarikariViewRate = 0.8f;
    static const s32 sKarikariCounterMax = 30;
    static const f32 sPlayerRadius = 75.0f;
};  // namespace

void CamKarikariEffector_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    f32 f3 = JGeometry::TUtil< f32 >::epsilon();
    (void)0.5f;
    f32 f5 = MR::pi();
}

CamKarikariEffector::CamKarikariEffector() : mCounter(0) {
}

void CamKarikariEffector::update(CameraMan* pCameraMan) {
    // FIXME: a few minor issues with float and register scheduling
    // https://decomp.me/scratch/7CCpm

    if (MR::isPlayerDead()) {
        CameraLocalUtil::setFovy(pCameraMan, CameraLocalUtil::getFovy(pCameraMan));
        return;
    }

    if (MR::getKarikariClingNum() == 0) {
        if (mCounter > 0) {
            mCounter--;
        }
    } else {
        if (mCounter < ::sKarikariCounterMax) {
            mCounter++;
        }
    }

    // FIXME: t stored in f0 but should be in f31
    f32 t = MR::getClingNumMax() == 0 ? ::sKarikariViewRate : static_cast< f32 >(MR::getKarikariClingNum()) / MR::getClingNumMax();

    if (mCounter <= 0) {
        return;
    }

    f32 fovRate = 1.0f - (1.0f - t) * (1.0f - t);

    TVec3f diffWatchPos = CameraLocalUtil::getWatchPos(pCameraMan) - CameraLocalUtil::getPos(pCameraMan);
    TVec3f toWatchPos(diffWatchPos);
    MR::normalize(&toWatchPos);

    // FIXME: register scheduling issue, result of operator * should be preloaded before the actual mult?
    TVec3f playerUp;
    MR::getPlayerUpVec(&playerUp);
    TVec3f playerFocusPos = *MR::getPlayerPos() + playerUp * ::sPlayerRadius;

    TVec3f diffPlayerPos = playerFocusPos - CameraLocalUtil::getPos(pCameraMan);
    TVec3f toPlayerPos(diffPlayerPos);
    MR::normalize(&toPlayerPos);

    // Blend WatchPos to player focus pos by rotation (ease in)
    f32 rotRatio = MR::clamp((1.0f - MR::cos(mCounter * MR::pi() / ::sKarikariCounterMax)) * 0.5f, 0.0f, 1.0f);
    TQuat4f rot;
    rot.setRotate(toWatchPos, toPlayerPos, rotRatio);
    rot.transform(diffWatchPos);

    CameraLocalUtil::setPos(pCameraMan, CameraLocalUtil::getPos(pCameraMan));
    CameraLocalUtil::setWatchPos(pCameraMan, diffWatchPos + CameraLocalUtil::getPos(pCameraMan));

    TVec3f camUp(CameraLocalUtil::getUpVec(pCameraMan));
    rot.transform(camUp);
    CameraLocalUtil::setUpVec(pCameraMan, camUp);

    // Blend FovY
    f32 dist = diffPlayerPos.length();
    f32 fovAngle = MR::asin(75.0f / dist);

    f32 fovy = MR::atan2((dist * MR::tan(fovAngle)) / 0.3f, dist);

    if (fovy < CameraLocalUtil::getFovy(pCameraMan) * MR::pi() / 180.0f) {
        CameraLocalUtil::setFovy(pCameraMan, (fovy * 180.0f * fovRate) / MR::pi() + (1.0f - fovRate) * CameraLocalUtil::getFovy(pCameraMan));
    }
}
