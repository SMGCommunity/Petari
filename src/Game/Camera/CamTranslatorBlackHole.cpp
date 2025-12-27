#include "Game/Camera/CamTranslatorBlackHole.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorBlackHole::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f vecs[2] = { general->mAxis, general->mWPoint };

    CameraBlackHole* camera = mCamera;

    camera->mWPoint.set< f32 >(vecs[1]);
    camera->mAxis.set< f32 >(vecs[0]);
}

Camera* CamTranslatorBlackHole::getCamera() const {
    return mCamera;
}
