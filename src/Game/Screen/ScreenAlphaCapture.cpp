#include "Game/Screen/ScreenAlphaCapture.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>

namespace {
    ScreenAlphaCapture* getScreenAlphaCapture() {
        return MR::getSceneObj< ScreenAlphaCapture >(SceneObj_ScreenAlphaCapture);
    }
};  // namespace

ScreenAlphaCapture::ScreenAlphaCapture(const char* pName) : NameObj(pName) {
    for (s32 i = 0; i < ARRAY_SIZE(mTexture); i++) {
        mTexture[i] = nullptr;
    }
}

void ScreenAlphaCapture::init(const JMapInfoIter& rIter) {
}

void ScreenAlphaCapture::createScreenAlpha(s32 textureIndex, f32 screenScale) {
    MR::CurrentHeapRestorer heapRestorer(MR::getSceneHeapGDDR3());

    mTexture[textureIndex] =
        new JUTTexture(JUTVideo::getManager()->getFbWidth() * screenScale, JUTVideo::getManager()->getEfbHeight() * screenScale, GX_TF_I8);
    mTexture[textureIndex]->mWrapS = GX_CLAMP;
    mTexture[textureIndex]->mWrapT = GX_CLAMP;
    mTexture[textureIndex]->mMinType = GX_LINEAR;
    mTexture[textureIndex]->mMagType = GX_LINEAR;
}

void ScreenAlphaCapture::captureScreenAlpha(s32 textureIndex) {
    GXRenderModeObj* renderModeObj = JUTVideo::getManager()->getRenderMode();

    GXSetCopyFilter(GX_FALSE, renderModeObj->sample_pattern, GX_FALSE, renderModeObj->vfilter);
    mTexture[textureIndex]->capture(0, 0, GX_CTF_A8, false, 0);
    GXSetCopyFilter(GX_FALSE, renderModeObj->sample_pattern, GX_TRUE, renderModeObj->vfilter);
}

void ScreenAlphaCapture::loadScreenTexture(s32 index, GXTexMapID id) {
    mTexture[index]->load(id);
    MR::resetTextureCacheSize();
}

JUTTexture* ScreenAlphaCapture::getTexture(s32 index) {
    return mTexture[index];
}

namespace MR {
    void createScreenAlphaSceneObj(s32 textureIndex, f32 screenScale) {
        MR::createSceneObj(SceneObj_ScreenAlphaCapture);
        ::getScreenAlphaCapture()->createScreenAlpha(textureIndex, screenScale);
    }

    void captureScreenAlpha(s32 textureIndex) {
        ::getScreenAlphaCapture()->captureScreenAlpha(textureIndex);
    }

    void loadScreenAlphaTexture(s32 index, GXTexMapID id) {
        ::getScreenAlphaCapture()->loadScreenTexture(index, id);
    }

    JUTTexture* getScreenAlphaTexture(s32 index) {
        return ::getScreenAlphaCapture()->getTexture(index);
    }
};  // namespace MR
