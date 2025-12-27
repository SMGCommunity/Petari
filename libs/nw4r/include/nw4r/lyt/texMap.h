#pragma once

#include "nw4r/lyt/types.h"
#include "revolution/tpl.h"
#include <revolution.h>

namespace nw4r {
    namespace lyt {
        class TexMap {
        public:
            TexMap() {
                SetImage(NULL);
                SetSize(0, 0);
                SetTexelFormat(GX_TF_I4);
                SetWrapMode(GX_CLAMP, GX_CLAMP);
                SetMipMap(false);
                SetFilter(GX_LINEAR, GX_LINEAR);
                SetLOD(0.0f, 0.0f);
                SetLODBias(0.0f);
                SetBiasClampEnable(false);
                SetEdgeLODEnable(false);
                SetAnisotropy(GX_ANISO_1);
                SetPalette(NULL);
                SetPaletteFormat(GX_TL_IA8);
                SetPaletteEntryNum(0);
            }

            void SetImage(void* pImage) { mImage = pImage; }

            void SetAnisotropy(GXAnisotropy aniso) { mBits.anisotropy = aniso; }

            void Get(_GXTexObj*) const;
            void Get(_GXTlutObj*) const;
            void Set(const _GXTexObj&);
            void ReplaceImage(const TPLDescriptor*);
            void ReplaceImage(TPLPalette*, u32);

            inline GXTexFmt GetTexelFormat() const { return GXTexFmt(mBits.textureFormat); }

            inline GXTexWrapMode GetWrapModeS() const { return GXTexWrapMode(mBits.wrapS); }

            inline GXTexWrapMode GetWrapModeT() const { return GXTexWrapMode(mBits.wrapT); }

            inline GXTexFilter GetMinFilter() const { return GXTexFilter(mBits.minFilter); }

            inline GXTexFilter GetMagFilter() const { return GXTexFilter(mBits.magFilter); }

            inline bool IsBiasClampEnable() const { return 0 != mBits.biasClampEnable; }

            inline bool IsEdgeLODEnable() const { return 0 != mBits.edgeLODEnable; }

            inline GXAnisotropy GetAnisotropy() const { return GXAnisotropy(mBits.anisotropy); }

            inline f32 GetLODBias() const { return mLODBias / 256.0f; }

            inline f32 GetMinLOD() const { return mMinLOD; }

            inline f32 GetMaxLOD() const { return mMaxLOD; }

            inline bool IsMipMap() const { return 0 != mBits.mipmap; }

            inline void* GetPalette() const { return mPalette; }

            inline GXTlutFmt GetPaletteFormat() const { return GXTlutFmt(mBits.paletteFormat); }

            inline u16 GetPaletteEntryNum() const { return mPltEntryNum; }

            void SetMipMap(bool mipmap) { mBits.mipmap = mipmap; }

            void SetLODBias(f32 bias) { mLODBias = u16(bias * 256); }

            void SetWrapMode(GXTexWrapMode wrapS, GXTexWrapMode wrapT) {
                mBits.wrapS = wrapS;
                mBits.wrapT = wrapT;
            }

            void SetFilter(GXTexFilter minFlt, GXTexFilter magFlt) {
                mBits.minFilter = minFlt;
                mBits.magFilter = magFlt;
            }

            void SetLOD(f32 minLOD, f32 maxLOD) {
                mMinLOD = minLOD;
                mMaxLOD = maxLOD;
            }

            void SetBiasClampEnable(bool enable) { mBits.biasClampEnable = enable; }

            void SetEdgeLODEnable(bool enable) { mBits.edgeLODEnable = enable; }

            void SetSize(u16 width, u16 height) {
                mWidth = width;
                mHeight = height;
            }

            void SetTexelFormat(GXTexFmt value) { mBits.textureFormat = value; }

            void SetPalette(void* pPalette) { mPalette = pPalette; }

            void SetPaletteFormat(GXTlutFmt value) { mBits.paletteFormat = value; }

            void SetPaletteEntryNum(u16 value) { mPltEntryNum = value; }

            void Set(const TexMap& texMap) { *this = texMap; }

            const Size GetSize() const { return Size(mWidth, mHeight); }

            void* mImage;      // 0x00
            void* mPalette;    // 0x04
            u16 mWidth;        // 0x08
            u16 mHeight;       // 0x0A
            f32 mMinLOD;       // 0x0C
            f32 mMaxLOD;       // 0x10
            u16 mLODBias;      // 0x14
            u16 mPltEntryNum;  // 0x16

            struct {
                u32 textureFormat : 4;
                u32 mipmap : 1;
                u32 wrapS : 2;
                u32 wrapT : 2;
                u32 minFilter : 3;
                u32 magFilter : 1;
                u32 biasClampEnable : 1;
                u32 edgeLODEnable : 1;
                u32 anisotropy : 2;
                u32 paletteFormat : 2;
            } mBits;  // 0x18
        };
    };  // namespace lyt
};  // namespace nw4r
