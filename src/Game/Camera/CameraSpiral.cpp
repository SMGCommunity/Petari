#include "Game/Camera/CameraSpiral.hpp"
#include "Game/Camera/CamTranslatorSpiral.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraSpiral_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

CameraSpiral::CameraSpiral(const char* pName)
    : Camera(pName), mEndTime(60), mTimer(), mStartTime(), mDistStart(1000.0), mDistEnd(1000.0f), mAngleStart(), mAngleEnd() {
}

void CameraSpiral::reset() {
    mTimer = 0;
}

CameraTargetObj* CameraSpiral::calc() {
    // FIXME: load order swaps and matrix register alloc instead of stack lookup
    // https://decomp.me/scratch/lv4im

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    f32 easeTime = mTimer < mStartTime ? 0.0f : mTimer - mStartTime;

    f32 rate;
    switch (mEaseType) {
    case EaseType_Linear: {
        f32 t = easeTime / getInterval();
        rate = t;
        break;
    }
    case EaseType_InOut: {
        f32 t = easeTime / getInterval();
        f32 t3 = t * t * t;
        f32 t4 = t3 * t;
        f32 t5 = t4 * t;
        rate = t5 * 6.0f - t4 * 15.0f + t3 * 10.0f;
        break;
    }
    }

    f32 invrate = 1.0f - rate;
    f32 offsetY = mOffsetYEnd * rate + mOffsetYStart * invrate;
    f32 angle = mAngleEnd * rate + mAngleStart * invrate;
    f32 dist = mDistEnd * rate + mDistStart * invrate;

    TVec3f spiralDir(MR::sin(angle), 0.0f, MR::cos(angle));
    TVec3f offset = spiralDir * dist + TVec3f(0.0f, offsetY, 0.0f);

    TPos3f mtx;
    mtx.identity();
    mtx.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
    mtx.setYDir(CameraLocalUtil::getTarget(this)->getUpVec());
    mtx.setZDir(CameraLocalUtil::getTarget(this)->getFrontVec());

    mtx.mult33(offset);
    mtx.mult33(spiralDir);

    TVec3f pos;
    pos.add(watchPoint, offset);
    MR::normalize(&offset);

    TQuat4f rot;
    rot.setRotate(spiralDir, offset);

    TVec3f up;
    rot.transform(CameraLocalUtil::getTarget(this)->getUpVec(), up);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());

    if (++mTimer > mEndTime) {
        mTimer = mEndTime;
    }

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraSpiral::createTranslator() {
    return new CamTranslatorSpiral(this);
}
