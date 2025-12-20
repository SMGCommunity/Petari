#pragma once

#include "nw4r/lyt/types.h"
#include "nw4r/math/types.h"
#include <revolution.h>

namespace nw4r {
    namespace lyt {
        namespace res {
            struct BinaryFileHeader {
                char signature[4];
                u16 byteOrder;
                u16 version;
                u32 fileSize;
                u16 headerSize;
                u16 dataBlocks;
            };

            struct DataBlockHeader {
                char kind[4];
                u32 size;
            };

            struct Layout {
                DataBlockHeader blockHeader;
                u8 originType;
                u8 padding[3];
                Size layoutSize;
            };

            struct Font {
                u32 nameStrOffset;
                u8 type;
                u8 padding[3];
            };

            struct FontList {
                DataBlockHeader blockHeader;
                u16 fontNum;
                u8 padding[2];
            };

            struct Texture {
                u32 nameStrOffset;
                u8 type;
                u8 padding[3];
            };

            struct TextureList {
                DataBlockHeader blockHeader;
                u16 texNum;
                u8 padding[2];
            };

            struct TexMap {
                TexMap() : texIdx(0), wrapSflt(0), wrapTflt(0) {}

                GXTexWrapMode GetWarpModeS() const { return GXTexWrapMode(detail::GetBits(wrapSflt, 0, 2)); }
                GXTexWrapMode GetWarpModeT() const { return GXTexWrapMode(detail::GetBits(wrapTflt, 0, 2)); }

                GXTexFilter GetMinFilter() const {
                    const int bitLen = 3;
                    const u8 bitData = detail::GetBits(wrapSflt, 2, bitLen);
                    return GXTexFilter(detail::GetBits(bitData + GX_LINEAR, 0, bitLen));
                }

                GXTexFilter GetMagFilter() const {
                    const int bitLen = 1;
                    const u8 bitData = detail::GetBits(wrapTflt, 2, bitLen);
                    return GXTexFilter(detail::GetBits(bitData + GX_LINEAR, 0, bitLen));
                }

                void SetWarpModeS(GXTexWrapMode value) { detail::SetBits(&wrapSflt, 0, 2, u8(value)); }
                void SetWarpModeT(GXTexWrapMode value) { detail::SetBits(&wrapTflt, 0, 2, u8(value)); }

                void SetMinFilter(GXTexFilter value) {
                    const int bitLen = 3;
                    const u8 bitData = u8(detail::GetBits(value - GX_LINEAR, 0, bitLen));  // Set to zero when (value = GX_LINEAR)
                    detail::SetBits(&wrapSflt, 2, bitLen, bitData);
                }

                void SetMagFilter(GXTexFilter value) {
                    const int bitLen = 1;
                    const u8 bitData = u8(detail::GetBits(value - GX_LINEAR, 0, bitLen));
                    detail::SetBits(&wrapTflt, 2, bitLen, bitData);
                }

                u16 texIdx;
                u8 wrapSflt;
                u8 wrapTflt;
            };

            struct MaterialList {
                DataBlockHeader blockHeader;
                u16 materialNum;
                u8 padding[2];
            };

            struct Pane {
                DataBlockHeader blockHeader;
                u8 flag;
                u8 basePosition;
                u8 alpha;
                u8 padding;
                char name[16];
                char userData[8];
                math::VEC3 translate;
                math::VEC3 rotate;
                math::VEC2 scale;
                Size size;
            };

            struct Picture : Pane {
                u32 vtxCols[4];
                u16 materialIdx;
                u8 texCoordNum;
                u8 padding[1];
            };

            struct TextBox : Pane {
                u16 textBufBytes;
                u16 textStrBytes;
                u16 materialIdx;
                u16 fontIdx;
                u8 textPosition;
                u8 textAlignment;
                u8 padding[2];
                u32 textStrOffset;
                u32 textCols[2];
                Size fontSize;
                f32 charSpace;
                f32 lineSpace;
            };

            struct WindowFrame {
                u16 materialIdx;
                u8 textureFlip;
                u8 padding1;
            };

            struct WindowContent {
                u32 vtxCols[4];
                u16 materialIdx;
                u8 texCoordNum;
                u8 padding[1];
            };

            struct Window : Pane {
                InflationLRTB inflation;
                u8 frameNum;
                u8 padding1;
                u8 padding2;
                u8 padding3;
                u32 contentOffset;
                u32 frameOffsetTableOffset;
            };

            struct Bounding : Pane {};

            struct ExtUserDataList {
                DataBlockHeader blockHeader;
                detail::ResU16 num;
                u8 padding[2];
            };

            struct Group {
                DataBlockHeader blockHeader;
                char name[16];
                u16 paneNum;
                u8 padding[2];
            };

            struct MaterialResourceNum {
                u8 GetTexMapNum() const NO_INLINE { return u8(detail::GetBits(bits, 0, 4)); }
                u8 GetTexSRTNum() const NO_INLINE { return u8(detail::GetBits(bits, 4, 4)); }
                u8 GetTexCoordGenNum() const NO_INLINE { return u8(detail::GetBits(bits, 8, 4)); }
                bool HasTevSwapTable() const NO_INLINE { return detail::TestBit(bits, 12); }
                u8 GetIndTexSRTNum() const NO_INLINE { return u8(detail::GetBits(bits, 13, 2)); }
                u8 GetIndTexStageNum() const NO_INLINE { return u8(detail::GetBits(bits, 15, 3)); }
                u8 GetTevStageNum() const NO_INLINE { return u8(detail::GetBits(bits, 18, 5)); }
                bool HasAlphaCompare() const NO_INLINE { return detail::TestBit(bits, 23); }
                bool HasBlendMode() const NO_INLINE { return detail::TestBit(bits, 24); }
                u8 GetChanCtrlNum() const NO_INLINE { return u8(detail::GetBits(bits, 25, 1)); }
                u8 GetMatColNum() const NO_INLINE { return u8(detail::GetBits(bits, 27, 1)); }

                u32 bits;
            };

            struct Material {
                char name[20];
                GXColorS10 tevCols[3];
                GXColor tevKCols[4];
                MaterialResourceNum resNum;
            };

            struct AnimationTagBlock {
                DataBlockHeader blockHeader;
                detail::ResU16 tagOrder;
                detail::ResU16 groupNum;
                detail::ResU32 nameOffset;
                detail::ResU32 groupsOffset;
                detail::ResS16 startFrame;
                detail::ResS16 endFrame;
                u8 flag;
                u8 padding[3];
            };

            struct AnimationShareBlock {
                DataBlockHeader blockHeader;
                detail::ResU32 animShareInfoOffset;
                detail::ResU16 shareNum;
                u8 padding[2];
            };

            struct AnimationBlock {
                DataBlockHeader blockHeader;
                u16 frameSize;
                u8 loop;
                u8 padding1;
                u16 fileNum;
                u16 animContNum;
                u32 animContOffsetsOffset;
            };

            struct HermiteKey {
                f32 frame;
                f32 value;
                f32 slope;
            };

            struct StepKey {
                f32 frame;
                u16 value;
                u16 pad;
            };
        };  // namespace res
    };  // namespace lyt
};  // namespace nw4r
