#include "Game/Screen/CaptureScreenDirector.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/JUtility/JUTVideo.hpp"
#include <cstring>

CaptureScreenDirector::CaptureScreenDirector() : NameObj("画面キャプチャ") {
    _C = nullptr;
    mTimingType = "Indirect";
    mTexture = nullptr;
    _18 = 0;
    MR::CurrentHeapRestorer heap_restorer((JKRHeap*)MR::getStationedHeapGDDR3());
    mTexture = new JUTTexture(JUTVideo::sManager->mRenderModeObj->fbWidth, JUTVideo::sManager->mRenderModeObj->efbHeight, GX_TF_RGB565);
}

void CaptureScreenDirector::captureIfAllow(const char *pName) {
    const TimingInfo* info = findFromName(pName);
    if (getUsingTiming() == info && (!getUsingTiming()->_C || _18)) {
        capture();
        _18 = 0;
    }
}

void CaptureScreenDirector::capture() {
    _GXRenderModeObj* renderObj = JUTVideo::sManager->mRenderModeObj;
    GXSetCopyFilter(GX_FALSE, renderObj->sample_pattern, GX_FALSE, renderObj->vfilter);
    mTexture->capture(0, 0, GX_TF_RGB565, false, 0);
    GXSetCopyFilter(GX_FALSE, renderObj->sample_pattern, GX_TRUE, renderObj->vfilter);
}

void CaptureScreenDirector::requestCaptureTiming(const char *pName) {
    const TimingInfo* info = findFromName(pName);
    if (!getCurrentTiming() || getCurrentTiming()->timing < info->timing) {
        _C = pName;
        _18 = 1;
    }
}

void CaptureScreenDirector::invalidateCaptureTiming(const char *pName) {
    const TimingInfo* info = getCurrentTiming();
    if (info) {
        if (strcmp(info->name, pName) == 0) {
            _C = 0;
        }
    }
}

ResTIMG* CaptureScreenDirector::getResTIMG() const {
    return mTexture->_20;
}

ResTIMG* CaptureScreenDirector::getTexImage() const {
    return mTexture->_24;
}

const TimingInfo* CaptureScreenDirector::getUsingTiming() const {
    if (_C) {
        return findFromName(_C);
    }

    return findFromName(mTimingType);
}

const TimingInfo* CaptureScreenDirector::getCurrentTiming() const {
    if (_C) {
        return findFromName(_C);
    }

    return nullptr;
}

const TimingInfo* CaptureScreenDirector::findFromName(const char *pName) const {
    for (u32 i = 0; i < 6; i++) {
        if (strcmp(cTimingInfo[i].name, pName) == 0) {
            return &cTimingInfo[i];
        }
    }

    return nullptr;
}

CaptureScreenActor::CaptureScreenActor(u32 scene_arg, const char *pCameraName) : NameObj("画面キャプチャ") {
    mCameraName = pCameraName;
    MR::connectToScene(this, -1, -1, -1, scene_arg);
}

void CaptureScreenActor::draw() const {
    MR::captureScreenIfAllow(mCameraName);
}

CaptureScreenDirector::~CaptureScreenDirector() {

}

CaptureScreenActor::~CaptureScreenActor() {

}
