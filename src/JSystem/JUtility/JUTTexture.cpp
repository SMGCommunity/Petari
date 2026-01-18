#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTPalette.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>

JUTTexture::JUTTexture() {
    setCaptureFlag(false);

    mEmbPalette = nullptr;
    mTIMG = nullptr;
}

JUTTexture::JUTTexture(int width, int height, GXTexFmt format) {
    mFlag = mFlag & 2 | 1;
    u32 bufSize = GXGetTexBufferSize(width, height, format, GX_FALSE, 1);

    ResTIMG* texBuf = reinterpret_cast< ResTIMG* >(new (sizeof(ResTIMG)) u8[bufSize + sizeof(ResTIMG)]);
    _3C = texBuf;
    texBuf->mFormat = format;
    texBuf->mTransparency = 0;
    texBuf->mWidth = width;
    texBuf->mHeight = height;
    texBuf->mWrapS = GX_CLAMP;
    texBuf->mWrapT = GX_CLAMP;
    texBuf->mPaletteName = GX_TLUT0;
    texBuf->mPaletteFormat = GX_TL_IA8;
    texBuf->mPaletteNum = 0;
    texBuf->mPaletteDataOffset = 0;
    texBuf->mMipmap = false;
    texBuf->mDoEdgeLod = false;
    texBuf->mBiasClamp = false;
    texBuf->mMaxAnisotropy = GX_ANISO_1;
    texBuf->mMinType = GX_LINEAR;
    texBuf->mMagType = GX_LINEAR;
    texBuf->mMinLod = 0;
    texBuf->mMaxLod = 0;
    texBuf->mImageNum = 1;
    texBuf->mLodBias = 0;
    texBuf->mImageDataOffset = sizeof(ResTIMG);
    mEmbPalette = nullptr;

    // cast to u8 solves ambiguity
    storeTIMG(texBuf, static_cast< u8 >(0));
    DCFlushRange(mImage, bufSize);
}

JUTTexture::~JUTTexture() {
    if (mFlag & 0x1) {
        delete[] _3C;
    }

    if (mFlag & 0x2) {
        delete mEmbPalette;
    }
}

// JUTTexture::storeTIMG(const ResTIMG *, u8)

void JUTTexture::storeTIMG(const ResTIMG* pTIMG, JUTPalette* pPalette) {
    GXTlut tlut;

    if (pPalette != nullptr) {
        tlut = static_cast< GXTlut >(pPalette->mName);
    } else {
        tlut = GX_TLUT0;
    }

    storeTIMG(pTIMG, pPalette, tlut);
}

// JUTTexture::storeTIMG(ResTIMG const *, JUTPalette *, GXTlut)

void JUTTexture::attachPalette(JUTPalette* pPalette) {
    if (mTIMG->mPaletteName == GX_TLUT0) {
        return;
    }

    if (pPalette == nullptr && mEmbPalette != nullptr) {
        mPalette = mEmbPalette;
    } else {
        mPalette = pPalette;
    }

    initTexObj(static_cast< GXTlut >(mPalette->mName));
}

void JUTTexture::init() {
    if (mTIMG->mPaletteNum == 0) {
        initTexObj();
    } else if (mEmbPalette != nullptr) {
        mPalette = mEmbPalette;

        initTexObj(static_cast< GXTlut >(mPalette->mName));
    }
}

/*
void JUTTexture::initTexObj() {
    u32 imageDataOffset = mTIMG->mImageDataOffset != 0
        ? mTIMG->mImageDataOffset
        : sizeof(ResTIMG);

    GXInitTexObj(
        &mObj,
        static_cast<void*>(mTIMG + imageDataOffset),
        mTIMG->mWidth,
        mTIMG->mHeight,
        static_cast<GXTexFmt>(mTIMG->mFormat),
        static_cast<GXTexWrapMode>(mTIMG->mWrapS),
        static_cast<GXTexWrapMode>(mTIMG->mWrapT),
        mTIMG->mMipmap != false);
    GXInitTexObjLOD(
        &mObj,
        static_cast<GXTexFilter>(mMinType),
        static_cast<GXTexFilter>(mMagType),
        mMinLod * 8.0f,
        mMaxLod * 8.0f,
        mLodBias * 100.0f,
        mTIMG->mBiasClamp,
        mTIMG->mDoEdgeLod,
        static_cast<GXAnisotropy>(mTIMG->mMaxAnisotropy));
}
*/

// JUTTexture::initTexObj(GXTlut)

void JUTTexture::load(GXTexMapID texMapID) {
    if (mPalette != nullptr) {
        mPalette->load();
    }

    GXLoadTexObj(&mObj, texMapID);
}

void JUTTexture::capture(int width, int height, GXTexFmt format, bool mipmap, GXBool clear) {
    if (mFlag & 0x1) {
        if (mipmap) {
            GXSetTexCopySrc(width, height, mTIMG->mWidth * 2, mTIMG->mHeight * 2);
        } else {
            GXSetTexCopySrc(width, height, mTIMG->mWidth, mTIMG->mHeight);
        }

        GXSetTexCopyDst(mTIMG->mWidth, mTIMG->mHeight, format, mipmap);
        GXCopyTex(mImage, clear);
        GXPixModeSync();
    }
}

void JUTTexture::setCaptureFlag(bool set) {
    mFlag = mFlag & 0x2 | (set << 0);
}

void JUTTexture::setEmbPaletteDelFlag(bool set) {
    mFlag = mFlag & 0x1 | (set << 1);
}
