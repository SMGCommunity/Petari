#include "Camera/CameraTargetHolder.h"
#include "revolution/mtx.h"

CameraTargetHolder::CameraTargetHolder()
    : mTarget(0)
{
    mTargetActor = new CameraTargetActor("アクター注目");
    mTargetPlayer = new CameraTargetPlayer("マリオ注目");
}

void CameraTargetHolder::movement()
{
    mTarget->movement();
}

CameraTargetObj* CameraTargetHolder::get()
{
    return mTarget;
}

void CameraTargetHolder::set(CameraTargetObj *pObj)
{
    mTarget = pObj;
}

void CameraTargetHolder::set(const LiveActor *pActor)
{
    mTargetActor->mActor = pActor;
    mTarget = mTargetActor;
}

bool CameraTargetHolder::isOnGround() const
{
    bool ret = false;

    if (!mTarget->isJumping() || mTarget->isWaterMode() || mTarget->isOnWaterSurface())
    {
        ret = true;
    }

    return ret;
}

bool CameraTargetHolder::isMoving() const
{
    Vec* vec = reinterpret_cast<Vec*>(&mTarget->getLastMove());

    return PSVECMag(vec) > 1.0f;
}

CameraTargetHolder::~CameraTargetHolder()
{
    
}