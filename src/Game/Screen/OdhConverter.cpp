#include "Game/Screen/OdhConverter.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>

extern "C" u32 ODHEncodeRGB565(u8*, u8*, int, int, u32, int, u8*);

namespace {
    const u32 cCaptureWidth = 488;
    const u32 cCaptureHeight = 328;
    const f32 cCaptureOffsetX = 34.0f;
    const f32 cCaptureOffsetY = 44.0f;
    const u32 cLimitSize = 40960;
    const s32 cQuality = 80;
    const u32 cTexPosX = 0;
    const u32 cTexPosY = 0;

    OdhConverter* getConverter() {
        return MR::getSceneObj< OdhConverter >(SceneObj_OdhConverter);
    }
};  // namespace

OdhConverter::OdhConverter()
    : LayoutActor("ODH-jpeg変換", true), mCaptureWidth(cCaptureWidth), mCaptureHeight(cCaptureHeight), mLimitSize(cLimitSize),
      mIsRequestedCapture(false), mImage(nullptr), mCaptureImage(nullptr) {}

void OdhConverter::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, -1, -1, -1, 62);
    MR::CurrentHeapRestorer heapRestorer(MR::getSceneHeapGDDR3());

    mImage = new (32) u8[mLimitSize];
    mCaptureImage = new (32) u8[mCaptureWidth * mCaptureHeight * 3];
}

void OdhConverter::draw() const {}

void OdhConverter::convert() {
    const GXRenderModeObj* pRenderModeObj;
    u8* pScreenTexImage;

    pScreenTexImage = MR::getScreenTexImage();
    pRenderModeObj = JUTVideo::sManager->mRenderModeObj;

    GXSetCopyFilter(GX_FALSE, pRenderModeObj->sample_pattern, GX_FALSE, pRenderModeObj->vfilter);
    GXSetTexCopySrc(cTexPosX, cTexPosY, mCaptureWidth, mCaptureHeight);
    GXSetTexCopyDst(mCaptureWidth, mCaptureHeight, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(pScreenTexImage, GX_FALSE);
    GXDrawDone();
    GXSetCopyFilter(GX_FALSE, pRenderModeObj->sample_pattern, GX_TRUE, pRenderModeObj->vfilter);
    DCStoreRange(pScreenTexImage, mCaptureWidth * mCaptureHeight * 2);

    mImageSize = ODHEncodeRGB565(pScreenTexImage, mImage, mCaptureWidth, mCaptureHeight, mLimitSize, cQuality, mCaptureImage);
}

namespace MR {
    void createOdhConverter() {
        createSceneObj(SceneObj_OdhConverter);
    }

    void requestCaptureOdhImage() {
        return getConverter()->requestCapture();
    }

    bool isRequestedCaptureOdhImage() {
        if (!isExistSceneObj(SceneObj_OdhConverter)) {
            return false;
        }

        return getConverter()->isRequestedCapture();
    }

    void captureOdhImage() {
        getConverter()->capture();
    }

    void setPortCaptureOdhImage() {
        Mtx44 mtx;
        f32 bottom;
        f32 right;
        f32 top;
        f32 left;

        top = getFrameBufferHeight() * 0.5f - cCaptureOffsetX;
        left = -getFrameBufferWidth() * 0.5f + cCaptureOffsetY;
        right = getFrameBufferWidth() + left;
        bottom = top - getFrameBufferHeight();

        C_MTXOrtho(mtx, top, bottom, left, right, -1000.0f, 1000.0f);
        GXSetProjection(mtx, GX_ORTHOGRAPHIC);
    }

    const u8* getOdhImage() {
        if (!isExistSceneObj(SceneObj_OdhConverter)) {
            return nullptr;
        }

        return getConverter()->getImage();
    }

    u32 getOdhImageSize() {
        if (!isExistSceneObj(SceneObj_OdhConverter)) {
            return nullptr;
        }

        return getConverter()->getImageSize();
    }

    NameObjAdaptor* createAdaptorAndConnectToWiiMessageBoard(const char* pParam1, const FunctorBase& rFunc) {
        NameObjAdaptor* pDrawAdaptor = createDrawAdaptor(pParam1, rFunc);

        connectToScene(pDrawAdaptor, -1, -1, -1, 78);

        return pDrawAdaptor;
    }
};  // namespace MR
