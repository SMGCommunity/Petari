#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <cstdio>

namespace {
    void createActorCameraName(char* pName, u32 size, const LiveActor* pActor, const ActorCameraInfo* pInfo) NO_INLINE {
        if (pInfo->mCameraSetID & 0x8000) {
            snprintf(pName, size, "%s共通%03d", pActor->mName, pInfo->mCameraSetID - 0x8000);
        } else {
            snprintf(pName, size, "%s固有%03d", pActor->mName, pInfo->mCameraSetID);
        }
    }

    void createMultiActorCameraName(char* pName, u32 size, const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pCamName) NO_INLINE {
        s32 id = pInfo->mCameraSetID;
        if (id & 0x8000) {
            snprintf(pName, size, "%s共通%s%03d", pActor->mName, id - 0x8000);
        } else {
            snprintf(pName, size, "%s固有%s%03d", pActor->mName, id);
        }
    }
};  // namespace

namespace MR {
    void initAnimCamera(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pName) {
        char actorName[128];
        char camName[64];
        snprintf(actorName, sizeof(actorName), "%s%s", pActor->mName, pName);
        snprintf(camName, sizeof(camName), "%s.camn", pName);
        MR::declareEventCameraAnim(pInfo, actorName, getResourceHolder(pActor)->mFileInfoTable->getRes(camName));
    }

    ActorCameraInfo* createActorCameraInfo(const JMapInfoIter& rIter) {
        return new ActorCameraInfo(rIter);
    }

    bool createActorCameraInfoIfExist(const JMapInfoIter& rIter, ActorCameraInfo** pInfo) {
        ActorCameraInfo newInfo(rIter);

        if (newInfo.mCameraSetID == -1) {
            return false;
        } else {
            (*pInfo) = new ActorCameraInfo(rIter);
            return true;
        }
    }

    bool initActorCamera(const LiveActor* pActor, const JMapInfoIter& rIter, ActorCameraInfo** pInfo) {
        return initMultiActorCamera(pActor, rIter, pInfo, nullptr);
    }

    bool initMultiActorCamera(const LiveActor* pActor, const JMapInfoIter& rIter, ActorCameraInfo** pInfo, const char* pName) {
        if (*pInfo == nullptr) {
            *pInfo = new ActorCameraInfo(rIter);
        }

        return initMultiActorCameraNoInit(pActor, (*pInfo), pName);
    }

    bool initMultiActorCameraNoInit(const LiveActor* pActor, ActorCameraInfo* pInfo, const char* pName) {
        char newName[0x100];
        if (pInfo->mCameraSetID < 0) {
            return false;
        }
        if (pName != nullptr) {
            ::createMultiActorCameraName(newName, 0x100, pActor, pInfo, pName);
        } else {
            ::createActorCameraName(newName, 0x100, pActor, pInfo);
        }
        declareEventCamera(pInfo, newName);
        return true;
    }

    void initActorCameraProgrammable(const LiveActor* pActor) {
        declareEventCameraProgrammable(pActor->mName);
    }

    bool startActorCameraNoTarget(const LiveActor* pActor, const ActorCameraInfo* pInfo, s32 a3) {
        CameraTargetArg target = CameraTargetArg();
        return startMultiActorCameraTargetOther(pActor, pInfo, nullptr, target, a3);
    }

    bool startActorCameraTargetPlayer(const LiveActor* pActor, const ActorCameraInfo* pInfo, s32 a3) {
        CameraTargetArg target = CameraTargetArg();
        setCameraTargetToPlayer(&target);
        return startMultiActorCameraTargetOther(pActor, pInfo, nullptr, target, a3);
    }

    bool startActorCameraTargetSelf(const LiveActor* pActor, const ActorCameraInfo* pInfo, s32 a3) {
        CameraTargetArg target = CameraTargetArg(pActor);
        return startMultiActorCameraTargetOther(pActor, pInfo, nullptr, target, a3);
    }

    bool startActorCameraTargetOther(const LiveActor* pActor, const ActorCameraInfo* pInfo, const CameraTargetArg& rTarget, s32 a4) {
        return startMultiActorCameraTargetOther(pActor, pInfo, nullptr, rTarget, a4);
    }

    bool startMultiActorCameraNoTarget(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pName, s32 a4) {
        CameraTargetArg target = CameraTargetArg();
        return startMultiActorCameraTargetOther(pActor, pInfo, pName, target, a4);
    }

    bool startMultiActorCameraTargetPlayer(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pName, s32 a4) {
        CameraTargetArg target = CameraTargetArg();
        setCameraTargetToPlayer(&target);
        return startMultiActorCameraTargetOther(pActor, pInfo, pName, target, a4);
    }

    bool startMultiActorCameraTargetSelf(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pName, s32 a4) {
        return startMultiActorCameraTargetOther(pActor, pInfo, pName, CameraTargetArg(pActor), a4);
    }

    /*
    bool startMultiActorCameraTargetOther(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, const CameraTargetArg &rArg, s32
    a5) { char newName; if (pInfo->mCameraSetID < 0) { return false;
        }
        else {
            if (pName != nullptr) {
            ::createMultiActorCameraName(&newName, 0x100, pActor, pInfo, pName);
            }
            else {
            ::createActorCameraName(&newName, 0x100, pActor, pInfo);
            }
            startEventCamera(pInfo, &newName, rArg, a5);
            return true;
        }
    }
    */

    void startActorCameraProgrammable(const LiveActor* pActor, s32 a2) {
        startGlobalEventCameraNoTarget(pActor->mName, a2);
    }

    bool endActorCamera(const LiveActor* pActor, const ActorCameraInfo* pInfo, bool a3, s32 a4) {
        return endMultiActorCamera(pActor, pInfo, nullptr, a3, a4);
    }

    bool endActorCameraAtLanding(const LiveActor* pActor, const ActorCameraInfo* pInfo, s32 a3) {
        return endMultiActorCameraAtLanding(pActor, pInfo, nullptr, a3);
    }

    void endActorCameraProgrammable(const LiveActor* pActor, s32 a2, bool a3) {
        endGlobalEventCamera(pActor->mName, a2, a3);
    }

    bool isActiveActorCamera(const LiveActor* pActor, const ActorCameraInfo* pInfo) {
        return isActiveMultiActorCamera(pActor, pInfo, nullptr);
    }

    void setProgrammableCameraParam(const LiveActor* pActor, const TVec3f& v1, const TVec3f& v2, const TVec3f& v3) {
        setProgrammableCameraParam(pActor->mName, v1, v2, v3, true);
    }

    void setProgrammableCameraParamFovy(const LiveActor* pActor, f32 fov) {
        setProgrammableCameraParamFovy(pActor->mName, fov);
    }
};  // namespace MR

CameraTargetArg::CameraTargetArg() : mTargetObj(nullptr), mTargetMtx(nullptr), mLiveActor(nullptr), mMarioActor(nullptr) {
}
