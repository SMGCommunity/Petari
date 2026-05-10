#include "Game/Camera/CamKarikariEffector.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

namespace {
    static const f32 sKarikariViewRate = 0.8f;
    static const s32 sKarikariCounterMax = 30;
    static const f32 sPlayerRadius = 75.0f;
}  // namespace

// TODO: replace call with MR::clamp01
inline f32 clamp01(f32 val) {
    if (val < 0.0f) {
        return 0.0f;
    }
    if (val > 1.0f) {
        return 1.0f;
    }
    return val;
}

// TODO: replace with proper function
inline f32 toRadian(f32 degree) {
    f32 pi = PI;
    return degree * pi / 180.0f;
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
        if (mCounter < sKarikariCounterMax) {
            mCounter++;
        }
    }

    // FIXME: t stored in f0 but should be in f31
    f32 t = MR::getClingNumMax() == 0 ? sKarikariViewRate : static_cast< f32 >(MR::getKarikariClingNum()) / MR::getClingNumMax();

    if (mCounter <= 0) {
        return;
    }

    f32 fovRate = 1.0f - (1.0f - t) * (1.0f - t);

    TVec3f diffWatchPos = CameraLocalUtil::getWatchPos(pCameraMan) - CameraLocalUtil::getPos(pCameraMan);
    TVec3f toWatchPos(diffWatchPos);
    MR::normalize(&toWatchPos);

    // FIXME: register scheduling issue, result of multInLine should be preloaded before the actual mult?
    TVec3f playerUp;
    MR::getPlayerUpVec(&playerUp);
    TVec3f playerFocusPos = *MR::getPlayerPos() + playerUp.multInLine(sPlayerRadius);

    TVec3f diffPlayerPos = playerFocusPos - CameraLocalUtil::getPos(pCameraMan);
    TVec3f toPlayerPos(diffPlayerPos);
    MR::normalize(&toPlayerPos);

    // Blend WatchPos to player focus pos by rotation (ease in)

    // FIXME: PI mult order wrong
    // TODO: this is likely MR::clamp(val, 0.0f, 1.0f), or MR::clamp01(val) instead of MR::clamp01(ptr)
    f32 rotRatio = clamp01((1.0f - JMACosRadian(mCounter * PI / sKarikariCounterMax)) * 0.5f);
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
    f32 fovAngle = JMAAsinRadian(75.0f / dist);

    f32 s = JMASinRadian(fovAngle);
    f32 c = JMACosRadian(fovAngle);
    f32 fovy = JMath::sAtanTable.atan2_((dist * (s / c)) / 0.3f, dist);

    if (fovy < toRadian(CameraLocalUtil::getFovy(pCameraMan))) {
        CameraLocalUtil::setFovy(pCameraMan, (fovy * 180.0f * fovRate) / PI + (1.0f - fovRate) * CameraLocalUtil::getFovy(pCameraMan));
    }
}
