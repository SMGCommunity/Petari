#include "Game/Camera/CameraTargetHolder.h"
#include "Game/Camera/CameraTargetObj.h"
#include "Game/LiveActor/LiveActor.h"

CameraTargetHolder::CameraTargetHolder() {
    mTarget = NULL;
    mTargetActor = new CameraTargetActor("アクター注目");
    mTargetPlayer = new CameraTargetPlayer("マリオ注目");
}

CameraTargetHolder::~CameraTargetHolder() {

}

void CameraTargetHolder::movement() {
    mTargetActor->movement();
}

CameraTargetObj *CameraTargetHolder::get() {
    return mTarget;
}

void CameraTargetHolder::set(CameraTargetObj *pTarget) {
    mTarget = pTarget;
}

void CameraTargetHolder::set(const LiveActor *pActor) {
    mTargetActor->_10 = pActor;
    mTarget = mTargetActor;
}

/*void CameraTargetHolder::set(const MarioActor *pActor) {
    mTargetPlayer->_10 = pActor;
    mTarget = mTargetPlayer;
}*/

bool CameraTargetHolder::isOnGround() const {
    return !mTarget->isJumping() || mTarget->isWaterMode() || mTarget->isOnWaterSurface();
}

bool CameraTargetHolder::isMoving() const {
    TVec3f *lastMove = mTarget->getLastMove();
    float length = PSVECMag(reinterpret_cast<const Vec *>(lastMove));

    return length > 1.0f;
}