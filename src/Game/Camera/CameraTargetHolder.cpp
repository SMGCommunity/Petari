#include "Game/Camera/CameraTargetHolder.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"

namespace {
// TODO: These are in lbl_805763C0/lbl_805763CD but im not sure if I can make it link to that
// (has to edit symbols.txt)
const char sCameraTargetActorName[] = "アクター注目";
const char sCameraTargetPlayerName[] = "マリオ注目";
}

CameraTargetHolder::CameraTargetHolder() {
    mTarget = nullptr;
    mTargetActor = new CameraTargetActor(sCameraTargetActorName);
    mTargetPlayer = new CameraTargetPlayer(sCameraTargetPlayerName);
}

void CameraTargetHolder::movement() {
    mTarget->movement();
}

CameraTargetObj* CameraTargetHolder::get() {
    return mTarget;
}

void CameraTargetHolder::set(CameraTargetObj* pTarget) {
    mTarget = pTarget;
}

void CameraTargetHolder::set(const LiveActor* pActor) {
    mTargetActor->mActor = pActor;
    mTarget = mTargetActor;
}

void CameraTargetHolder::set(const MarioActor* pActor) {
    mTargetPlayer->mActor = pActor;
    mTarget = mTargetPlayer;
}

bool CameraTargetHolder::isOnGround() const {
    return !mTarget->isJumping() || mTarget->isWaterMode() || mTarget->isOnWaterSurface();
}

bool CameraTargetHolder::isMoving() const {
    const TVec3f* lastMove = mTarget->getLastMove();
    f32 length = PSVECMag(reinterpret_cast< const Vec* >(lastMove));

    return length > 1.0f;
}

CameraTargetHolder::~CameraTargetHolder() {}
__declspec(section ".data") extern const u32 sCameraTargetHolderDataPad = 0;
__declspec(section ".sdata2") extern const f32 sCameraTargetHolderSdata2Pad = 0.0f;
