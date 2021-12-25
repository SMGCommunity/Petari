#include "Game/Camera/CameraAnim.h"
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorAnim.h"

void CamTranslatorAnim::setParam(const CameraParamChunk *pChunk) {
    CameraAnim *camera = reinterpret_cast<CameraAnim *>(mCamera);

    // mNum1 is used to store the pointer to the data
    camera->setParam(reinterpret_cast<unsigned char *>(pChunk->mGeneralParam->mNum1), pChunk->mGeneralParam->mDist);
}

Camera *CamTranslatorAnim::CamTranslatorAnim::getCamera() const {
    return mCamera;
}

u32 CamTranslatorAnim::getAnimFrame(const CameraParamChunk *pChunk) const {
    return CameraAnim::getAnimFrame(reinterpret_cast<unsigned char *>(pChunk->mGeneralParam->mNum1));
}