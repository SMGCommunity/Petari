#include "Game/Screen/CaptureScreenDirector.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <cstring>

namespace {
    const TimingInfo cTimingInfo[] = {
        {"GameScreen", 32, true, 1},
        {"Indirect", 1, false, 0},
        {"Camera", 4, true, 1},
        {"WipeMap", 5, true, 1},
        {"SystemWipe", 20, false, 1},
        {"Unused", 16, false, 0}};
};

CaptureScreenDirector::CaptureScreenDirector()
    : NameObj("画面キャプチャ"),
      _C(nullptr),
      mTimingType("Indirect"),
      mTexture(nullptr),
      _18(false) {
    MR::CurrentHeapRestorer heapRestorer(MR::getStationedHeapGDDR3());

    mTexture = new JUTTexture(
        JUTVideo::sManager->mRenderModeObj->fbWidth,
        JUTVideo::sManager->mRenderModeObj->efbHeight,
        GX_TF_RGB565);
}

void CaptureScreenDirector::captureIfAllow(const char* pName) {
    const TimingInfo* pInfo = findFromName(pName);

    if (getUsingTiming() == pInfo && (getUsingTiming()->_C == 0 || _18)) {
        capture();

        _18 = false;
    }
}

void CaptureScreenDirector::capture() {
    GXRenderModeObj* pRenderModeObj = JUTVideo::sManager->mRenderModeObj;

    GXSetCopyFilter(GX_FALSE, pRenderModeObj->sample_pattern, GX_FALSE, pRenderModeObj->vfilter);
    mTexture->capture(0, 0, GX_TF_RGB565, false, 0);
    GXSetCopyFilter(GX_FALSE, pRenderModeObj->sample_pattern, GX_TRUE, pRenderModeObj->vfilter);
}

void CaptureScreenDirector::requestCaptureTiming(const char* pName) {
    const TimingInfo* pInfo = findFromName(pName);

    if (getCurrentTiming() == nullptr || getCurrentTiming()->mTiming < pInfo->mTiming) {
        _C = pName;
        _18 = true;
    }
}

void CaptureScreenDirector::invalidateCaptureTiming(const char* pName) {
    const TimingInfo* pInfo = getCurrentTiming();

    if (pInfo != nullptr) {
        if (strcmp(pInfo->mName, pName) == 0) {
            _C = nullptr;
        }
    }
}

ResTIMG* CaptureScreenDirector::getResTIMG() const {
    return mTexture->mTIMG;
}

u8* CaptureScreenDirector::getTexImage() const {
    return mTexture->mImage;
}

const TimingInfo* CaptureScreenDirector::getUsingTiming() const {
    if (_C != nullptr) {
        return findFromName(_C);
    }

    return findFromName(mTimingType);
}

const TimingInfo* CaptureScreenDirector::getCurrentTiming() const {
    if (_C != nullptr) {
        return findFromName(_C);
    }

    return nullptr;
}

const TimingInfo* CaptureScreenDirector::findFromName(const char* pName) const {
    for (int i = 0; i < sizeof(cTimingInfo) / sizeof(*cTimingInfo); i++) {
        if (strcmp(cTimingInfo[i].mName, pName) == 0) {
            return &cTimingInfo[i];
        }
    }

    return nullptr;
}

CaptureScreenActor::CaptureScreenActor(u32 param1, const char* pCameraName)
    : NameObj("画面キャプチャ"),
      mCameraName(pCameraName) {
    MR::connectToScene(this, -1, -1, -1, param1);
}

void CaptureScreenActor::draw() const {
    MR::captureScreenIfAllow(mCameraName);
}
