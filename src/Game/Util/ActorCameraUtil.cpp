#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
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

    bool startMultiActorCameraTargetOther(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pName, const CameraTargetArg& rArg,
                                          s32 a5) {
        char newName[0x100];

        if (pInfo->mCameraSetID >= 0) {
            if (pName != nullptr) {
                ::createMultiActorCameraName(newName, sizeof(newName), pActor, pInfo, pName);
            } else {
                ::createActorCameraName(newName, sizeof(newName), pActor, pInfo);
            }
            startEventCamera(pInfo, newName, rArg, a5);
            return true;
        } else {
            return false;
        }
    }

    void startAnimCameraTargetPlayer(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* a3, s32 a4, f32 a5) {
        CameraTargetArg arg;
        MR::setCameraTargetToPlayer(&arg);
        char name[0x80];
        snprintf(name, sizeof(name), "%s%s", pActor->mName, a3);
        MR::startEventCameraAnim(pInfo, name, arg, a4, a5);
    }

    void startAnimCameraTargetSelf(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* a3, s32 a4, f32 a5) {
        CameraTargetArg arg(pActor);
        char name[0x80];
        snprintf(name, sizeof(name), "%s%s", pActor->mName, a3);
        MR::startEventCameraAnim(pInfo, name, arg, a4, a5);
    }

    void startAnimCameraTargetOther(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* a3, const CameraTargetArg& rTarget, s32 a5,
                                    f32 a6) {
        char name[0x80];
        snprintf(name, sizeof(name), "%s%s", pActor->mName, a3);
        MR::startEventCameraAnim(pInfo, name, rTarget, a5, a6);
    }

    void startActorCameraProgrammable(const LiveActor* pActor, s32 a2) {
        startGlobalEventCameraNoTarget(pActor->mName, a2);
    }

    bool endActorCamera(const LiveActor* pActor, const ActorCameraInfo* pInfo, bool a3, s32 a4) {
        return endMultiActorCamera(pActor, pInfo, nullptr, a3, a4);
    }

    bool endMultiActorCamera(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* a3, bool a4, s32 a5) {
        if (pInfo->mCameraSetID >= 0) {
            char name[0x100];
            if (a3 != nullptr) {
                ::createMultiActorCameraName(name, sizeof(name), pActor, pInfo, a3);
            } else {
                ::createActorCameraName(name, sizeof(name), pActor, pInfo);
            }

            MR::endEventCamera(pInfo, name, a4, a5);
            return true;
        }

        return false;
    }

    bool endActorCameraAtLanding(const LiveActor* pActor, const ActorCameraInfo* pInfo, s32 a3) {
        return endMultiActorCameraAtLanding(pActor, pInfo, nullptr, a3);
    }

    bool endMultiActorCameraAtLanding(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* a3, s32 a4) {
        if (pInfo->mCameraSetID >= 0) {
            char name[0x100];
            if (a3 != nullptr) {
                ::createMultiActorCameraName(name, sizeof(name), pActor, pInfo, a3);
            } else {
                ::createActorCameraName(name, sizeof(name), pActor, pInfo);
            }

            MR::endEventCameraAtLanding(pInfo, name, a4);
            return true;
        }

        return false;
    }

    bool isActiveMultiActorCamera(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* a3) {
        if (pInfo->mCameraSetID < 0) {
            return false;
        }

        char name[0x100];
        if (a3 != nullptr) {
            ::createMultiActorCameraName(name, sizeof(name), pActor, pInfo, a3);
        } else {
            ::createActorCameraName(name, sizeof(name), pActor, pInfo);
        }

        return MR::isEventCameraActive(pInfo, name);
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

    void initAnimCamera(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* a3, const char* a4) {
        char actorName[128];
        char camName[64];
        snprintf(actorName, sizeof(actorName), "%s%s", pActor->mName, a4);
        snprintf(camName, sizeof(camName), "%s.camn", a4);
        MR::declareEventCameraAnim(pInfo, actorName, MR::loadResourceFromArc(a3, camName));
    }

    void endAnimCamera(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* a3, s32 a4, bool a5) {
        char name[0x80];
        snprintf(name, sizeof(name), "%s%s", pActor->mName, a3);
        MR::endEventCamera(pInfo, name, a5, a4);
    }

    s32 getAnimCameraFrame(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pName) {
        char name[0x100];
        snprintf(name, sizeof(name), "%s%s", pActor->mName, pName);
        return MR::getAnimCameraFrame(pInfo, name);
    }

    bool isAnimCameraEnd(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pName) {
        char name[0x100];
        snprintf(name, sizeof(name), "%s%s", pActor->mName, pName);
        return MR::isAnimCameraEnd(pInfo, name);
    }

    s32 getActorCameraFrames(const LiveActor* pActor, const ActorCameraInfo* pInfo) {
        return MR::getMultiActorCameraFrames(pActor, pInfo, nullptr);
    }

    s32 getMultiActorCameraFrames(const LiveActor* pActor, const ActorCameraInfo* pInfo, const char* pName) {
        if (pInfo->mCameraSetID >= 0) {
            char name[0x100];

            if (pName != nullptr) {
                ::createMultiActorCameraName(name, sizeof(name), pActor, pInfo, pName);
            } else {
                ::createActorCameraName(name, sizeof(name), pActor, pInfo);
            }

            return MR::getEventCameraFrames(pInfo, name);
        }

        return 0;
    }

    bool isExistActorCamera(const ActorCameraInfo* pInfo) {
        if (pInfo == nullptr) {
            return false;
        }

        return pInfo->mCameraSetID != -1;
    }

    void startRumbleWithShakeCameraWeak(const LiveActor* pActor, const char* a2, const char* a3, f32 a4, f32 a5) {
        f32 dist = MR::calcDistanceToPlayer(pActor);

        if (dist <= a4) {
            MR::tryRumblePad(pActor, a2, 0);
            MR::shakeCameraNormal();
        } else {
            if (dist <= a5) {
                MR::tryRumblePad(pActor, a3, 0);
                MR::shakeCameraWeak();
            }
        }
    }

    void startRumbleWithShakeCameraNormalWeak(const LiveActor* pActor, const char* a2, const char* a3, f32 a4, f32 a5) {
        f32 dist = MR::calcDistanceToPlayer(pActor);

        if (dist <= a4) {
            MR::tryRumblePad(pActor, a2, 0);
            MR::shakeCameraNormalWeak();
        } else {
            if (dist <= a5) {
                MR::tryRumblePad(pActor, a3, 0);
                MR::shakeCameraWeak();
            }
        }
    }

    void startRumbleWithShakeCameraStrong(const LiveActor* pActor, const char* a2, const char* a3, f32 a4, f32 a5) {
        f32 dist = MR::calcDistanceToPlayer(pActor);

        if (dist <= a4) {
            MR::tryRumblePad(pActor, a2, 0);
            MR::shakeCameraStrong();
        } else {
            if (dist <= a5) {
                MR::tryRumblePad(pActor, a3, 0);
                MR::shakeCameraNormal();
            }
        }
    }

};  // namespace MR

CameraTargetArg::CameraTargetArg() : mTargetObj(nullptr), mTargetMtx(nullptr), mLiveActor(nullptr), mMarioActor(nullptr) {
}
