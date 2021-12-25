#include "Game/Camera/CameraDirector.h"
#include "Game/Camera/CameraTargetArg.h"
#include "Game/Camera/CameraTargetObj.h"
#include "Game/Camera/CameraTargetMtx.h"
#include "Game/LiveActor/LiveActor.h"

CameraTargetArg::CameraTargetArg(const LiveActor *pLiveActor) {
    mLiveActor = pLiveActor;
    mTargetObj = NULL;
    mTargetMtx = NULL;
    mMarioActor = NULL;
}

void CameraTargetArg::setTarget() const {
    CameraTargetObj* obj = mTargetObj;

    if (obj != NULL) {
        CameraDirector *director = MR::getCameraDirector();
        director->setTarget(obj);
        return;
    }

    CameraTargetMtx *mtx = mTargetMtx;
    
    if (mtx != NULL)  {
        mtx->invalidateLastMove();

        CameraDirector *director = MR::getCameraDirector();
        director->setTarget(mTargetMtx);
        return;
    }

    const LiveActor *liveActor = mLiveActor;
    
    if (liveActor != NULL) {
        CameraDirector *director = MR::getCameraDirector();
        director->setTargetActor(liveActor);
        return;
    }

    MarioActor *marioActor = mMarioActor;
    
    if (marioActor != NULL) {
        CameraDirector *director = MR::getCameraDirector();
        director->setTargetPlayer(marioActor);
    }
}