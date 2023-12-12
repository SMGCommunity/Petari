#pragma once

#include <revolution.h>
#include <revolution/tpl.h>
#include "nw4r/lyt/types.h"

namespace nw4r {
    namespace lyt {
        class TexMap {
        public:

            TexMap() {
                SetImage(0);
                SetSize(0, 0);
                SetTexelFormat(GXTexFmt(0));
                SetWrapMode(GX_CLAMP, GX_CLAMP);
                SetMipMap(false);
                SetFilter(GX_LINEAR, GX_LINEAR);
                SetLOD(0, 0);
                SetLODBias(0);
                SetBiasClampEnable(false);
                SetEdgeLODEnable(false);
                SetAnisotropy(GX_ANISO_1);
                SetPalette(0);
                SetPaletteFormat(GXTlutFmt(0));
                SetPaletteEntryNum(0);
            }

            void Get(GXTexObj *) const;
            void Get(GXTlutObj *) const;
            void Set(const GXTexObj &);
            void ReplaceImage(TPLPalettePtr, u32);

            void* GetImage() const {
                return mpImage;
            }

            u16 GetWidth() const {
                return mWidth;
            }

            u16 GetHeight() const {
                return mHeight;
            }

            const Size GetSize() const {
                return Size(mWidth, mHeight);
            }

            GXTexFmt GetTexelFormat() const {
                return GXTexFmt(mBits.textureFormat);
            }

            void* GetPalette() const {
                return mpPalette;
            }

            GXTlutFmt GetPaletteFormat() const {
                return GXTlutFmt(mBits.paletteFormat);
            }

            u16 GetPaletteEntryNum() const {
                return mPaletteEntryNum;
            }

            bool IsMipMap() const {
                return mBits.mipmap != 0;
            }

            GXTexWrapMode GetWrapModeS() const {
                return GXTexWrapMode(mBits.wrapS);
            }

            GXTexWrapMode GetWrapModeT() const {
                return GXTexWrapMode(mBits.wrapT);
            }

            GXTexFilter GetMinFilter() const {
                return GXTexFilter(mBits.minFilter);
            }

            GXTexFilter GetMagFilter() const {
                return GXTexFilter(mBits.magFilter);
            }

            f32 GetMinLOD() const {
                return mMinLOD;
            }

            f32 GetMaxLOD() const {
                return mMaxLOD;
            }

            f32 GetLODBias() const {
                return mLODBias / 256.0f;
            }

            bool IsBiasClampEnable() const {
                return mBits.biasClampEnable != 0;
            }

            bool IsEdgeLODEnable() const {
                return mBits.edgeLODEnable != 0;
            }

            GXAnisotropy GetAnisotropy() const {
                return GXAnisotropy(mBits.anisotropy);
            }

            void SetImage(void *pImage) {
                mpImage = pImage;
            }

            void SetSize(u16 width, u16 height) {
                mWidth = width;
                mHeight = height;
            }

            void SetTexelFormat(GXTexFmt fmt) {
                mBits.textureFormat = fmt;
            }

            void SetMipMap(bool mipmap) {
                mBits.mipmap = mipmap;
            }

            void SetPalette(void *pPalette) {
                mpPalette = pPalette;
            }

            void SetPaletteFormat(GXTlutFmt fmt) {
                mBits.paletteFormat = fmt;
            }

            void SetPaletteEntryNum(u16 val) {
                mPaletteEntryNum = val;
            }

            void SetWrapMode(GXTexWrapMode wrapS, GXTexWrapMode wrapT) {
                mBits.wrapS = wrapS;
                mBits.wrapT = wrapT;
            }

            void SetFilter(GXTexFilter minFlt, GXTexFilter magFlt) {
                mBits.minFilter = minFlt;
                mBits.magFilter = magFlt;
            }

            void SetLOD(f32 min, f32 max) {
                mMinLOD = min;
                mMaxLOD = max;
            }

            void SetLODBias(f32 bias) {
                mLODBias = u16(bias * 256);
            }

            void SetBiasClampEnable(bool en) {
                mBits.biasClampEnable = en;
            }

            void SetEdgeLODEnable(bool en) {
                mBits.edgeLODEnable = en;
            }

            void SetAnisotropy(GXAnisotropy an) {
                mBits.anisotropy = an;
            }

            inline void ReplaceImage(const TPLDescriptor *);

            void* mpImage;              // _0
            void* mpPalette;            // _4
            u16 mWidth;                 // _8
            u16 mHeight;                // _A
            f32 mMinLOD;                // _C
            f32 mMaxLOD;                // _10
            u16 mLODBias;               // _14
            u16 mPaletteEntryNum;       // _16

            struct {
                u32 textureFormat   : 4;
                u32 mipmap          : 1;
                u32 wrapS           : 2;
                u32 wrapT           : 2;
                u32 minFilter       : 3;
                u32 magFilter       : 1;
                u32 biasClampEnable : 1;
                u32 edgeLODEnable   : 1;
                u32 anisotropy      : 2;
                u32 paletteFormat   : 2;
            } mBits;
        };
    };
};