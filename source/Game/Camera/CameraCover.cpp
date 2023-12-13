#include "Game/Camera/CameraCover.hpp"
#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"

namespace {
    const f32 sAngleThreshold = (JGeometry::TUtil<f32>::PI() * 60.0f) / 180.0f;
};

CameraCover::CameraCover(const char *pName) : NameObj(pName) {
    _3C = 0;
    _40 = 0;
    _41 = false;
    _44 = 1;
    mActor = new CaptureScreenActor(46, "Camera");
    MR::connectToScene(this, 26, -1, -1, 81);
    MR::joinToNameObjGroup(this, "IgnorePauseNameObj");
    _C.identity();
    mActor->initWithoutIter();
}

void CameraCover::movement() {
    if (isCameraHopping()) {
        if (!_3C) {
            MR::startToCaptureScreen("Camera");
            CategoryList::execute((MR::DrawType)0x2E);
            _3C = _44 + 1;
        }
    }

    if (_3C && !--_3C) {
        MR::endToCaptureScreen("Camera");
    }

    copyCamera();
    _40 = 0;
    _41 = false;
}

void CameraCover::draw() const {
    if (_3C) {
        ResTIMG* timg = MR::getScreenResTIMG();
        JUTTexture texture;
        texture._28 = 0;
        texture.storeTIMG(timg, (u8)0);
        texture._3B &= 2;
        ImageEffectLocalUtil::setupDrawTexture();
        ImageEffectLocalUtil::drawTexture(&texture, 1, 0, 255, (ImageEffectLocalUtil::ETexDrawType)0);
    }
}

void CameraCover::cover(u32 a1) {
    _44 = a1;
    _41 = true;
}

bool CameraCover::isCameraHopping() const {
    if (_40) {
       return false; 
    }

    return _41; 
}

void CameraCover::copyCamera() {
    _C.setInline(*MR::getCameraInvViewMtx());
}

CameraCover::~CameraCover() {

}