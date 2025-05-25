#include "Game/NPC/MiiFaceIcon.hpp"
#include "Game/NPC/MiiFaceIconHolder.hpp"
#include "Game/NPC/MiiFaceRecipe.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

MiiFaceIcon::MiiFaceIcon(u16 width, u16 height, const MiiFaceRecipe& rRecipe, const char* pName) :
    NameObj(pName),
    mIndex(0),
    mIsFavoriteColorBG(false),
    mWidth(width),
    mHeight(height),
    mRecipe(new MiiFaceRecipe(rRecipe)),
    mImageBuffer(nullptr),
    mIsCreated(false),
    mMakeIconResult(RFLErrcode_Unknown),
    mIsRequestMakeIcon(false)
{
    createImageBuffer();
    MR::registerMiiFaceIcon(this);
}

void MiiFaceIcon::init(const JMapInfoIter& rIter) {
    
}

void MiiFaceIcon::movement() {
    
}

// FIXME: Stack accesses are ordered incorrectly.
void MiiFaceIcon::drawIcon() {
    if (mIsCreated) {
        return;
    }

    if (mIsFavoriteColorBG) {
        mMakeIconResult = mRecipe->makeIconWithFavoriteColor(mImageBuffer, mWidth, mHeight);
    }
    else {
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
    u32 imageBufferSize = mWidth * mHeight * 2 + 32;

    mImageBuffer = new(32) u8[imageBufferSize];
    MR::zeroMemory(mImageBuffer, imageBufferSize);

    height = mHeight;
    width = mWidth;
    pImageBuffer = reinterpret_cast<ResTIMG*>(mImageBuffer);

    pImageBuffer->mFormat = GX_TF_RGB5A3;
    pImageBuffer->mWidth = width;
    pImageBuffer->mHeight = height;
    pImageBuffer->mWrapS = GX_CLAMP;
    pImageBuffer->mWrapT = GX_CLAMP;
    pImageBuffer->mPaletteFormat = 0;
    pImageBuffer->_9 = 0;
    pImageBuffer->mPaletteCount = 0;
    pImageBuffer->mPaletteOffs = 0;
    pImageBuffer->_10 = 0;
    pImageBuffer->_11 = 0;
    pImageBuffer->_12 = 0;
    pImageBuffer->_13 = 0;
    pImageBuffer->mMagType = GX_LINEAR;
    pImageBuffer->mMinType = GX_LINEAR;
    pImageBuffer->_16 = 0;
    pImageBuffer->_17 = 0;
    pImageBuffer->mTotalImgCount = 1;
    pImageBuffer->_1A = 0;
    pImageBuffer->mImgDataOffs = 32;
}
