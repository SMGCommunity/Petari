#include "Game/Screen/ScreenPreserver.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/J2DGraph/J2DPicture.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "JSystem/JUtility/JUTVideo.hpp"

ScreenPreserver::ScreenPreserver() : NameObj(""), mIsActive(false) {
    for (s32 i = 0; i < ARRAY_SIZE(_D); i++) {
        _D[i] = 0;
    }
}

void ScreenPreserver::captureIfAllow() {
    MR::captureScreenIfAllow("GameScreen");
}

void ScreenPreserver::draw() const {
    if (!mIsActive) {
        return;
    }

    J2DOrthoGraphSimple ortho;
    ortho.setPort();

    JUTTexture tex(MR::getScreenResTIMG(), static_cast<u8>(0));
    J2DPicture pic(&tex);

    f32 width = MR::getScreenWidth();
    f32 height = static_cast<s32>(JUTVideo::getManager()->getEfbHeight());
    pic.draw(0.0f, 0.0f, width, height, false, false, false);
}

void ScreenPreserver::activate() {
    if (!mIsActive) {
        mIsActive = true;
        MR::startToCaptureScreen("GameScreen");
    }
}

void ScreenPreserver::deactivate() {
    if (mIsActive) {
        mIsActive = false;
        MR::endToCaptureScreen("GameScreen");
    }
}
