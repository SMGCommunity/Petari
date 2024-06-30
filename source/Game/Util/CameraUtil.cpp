#include "Game/Util.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/Camera/CameraContext.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace MR {
    f32 getAspect() {
        return MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->getAspect();
    }

    f32 getNearZ() {
        return MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->mNearZ;
    }

    f32 getFarZ() {
        return MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->mFarZ;
    }

    f32 getFovy() {
        return MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->mFovy;
    }

    void setNearZ(f32 nearZ) {
        MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->setNearZ(nearZ);
    }

    void setFovy(f32 fovy) {
        MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->setFovy(fovy);
    }

    void setShakeOffset(f32 a1, f32 a2) {
        MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->setShakeOffset(a1, a2);
    }

    const TVec3f getCamPos() {
        TPos3f viewMtx = *MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->getInvViewMtx();
        TVec3f pos;
        MR::extractMtxTrans(viewMtx.toMtxPtr(), &pos);
        return pos;
    }

    TVec3f getCamXDir() {
        TPos3f viewMtx = *MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->getInvViewMtx();
        TVec3f dir;
        viewMtx.getXDir(dir);
        MR::normalizeOrZero(&dir);
        return -dir;
    }

    TVec3f getCamYDir() {
        TPos3f viewMtx = *MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->getInvViewMtx();
        TVec3f dir;
        viewMtx.getYDir(dir);
        MR::normalizeOrZero(&dir);
        return -dir;
    }

    TVec3f getCamZdir() {
        TPos3f viewMtx = *MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->getInvViewMtx();
        TVec3f dir;
        viewMtx.getZDir(dir);
        MR::normalizeOrZero(&dir);
        return -dir;
    }

    void createMirrorCamera() {
        MR::createSceneObj(SceneObj_MirrorCamera);
    }

    bool isExistMirrorCamera() {
        return MR::isExistSceneObj(SceneObj_MirrorCamera);
    }

    // MR::getMirrorCameraViewMtx
    // MR::getMirrorModelTexMtx

    void completeCameraParameters() {
        MR::getCameraDirector()->closeCreatingCameraChunk();
    }

    void resetCameraMan() {
        MR::getCameraDirector()->requestToResetCameraMan();
    }

    void startCameraInterpolation(u32 intr) {
        MR::getCameraDirector()->setInterpolation(intr);
    }

    void declareEventCamera(const ActorCameraInfo *pInfo, const char *pEventName) {
        MR::getCameraDirector()->declareEvent(pInfo->mZoneID, pEventName);
    }

    void endEventCamera(const ActorCameraInfo *pInfo, const char *pEventName, bool a3, s32 a4) {
        MR::getCameraDirector()->endEvent(pInfo->mZoneID, pEventName, a3, a4);
    }

    void declareGlobalEventCameraAbyss(const char *pEventName) {
        MR::getCameraDirector()->declareEvent(0, pEventName);
        CameraParamChunkEvent* chunk = MR::getCameraDirector()->getEventParameter(0, pEventName);

        if (chunk) {
            chunk->setCameraType("CAM_TYPE_EYEPOS_FIX_THERE", MR::getCameraDirector()->mHolder);
            chunk->mGeneralParam->mNum1 = 1;
            chunk->_64 = true;
        }
    }
};