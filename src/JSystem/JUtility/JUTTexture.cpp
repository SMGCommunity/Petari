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

void JUTTexture::storeTIMG(const ResTIMG* pTIMG, u8 a1) {
    if (pTIMG != nullptr && a1 < 0x10) {
        mTIMG = pTIMG;
        mImage = (u8*)pTIMG + pTIMG->mImageDataOffset;
        if (pTIMG->mImageDataOffset == 0)
            mImage = (u8*)pTIMG + sizeof(ResTIMG);  // If ImageDataOffset is 0x00, assume it's 0x20

        mPalette = nullptr;
        mTlutName = 0;
        mWrapS = getTexInfo()->mWrapS;
        mWrapT = getTexInfo()->mWrapT;
        mMinType = getTexInfo()->mMinType;
        mMagType = getTexInfo()->mMagType;
        mMinLod = (s8)getTexInfo()->mMinLod;
        mMaxLod = (s8)getTexInfo()->mMaxLod;
        mLodBias = (s16)getTexInfo()->mLodBias;

        if (getTexInfo()->mPaletteNum == 0) {
            initTexObj();
            return;
        }

        s32 lut;
        if (getTexInfo()->mPaletteNum > 256) {
            lut = (a1 % 4) + GX_BIGTLUT0;
        } else {
            lut = a1;
        }

        if (mEmbPalette == nullptr || (mFlag & 2) == 0) {
            mEmbPalette = new JUTPalette((GXTlut)lut, (GXTlutFmt)getTexInfo()->mPaletteFormat, (JUTTransparency)getTexInfo()->mTransparency,
                                         getTexInfo()->mPaletteNum, (u8*)getTexInfo() + getTexInfo()->mPaletteDataOffset);
            setEmbPaletteDelFlag(true);
        } else {
            mEmbPalette->storeTLUT((GXTlut)lut, (GXTlutFmt)getTexInfo()->mPaletteFormat, (JUTTransparency)getTexInfo()->mTransparency,
                                   getTexInfo()->mPaletteNum, (u8*)getTexInfo() + getTexInfo()->mPaletteDataOffset);
        }
        attachPalette(mEmbPalette);
    }
}

void JUTTexture::storeTIMG(const ResTIMG* pTIMG, JUTPalette* pPalette) {
    GXTlut tlut;

    if (pPalette != nullptr) {
        tlut = static_cast< GXTlut >(pPalette->mName);
    } else {
        tlut = GX_TLUT0;
    }

    storeTIMG(pTIMG, pPalette, tlut);
}

void JUTTexture::storeTIMG(ResTIMG const* pTIMG, JUTPalette* pPalette, GXTlut a1) {
    if (pTIMG != nullptr) {
        mTIMG = pTIMG;
        mImage = (u8*)pTIMG + pTIMG->mImageDataOffset;
        if (pTIMG->mImageDataOffset == 0)
            mImage = (u8*)pTIMG + sizeof(ResTIMG);  // If ImageDataOffset is 0x00, assume it's sizeof ResTIMG

        if ((mFlag & 2) != 0) {
            delete mEmbPalette;
        }
        mEmbPalette = pPalette;
        setEmbPaletteDelFlag(false);
        mPalette = nullptr;
        if (pPalette != nullptr) {
            mTlutName = a1;
            if (a1 != pPalette->mName) {
                pPalette->storeTLUT(a1, (GXTlutFmt)pPalette->mFormat, (JUTTransparency)pPalette->mTransparency, pPalette->mLutNum, pPalette->mColorTable);
            }
        }

        mWrapS = getTexInfo()->mWrapS;
        mWrapT = getTexInfo()->mWrapT;
        mMinType = getTexInfo()->mMinType;  // this matches... are the variable names swapped?
        mMagType = getTexInfo()->mMagType;
        mMinLod = (s8)getTexInfo()->mMinLod;
        mMaxLod = (s8)getTexInfo()->mMaxLod;
        mLodBias = (s16)getTexInfo()->mLodBias;
        init();
    }
}

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

void JUTTexture::initTexObj() {
    GXBool mipmapEnabled;
    if (mTIMG->mMipmap != 0) {
        mipmapEnabled = 1;
    } else {
        mipmapEnabled = 0;
    }
    u8* image = ((u8*)mTIMG);
    image += (mTIMG->mImageDataOffset ? mTIMG->mImageDataOffset : 0x20);
    GXInitTexObj(
        &mObj,
        image,
        mTIMG->mWidth,
        mTIMG->mHeight,
        (GXTexFmt)mTIMG->mFormat,
        (GXTexWrapMode)mWrapS,
        (GXTexWrapMode)mWrapT,
        mipmapEnabled);

    GXInitTexObjLOD(
        &mObj,
        (GXTexFilter)mMinType,
        (GXTexFilter)mMagType,
        mMinLod / 8.0f,
        mMaxLod / 8.0f,
        mLodBias / 100.0f,
        mTIMG->mBiasClamp,
        mTIMG->mDoEdgeLod,
        (GXAnisotropy)mTIMG->mMaxAnisotropy);
}

void JUTTexture::initTexObj(GXTlut lut) {
    GXBool mipmapEnabled;
    if (mTIMG->mMipmap != 0) {
        mipmapEnabled = 1;
    } else {
        mipmapEnabled = 0;
    }
    mTlutName = lut;
    u8* image = ((u8*)mTIMG);
    image += (mTIMG->mImageDataOffset ? mTIMG->mImageDataOffset : 0x20);
    GXInitTexObj(&mObj, image, mTIMG->mWidth, mTIMG->mHeight, (GXTexFmt)mTIMG->mFormat, (GXTexWrapMode)mWrapS, (GXTexWrapMode)mWrapT, mipmapEnabled);

    GXInitTexObjLOD(&mObj, (GXTexFilter)mMinType, (GXTexFilter)mMagType, mMinLod / 8.0f, mMaxLod / 8.0f, mLodBias / 100.0f, mTIMG->mBiasClamp,
                    mTIMG->mDoEdgeLod, (GXAnisotropy)mTIMG->mMaxAnisotropy);
}

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
