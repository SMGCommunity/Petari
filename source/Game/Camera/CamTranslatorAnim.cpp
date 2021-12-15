#include "Game/Camera/CameraAnim.h"
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorAnim.h"

CamTranslatorAnim::CamTranslatorAnim(CameraAnim *pCamera) : CamTranslatorDummy(pCamera) {

}

/*void CamTranslatorAnim::setParam(const CameraParamChunk *pChunk) {
    CameraAnim *camera = reinterpret_cast<CameraAnim *>(mCamera);

    // This is wrong, don't understand anything here
    //camera->setParam(pChunk->mGeneralParam->mNum1, pChunk->mGeneralParam->mDist);
}*/

Camera *CamTranslatorAnim::CamTranslatorAnim::getCamera() const {
    return mCamera;
}