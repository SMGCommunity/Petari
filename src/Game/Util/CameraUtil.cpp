#include "Game/Util.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/Camera/CameraContext.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "JSystem/J3DGraphBase/J3DSys.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/mtx.h"

namespace MR {
    bool calcScreenPosition(TVec2f *pResult, const TVec3f &rViewMtxMult) {
        TVec3f normalizedScreenPos;
        TVec3f screenPos;
        TVec3f multedViewMtx;
        getSceneObj<CameraContext*>(SceneObj_CameraContext)->getViewMtx()->mult(rViewMtxMult, multedViewMtx);
        bool ret = calcNormalizedScreenPositionFromView(&normalizedScreenPos, multedViewMtx);
        ::calcNormalizedScreenPosToScreenPos(&screenPos, normalizedScreenPos);
        pResult->x = screenPos.x;
        pResult->y = screenPos.y;
        return ret;
    }

    bool calcScreenPosition(TVec3f *pResult, const TVec3f &rViewMtxMult) {
        TVec3f normalizedScreenPos;
        TVec3f multedViewMtx;
        getSceneObj<CameraContext*>(SceneObj_CameraContext)->getViewMtx()->mult(rViewMtxMult, multedViewMtx);
        bool ret = calcNormalizedScreenPositionFromView(&normalizedScreenPos, multedViewMtx);
        ::calcNormalizedScreenPosToScreenPos(pResult, normalizedScreenPos);
        return ret;
    }

    bool calcNormalizedScreenPosition(TVec3f *pResult, const TVec3f &rViewMtxMult) {
        TVec3f multedViewMtx;
        getSceneObj<CameraContext*>(SceneObj_CameraContext)->getViewMtx()->mult(rViewMtxMult, multedViewMtx);
        return calcNormalizedScreenPositionFromView(pResult, multedViewMtx);
    }

    bool calcWorldRayDirectionFromScreen(TVec3f *pResult, const TVec2f &a2) {
        bool ret = calcWorldPositionFromScreen(pResult, a2, -1.0f);
        pResult->sub(getCamPos());
        return ret;
    }

    void loadProjectionMtx() {
        GXSetProjection(getSceneObj<CameraContext*>(SceneObj_CameraContext)->mProjection, (GXProjectionType)nullptr);
    }

    void loadViewMtx() {
        PSMTXCopy((MtxPtr)getSceneObj<CameraContext*>(SceneObj_CameraContext)->getViewMtx(), j3dSys.mViewMtx);
    }

    const MtxPtr getCameraViewMtx() {
        return (MtxPtr)getSceneObj<CameraContext*>(SceneObj_CameraContext)->getViewMtx();
    }

    TPos3f* getCameraInvViewMtx() {
        return const_cast<TPos3f*>(getSceneObj<CameraContext*>(SceneObj_CameraContext)->getInvViewMtx());
    }

    const TPos3f* getCameraProjectionMtx() {
        return &getSceneObj<CameraContext*>(SceneObj_CameraContext)->mProjection;
    }

    void setCameraViewMtx(const TPos3f &a1, bool a2, bool a3, const TVec3f &a4) {
        getSceneObj<CameraContext*>(SceneObj_CameraContext)->setViewMtx(a1, a2, a3, a4);
    }

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
        return dir;
    }

    TVec3f getCamYdir() {
        TPos3f viewMtx = *MR::getSceneObj<CameraContext*>(SceneObj_CameraContext)->getInvViewMtx();
        TVec3f dir;
        viewMtx.getYDir(dir);
        MR::normalizeOrZero(&dir);
        return dir;
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
