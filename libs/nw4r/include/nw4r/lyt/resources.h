#pragma once

#include <revolution.h>
#include "nw4r/lyt/types.h"
#include "nw4r/math/types.h"

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

            struct Bounding : Pane {

            };

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
        };
    };
};
