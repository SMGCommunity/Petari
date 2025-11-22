#include "Game/Camera/CamTranslatorBlackHole.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorBlackHole::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;
    TVec3f wPoint = general->mWPoint;

    CameraBlackHole* camera = mCamera;

    camera->mWPoint.set< f32 >(wPoint);
    camera->mAxis.set< f32 >(axis);
}

Camera* CamTranslatorBlackHole::getCamera() const {
    return mCamera;
}
