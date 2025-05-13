#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <cstdio>

namespace {
    /* char* createActorCameraName(char *pName, u32 a2, const LiveActor *pActor, const ActorCameraInfo *pInfo) {
        if ((pInfo->mCameraSetID & 0x8000) != 0) {
            snprintf(pName, a2, "", pActor->mName, pInfo->mCameraSetID - 0x8000);
        }
        else {
            snprintf(pName, a2, "", pActor->mName);
        }
        return pName;
    } */

    /* char* createMultiActorCameraName(char *pNewName, u32 a2, const LiveActor *pActor, const ActorCameraInfo *pInfo, char *pName) {
        if ((pInfo->mCameraSetID & 0x8000) != 0) {
            snprintf(pNewName, a2, "", pActor->mName, pName, pInfo->mCameraSetID - 0x8000);
        }
        else {
            snprintf(pNewName, a2, "", pActor->mName);
        }
        return pNewName;
    } */
};

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

    /* bool initMultiActorCameraNoInit(const LiveActor *pActor, ActorCameraInfo *pInfo, const char *pName) {
        char *pNewName;
        if (pInfo->mCameraSetID < 0) {
            return false;
        }
        else {
            if (pName == nullptr) {
                pNewName = ::createActorCameraName(pNewName, 256, pActor, pInfo);
            }
            else {
                pNewName = ::createMultiActorCameraName(pNewName, 256, pActor, pInfo, pName);
            }
            declareEventCamera(pInfo, pNewName);
            return true;
        }
    } */


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

    /* bool startMultiActorCameraTargetOther(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, const CameraTargetArg &target, s32 a5) {
        char *pNewName;
        if (pInfo->mCameraSetID < 0) {
            return false;
        }
        else {
            if (pName == nullptr) {
                pNewName = ::createActorCameraName(pNewName, 256, pActor, pInfo);
            }
            else {
                pNewName = ::createMultiActorCameraName(pNewName, 256, pActor, pInfo, pName);
            }
            startEventCamera(pInfo, pNewName, target, a5);
            return true;
        }
    } */

    /* void startAnimCameraTargetPlayer(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, s32 a4, f32 a5) {
        CameraTargetArg target = CameraTargetArg();
        setCameraTargetToPlayer(&target);
        char* pNewName;
        snprintf(pNewName, 128, "", pActor->mName, pName);
        startEventCameraAnim(pInfo, pNewName, target, a4, a5);
        return;
    }
    90.28% match */ 
    
    /* void startAnimCameraTargetSelf(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, s32 a4, f32 a5) {
        CameraTargetArg target = CameraTargetArg(pActor);
        char* pNewName;
        snprintf(pNewName, 128, "", pActor->mName, pName);
        startEventCameraAnim(pInfo, pNewName, target, a4, a5);
        return;
    }
    90.00% match */

    /* void startAnimCameraTargetOther(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, const CameraTargetArg &target, s32 a4, f32 a5) {
        char* pNewName;
        snprintf(pNewName, 128, "", pActor->mName, pName);
        startEventCameraAnim(pInfo, pNewName, target, a4, a5);
        return;
    }
    80.81% match */
     

    void startActorCameraProgrammable(const LiveActor *pActor, s32 a2) {
        startGlobalEventCameraNoTarget(pActor->mName, a2);
    }

    bool endActorCamera(const LiveActor *pActor, const ActorCameraInfo *pInfo, bool a3, s32 a4) {
        return endMultiActorCamera(pActor, pInfo, nullptr, a3, a4);
        
    }

    /* bool endMultiActorCamera(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, bool a4, s32 a5) {
        char *pNewName;
        if (pInfo->mCameraSetID < 0) {
            return false;
        }
        else {
            if (pName == nullptr) {
                pNewName = ::createActorCameraName(pNewName, 256, pActor, pInfo);
            }
            else {
                pNewName = ::createMultiActorCameraName(pNewName, 256, pActor, pInfo, pName);
            }
            endEventCamera(pInfo, pNewName, a4, a5);
            return true;
        }
    } */

    bool endActorCameraAtLanding(const LiveActor *pActor, const ActorCameraInfo *pInfo, s32 a3) {
        return endMultiActorCameraAtLanding(pActor, pInfo, nullptr, a3);
        
    }

    /* bool endMultiActorCameraAtLanding(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, s32 a4) {
        char *pNewName;
        if (pInfo->mCameraSetID < 0) {
            return false;
        }
        else {
            if (pName == nullptr) {
                pNewName = ::createActorCameraName(pNewName, 256, pActor, pInfo);
            }
            else {
                pNewName = ::createMultiActorCameraName(pNewName, 256, pActor, pInfo, pName);
            }
            endEventCameraAtLanding(pInfo, pNewName, a4);
            return true;
        }
    } */

    void endActorCameraProgrammable(const LiveActor *pActor, s32 a2, bool a3) {
        endGlobalEventCamera(pActor->mName, a2, a3);
        return;
    }

    bool isActiveActorCamera(const LiveActor *pActor, const ActorCameraInfo *pInfo) {
        return isActiveMultiActorCamera(pActor, pInfo, nullptr);
    }

    /* bool isActiveMultiActorCamera(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName {
        char *pNewName;
        if (pInfo->mCameraSetID < 0) {
            return false;
        }
        else {
            if (pName == nullptr) {
                pNewName = ::createActorCameraName(pNewName, 256, pActor, pInfo);
            }
            else {
                pNewName = ::createMultiActorCameraName(pNewName, 256, pActor, pInfo, pName);
            }
            isEventCameraActive(pInfo, pNewName);
            return true;
        }
    } */

    void setProgrammableCameraParam(const LiveActor *pActor, const TVec3f &v1, const TVec3f &v2, const TVec3f &v3) {
        setProgrammableCameraParam(pActor->mName, v1, v2, v3, true);
        return;
    }

    void setProgrammableCameraParamFovy(const LiveActor *pActor, f32 fov) {
        setProgrammableCameraParamFovy(pActor->mName, fov);
        return;
    }

    /* void initAnimCamera(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName1, const char *pName2) {
        char* pNewName1;
        snprintf(pNewName1, 128, "", pActor->mName, pName2);
        char* pNewName2;
        snprintf(pNewName2, 64, "", pName2);
        loadResourceFromArc(pName1, pNewName2);
        declareEventCameraAnim(pInfo, pNewName1, (void*) pName1);
        return;

    } */

    /* void endAnimCamera(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName, s32 a4, bool a5) {
        char* pNewName;
        snprintf(pNewName, 128, "", pActor->mName, pName);
        endEventCamera(pInfo, pNewName, a5, a4);
        return;
    } */

    /* s32 getAnimCameraFrame(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName) {
        char* pNewName;
        snprintf(pNewName, 256, "", pActor->mName, pName);
        return getAnimCameraFrame(pInfo, pNewName);
    } */

    /* bool isAnimCameraEnd(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName) {
        char* pNewName;
        snprintf(pNewName, 256, "", pActor->mName, pName);
        return isAnimCameraEnd(pInfo, pNewName);
    } */

    /* s32 getActorCameraFrames(const LiveActor *pActor, const ActorCameraInfo *pInfo) {
        getMultiActorCameraFrames(pActor, pInfo, nullptr);
    } */

    /* bool getMultiActorCameraFrames(const LiveActor *pActor, const ActorCameraInfo *pInfo, const char *pName {
        char *pNewName;
        if (pInfo->mCameraSetID < 0) {
            return false;
        }
        else {
            if (pName == nullptr) {
                pNewName = ::createActorCameraName(pNewName, 256, pActor, pInfo);
            }
            else {
                pNewName = ::createMultiActorCameraName(pNewName, 256, pActor, pInfo, pName);
            }
            getEventCameraFrames(pInfo, pNewName);
            return true;
        }
    } */

    /* bool isExistActorCamera(const ActorCameraInfo *pInfo) {
        if (pInfo == nullptr) {
            return false;
        }
        else if (pInfo->mCameraSetID == -1) {
            return false;
        }
        else {
            return true;
        }
    } */
     
    void startRumbleWithShakeCameraWeak(const LiveActor *pActor, const char *pName1, const char *pName2, f32 range1, f32 range2) {
        if (calcDistanceToPlayer(pActor) <= range1) {

                tryRumblePad((void*) pActor, pName1, 0);
                shakeCameraNormal();
            
        }
        else if (calcDistanceToPlayer(pActor) <= range2) {
            tryRumblePad((void*) pActor, pName2, 0);
            shakeCameraWeak();
        }
    }

    void startRumbleWithShakeCameraNormalWeak(const LiveActor *pActor, const char *pName1, const char *pName2, f32 range1, f32 range2) {
        if (calcDistanceToPlayer(pActor) <= range1) {

                tryRumblePad((void*) pActor, pName1, 0);
                shakeCameraNormalWeak();
            
        }
        else if (calcDistanceToPlayer(pActor) <= range2) {
            tryRumblePad((void*) pActor, pName2, 0);
            shakeCameraWeak();
        }
    }

    void startRumbleWithShakeCameraStrong(const LiveActor *pActor, const char *pName1, const char *pName2, f32 range1, f32 range2) {
        if (calcDistanceToPlayer(pActor) <= range1) {

                tryRumblePad((void*) pActor, pName1, 0);
                shakeCameraStrong();
            
        }
        else if (calcDistanceToPlayer(pActor) <= range2) {
            tryRumblePad((void*) pActor, pName2, 0);
            shakeCameraNormal();
        }
    }



};

CameraTargetArg::CameraTargetArg() {
    mTargetObj = nullptr;
    mTargetMtx = nullptr;
    mLiveActor = nullptr;
    mMarioActor = nullptr;
}
