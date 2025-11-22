#include "Game/NPC/MiiFaceIcon.hpp"
#include "Game/NPC/MiiFaceIconHolder.hpp"
#include "Game/NPC/MiiFaceRecipe.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

MiiFaceIcon::MiiFaceIcon(u16 width, u16 height, const MiiFaceRecipe& rRecipe, const char* pName)
    : NameObj(pName), mIndex(0), mIsFavoriteColorBG(false), mWidth(width), mHeight(height), mRecipe(new MiiFaceRecipe(rRecipe)),
      mImageBuffer(nullptr), mIsCreated(false), mMakeIconResult(RFLErrcode_Unknown), mIsRequestMakeIcon(false) {
    createImageBuffer();
    MR::registerMiiFaceIcon(this);
}

void MiiFaceIcon::init(const JMapInfoIter& rIter) {}

void MiiFaceIcon::movement() {}

// FIXME: Stack accesses are ordered incorrectly.
void MiiFaceIcon::drawIcon() {
    if (mIsCreated) {
        return;
    }

    if (mIsFavoriteColorBG) {
        mMakeIconResult = mRecipe->makeIconWithFavoriteColor(mImageBuffer, mWidth, mHeight);
    } else {
        GXColor bgColor;
        bgColor.r = 0;
        bgColor.g = 0;
        bgColor.b = 0;
        bgColor.a = 255;

        mMakeIconResult = mRecipe->makeIcon(mImageBuffer, mWidth, mHeight, bgColor);
    }

    mIsCreated = true;
    mIsRequestMakeIcon = false;
}

void MiiFaceIcon::requestToMakeIcon() {
    mIsRequestMakeIcon = true;
}

void MiiFaceIcon::setIndex(u16 index) {
    mIndex = index;
    mRecipe->mIndex = index;
    mIsCreated = false;
    mMakeIconResult = RFLErrcode_Unknown;
}

void MiiFaceIcon::createImageBuffer() {
    ResTIMG* pImageBuffer;
    u16 width;
    u16 height;
    u32 imageBufferSize = mWidth * mHeight * 2 + sizeof(ResTIMG);

    mImageBuffer = new (sizeof(ResTIMG)) u8[imageBufferSize];
    MR::zeroMemory(mImageBuffer, imageBufferSize);

    height = mHeight;
    width = mWidth;
    pImageBuffer = reinterpret_cast< ResTIMG* >(mImageBuffer);

    pImageBuffer->mFormat = GX_TF_RGB5A3;
    pImageBuffer->mWidth = width;
    pImageBuffer->mHeight = height;
    pImageBuffer->mWrapS = GX_CLAMP;
    pImageBuffer->mWrapT = GX_CLAMP;
    pImageBuffer->mPaletteName = GX_TLUT0;
    pImageBuffer->mPaletteFormat = GX_TL_IA8;
    pImageBuffer->mPaletteNum = 0;
    pImageBuffer->mPaletteDataOffset = 0;
    pImageBuffer->mMipmap = false;
    pImageBuffer->mDoEdgeLod = false;
    pImageBuffer->mBiasClamp = false;
    pImageBuffer->mMaxAnisotropy = GX_ANISO_1;
    pImageBuffer->mMinType = GX_LINEAR;
    pImageBuffer->mMagType = GX_LINEAR;
    pImageBuffer->mMinLod = 0;
    pImageBuffer->mMaxLod = 0;
    pImageBuffer->mImageNum = 1;
    pImageBuffer->mLodBias = 0;
    pImageBuffer->mImageDataOffset = sizeof(ResTIMG);
}
