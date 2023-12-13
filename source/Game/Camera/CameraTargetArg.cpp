#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/LiveActor.hpp"

CameraTargetArg::CameraTargetArg(const LiveActor *pLiveActor) {
    mLiveActor = pLiveActor;
    mTargetObj = nullptr;
    mTargetMtx = nullptr;
    mMarioActor = nullptr;
}

void CameraTargetArg::setTarget() const {
    CameraTargetObj* obj = mTargetObj;

    if (obj != nullptr) {
        CameraDirector *director = MR::getCameraDirector();
        director->setTarget(obj);
        return;
    }

    CameraTargetMtx *mtx = mTargetMtx;
    
    if (mtx != nullptr)  {
        mtx->invalidateLastMove();

        CameraDirector *director = MR::getCameraDirector();
        director->setTarget(mTargetMtx);
        return;
    }

    const LiveActor *liveActor = mLiveActor;
    
    if (liveActor != nullptr) {
        CameraDirector *director = MR::getCameraDirector();
        director->setTargetActor(liveActor);
        return;
    }

    MarioActor *marioActor = mMarioActor;
    
    if (marioActor != nullptr) {
        CameraDirector *director = MR::getCameraDirector();
        director->setTargetPlayer(marioActor);
    }
}