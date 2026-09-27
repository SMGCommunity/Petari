#include "Game/Camera/CamKarikariEffector.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <JSystem/JGeometry/TUtil.hpp>

namespace {
    static const f32 sKarikariViewRate = 0.8f;
    static const s32 sKarikariCounterMax = 30;
    static const f32 sPlayerRadius = 75.0f;
};  // namespace

void CamKarikariEffector_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    f32 f3 = MR::epsilon();
    (void)0.5f;
    f32 f5 = MR::pi();
}

CamKarikariEffector::CamKarikariEffector() : mCounter(0) {
}

namespace {
    inline TVec3f calcPlayerFocusPos(const TVec3f& rOffset) {
        const TVec3f* pOffset = &rOffset;

        return *MR::getPlayerPos() + *pOffset;
    }

    inline f32 calcFovy(f32 dist) {
        f32 fovAngle = MR::asin(75.0f / dist);

        return MR::atan2((dist * MR::tan(fovAngle)) / 0.3f, dist);
    }
}

#pragma push
#pragma global_optimizer off
void CamKarikariEffector::update(CameraMan* pCameraMan) {
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

    f32 t;

    if (MR::getClingNumMax() == 0) {
        t = ::sKarikariViewRate;
    } else {
        t = static_cast< f32 >(MR::getKarikariClingNum()) / MR::getClingNumMax();
    }

    if (mCounter <= 0) {
        return;
    }

    t = 1.0f - t;
    t *= t;
    t = 1.0f - t;

    TVec3f diffWatchPos = CameraLocalUtil::getWatchPos(pCameraMan) - CameraLocalUtil::getPos(pCameraMan);
    TVec3f toWatchPos(diffWatchPos);
    MR::normalize(&toWatchPos);

    TVec3f playerUp;
    MR::getPlayerUpVec(&playerUp);
    TVec3f playerFocusPos = calcPlayerFocusPos(playerUp * ::sPlayerRadius);

    TVec3f diffPlayerPos = playerFocusPos - CameraLocalUtil::getPos(pCameraMan);
    TVec3f toPlayerPos(diffPlayerPos);
    MR::normalize(&toPlayerPos);

    f32 rotRatio = MR::clamp((1.0f - MR::cos(mCounter * MR::pi() / ::sKarikariCounterMax)) * 0.5f, 0.0f, 1.0f);
    TQuat4f rot;
    rot.setRotate(toWatchPos, toPlayerPos, rotRatio);
    rot.transform(diffWatchPos);

    CameraLocalUtil::setPos(pCameraMan, CameraLocalUtil::getPos(pCameraMan));
    CameraLocalUtil::setWatchPos(pCameraMan, diffWatchPos + CameraLocalUtil::getPos(pCameraMan));

    TVec3f camUp(CameraLocalUtil::getUpVec(pCameraMan));
    rot.transform(camUp);
    CameraLocalUtil::setUpVec(pCameraMan, camUp);

    f32 fovy = calcFovy(diffPlayerPos.length());

    if (fovy < CameraLocalUtil::getFovy(pCameraMan) * MR::pi() / 180.0f) {
        CameraLocalUtil::setFovy(pCameraMan, (fovy * 180.0f * t) / MR::pi() + (1.0f - t) * CameraLocalUtil::getFovy(pCameraMan));
    }
}
#pragma pop
