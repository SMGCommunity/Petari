#include "Game/Camera/CamHeliEffector.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Player/MarioActor.hpp"  // DO NOT REMOVE
#include "Game/Util/MathUtil.hpp"

void CamHeliEffector_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.0000038146973f;
    (void)0.5f;
    (void)0.98f;
    (void)0.001f;
    (void)60.0f;
    (void)(MR::pi() / 6.0f);
}

CamHeliEffector::CamHeliEffector() : _0(), _4() {
}

void CamHeliEffector::update(CameraMan* pCamera) {
    if (!CameraLocalUtil::getTarget(pCamera)->isCameraStateOn(1)) {
        _0 = 0;
        if (_4 == 0.0f) {
            return;
        }

        f32 val = _4;
        val *= 0.98f;
        _4 = val;
        if (val < 0.001f) {
            _4 = 0.0f;
        }
    } else {
        f32 val = _0 / 60.0f;
        if (val > 1.0f) {
            val = 1.0f;
        }

        val *= val;
        val *= val;
        _4 += (1.0f - _4) * val;
        _0++;
    }

    TVec3f up = CameraLocalUtil::getTarget(pCamera)->getUpVec();
    TVec3f diff = CameraLocalUtil::getPos(pCamera) - CameraLocalUtil::getWatchPos(pCamera);

    if (MR::isNearZero(diff)) {
        return;
    }

    TVec3f direction = diff;
    MR::normalize(&direction);

    f32 angle = direction.angle(up);
    if (angle < MR::pi() / 6.0f) {
        return;
    }

    TQuat4f quat;
    const f32 rate = (angle - MR::pi() / 6.0f) / angle;
    quat.setRotate(direction, up, rate * _4);
    quat.transform(diff);

    CameraLocalUtil::setPos(pCamera, diff + CameraLocalUtil::getWatchPos(pCamera));

    TVec3f newUp(CameraLocalUtil::getUpVec(pCamera));
    quat.transform(newUp);
    CameraLocalUtil::setUpVec(pCamera, newUp);
}
