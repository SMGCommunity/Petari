#include "JSystem/JUtility/JUTTexture.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include <revolution.h>

JUTTexture::JUTTexture(const ResTIMG *pImgData, u8 a2) {
    _28 = 0;
    storeTIMG(pImgData, a2);
    _3B &= 2;
}

JUTTexture::JUTTexture(int width, int height, _GXTexFmt texFmt) {
    _3B = _3B & 2 | 1;
    u32 bufSize = GXGetTexBufferSize(width, height, texFmt, GX_FALSE, 1);

    ResTIMG* texBuf = reinterpret_cast<ResTIMG*>(new(0x20) u8[bufSize + 0x20]);
    _3C = texBuf;
    texBuf->mFormat = texFmt;
    texBuf->mEnableAlpha = 0;
    texBuf->mWidth = width;
    texBuf->mHeight = height;
    texBuf->mWrapS = 0;
    texBuf->mWrapT = 0;
    texBuf->mPaletteFormat = 0;
    texBuf->_9 = 0;
    texBuf->mPaletteCount = 0;
    texBuf->mPaletteOffs = 0;
    texBuf->_10 = 0;
    texBuf->_11 = 0;
    texBuf->_12 = 0;
    texBuf->_13 = 0;
    texBuf->mMagType = 1;
    texBuf->mMinType = 1;
    texBuf->_16 = 0;
    texBuf->_17 = 0;
    texBuf->mTotalImgCount = 1;
    texBuf->_1A = 0;
    texBuf->mImgDataOffs = 0x20;
    _28 = 0;

    // cast to u8 solves ambiguity
    storeTIMG(texBuf, (u8)0);
    DCFlushRange(_24, bufSize);
}

JUTTexture::~JUTTexture() {
    if (_3B & 0x1) {
        delete[] _3C;
    }

    if (_3B & 0x2) {
        delete _28;
    }
}

// JUTTexture::storeTIMG((ResTIMG const *, unsigned char))

void JUTTexture::storeTIMG(const ResTIMG *pImg, JUTPalette *pPalette) {
    _GXTlut tlut;

    if (!pPalette) {
        tlut = GX_TLUT0;
    }
    else {
        tlut = pPalette->_C;
    }

    storeTIMG(pImg, pPalette, tlut);
}

// JUTTexture::storeTIMG((ResTIMG const *, JUTPalette *, _GXTlut))

#ifdef NON_MATCHING
void JUTTexture::attachPalette(JUTPalette *pPalette) {
    JUTPalette* thing;
    if (_20->mPaletteFormat) {
        if (pPalette || (thing = _28) == 0) {
            mPalette = pPalette;
        }
        else {
            mPalette = thing;
        }

        initTexObj(mPalette->_C);
    }
}
#endif

void JUTTexture::init() {
    if (!_20->mPaletteCount) {
        initTexObj();
    }
    else {
        if (_28) {
            mPalette = _28;
            initTexObj(mPalette->_C);
        }
    }
}

/*void JUTTexture::initTexObj() {
    u32 offs = 0x20;
    if (_20->mImgDataOffs) {
        offs = _20->mImgDataOffs;
    }

    //GXInitTexObj(&mTexObj, reinterpret_cast<void*>(_20 + offs), _20->mWidth, _20->mHeight, (_GXTexFmt)_20->mFormat, (_GXTexWrapMode)_20->mWrapS, (_GXTexWrapMode)_20->mWrapT, _20->_10);
    //GXInitTexObjLOD(&mTexObj, (GXTexFilter)mMagType, (GXTexFilter)mMinType,  _34 * 0.125f, _36 * 0.125f, _38 / 100.0f, _20->_12, _20->_11, (GXAnisotropy)_20->_13);
}*/

// JUTTexture::initTexObj((_GXTlut))

void JUTTexture::load(_GXTexMapID texMapID) {
    if (mPalette) {
        mPalette->load();
    }

    GXLoadTexObj(&mTexObj, texMapID);
}

void JUTTexture::capture(int width, int height, _GXTexFmt texFmt, bool useMipmap, GXBool clear) {
    if (_3B & 0x1) {
        if (useMipmap) {
            GXSetTexCopySrc(width, height, (2 * _20->mWidth), (2 * _20->mHeight));
        }
        else {
            GXSetTexCopySrc(width, height, _20->mWidth, _20->mHeight);
        }

        GXSetTexCopyDst(_20->mWidth, _20->mHeight, texFmt, useMipmap);
        GXCopyTex(_24, clear);
        GXPixModeSync();
    }
}

void JUTTexture::setEmbPaletteDelFlag(bool set) {
    _3B = _3B & 0x1 | (2 * set);
}