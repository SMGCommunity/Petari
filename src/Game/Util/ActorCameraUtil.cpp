#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <cstdio>

namespace MR {
    ActorCameraInfo* createActorCameraInfo(const JMapInfoIter &rIter) {
        ActorCameraInfo* pInfo = new ActorCameraInfo(rIter);
        return pInfo;
    }

    bool createActorCameraInfoIfExist(const JMapInfoIter &rIter, ActorCameraInfo **pInfo) {
        if (ActorCameraInfo(rIter).mCameraSetID == -1) {
            return false;
        }
        else {
            (*pInfo) = new ActorCameraInfo(rIter);
            return true;
        }
    }

    bool initActorCamera(const LiveActor *pActor, const JMapInfoIter &rIter, ActorCameraInfo **pInfo) {
        return initMultiActorCamera(pActor, rIter, pInfo, nullptr);
    }

    bool initMultiActorCamera(const LiveActor *pActor, const JMapInfoIter &rIter, ActorCameraInfo **pInfo, const char *pName) {
        if ((*pInfo) == nullptr) {
            (*pInfo) = new ActorCameraInfo(rIter);
        }
        return initMultiActorCameraNoInit(pActor, (*pInfo), pName);
    }

    void initActorCameraProgrammable(const LiveActor *pActor) {
        declareEventCameraProgrammable(pActor->mName);
        return;
    }

    bool startActorCameraNoTarget(const LiveActor *pActor, const ActorCameraInfo *pInfo, s32 a3) {
        CameraTargetArg target = CameraTargetArg();
        return startMultiActorCameraTargetOther(pActor, pInfo, nullptr, target, a3);
    }

    bool startActorCameraTargetPlayer(const LiveActor *pActor, const ActorCameraInfo *pInfo, s32 a3) {
        CameraTargetArg target = CameraTargetArg();
        setCameraTargetToPlayer(&target);
        return startMultiActorCameraTargetOther(pActor, pInfo, nullptr, target, a3);
    }

    bool startActorCameraTargetSelf(const LiveActor *pActor, const ActorCameraInfo *pInfo, s32 a3) {
        CameraTargetArg target = CameraTargetArg(pActor);
        return startMultiActorCameraTargetOther(pActor, pInfo, nullptr, target, a3);
        
    }

    bool startActorCameraTargetOther(const LiveActor *pActor, const ActorCameraInfo *pInfo, const CameraTargetArg &rTarget, s32 a4) {
        return startMultiActorCameraTargetOther(pActor, pInfo, nullptr, rTarget, a4);
        
    }

    bool startMultiActorCameraNoTarget(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, s32 a4) {
        CameraTargetArg target = CameraTargetArg();
        return startMultiActorCameraTargetOther(pActor, pInfo, pName, target, a4);
        
    }

    bool startMultiActorCameraTargetPlayer(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, s32 a4) {
        CameraTargetArg target = CameraTargetArg();
        setCameraTargetToPlayer(&target);
        return startMultiActorCameraTargetOther(pActor, pInfo, pName, target, a4);       
    }

    bool startMultiActorCameraTargetSelf(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, s32 a4) {
        return startMultiActorCameraTargetOther(pActor, pInfo, pName, CameraTargetArg(pActor), a4);
    }
     
    void startActorCameraProgrammable(const LiveActor *pActor, s32 a2) {
        startGlobalEventCameraNoTarget(pActor->mName, a2);
    }

    bool endActorCamera(const LiveActor *pActor, const ActorCameraInfo *pInfo, bool a3, s32 a4) {
        return endMultiActorCamera(pActor, pInfo, nullptr, a3, a4);
        
    }

    bool endActorCameraAtLanding(const LiveActor *pActor, const ActorCameraInfo *pInfo, s32 a3) {
        return endMultiActorCameraAtLanding(pActor, pInfo, nullptr, a3);
        
    }

    void endActorCameraProgrammable(const LiveActor *pActor, s32 a2, bool a3) {
        endGlobalEventCamera(pActor->mName, a2, a3);
        return;
    }

    bool isActiveActorCamera(const LiveActor *pActor, const ActorCameraInfo *pInfo) {
        return isActiveMultiActorCamera(pActor, pInfo, nullptr);
    }

    void setProgrammableCameraParam(const LiveActor *pActor, const TVec3f &v1, const TVec3f &v2, const TVec3f &v3) {
        setProgrammableCameraParam(pActor->mName, v1, v2, v3, true);
        return;
    }

    void setProgrammableCameraParamFovy(const LiveActor *pActor, f32 fov) {
        setProgrammableCameraParamFovy(pActor->mName, fov);
        return;
    }
};

CameraTargetArg::CameraTargetArg() {
    mTargetObj = nullptr;
    mTargetMtx = nullptr;
    mLiveActor = nullptr;
    mMarioActor = nullptr;
}
