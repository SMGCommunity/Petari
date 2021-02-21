#include "Camera/CameraTargetHolder.h"

CameraTargetHolder::CameraTargetHolder()
    : mTarget(0)
{
    mTargetActor = new CameraTargetActor("アクター注目");
    mTargetPlayer = new CameraTargetPlayer("マリオ注目");
}