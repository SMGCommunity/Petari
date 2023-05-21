
#pragma once

#include <revolution.h>
#include "nw4r/lyt/types.h"

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

            struct MaterialResourceNum {
                MaterialResourceNum() : mBits(0) {

                }

                u32 mBits;
            };

            struct Material {
                char name[20];
                GXColorS10 tevCols[3];
                GXColor tevKCols[4];
                MaterialResourceNum resNum;
            };

            struct MaterialList {
                DataBlockHeader blockHeader;
                u16 materialNum;
                u16 pad;
            };

            struct Layout {
                DataBlockHeader blockHeader;
                u8 originType;
                u8 pad[3];
                Size layoutSize;
            };

            struct Font {
                u32 nameStrOffset;
                u8 type;
                u8 pad[3];
            };

            struct FontList {
                DataBlockHeader blockHeader;
                u16 fontNum;
                u8 pad[2];
            };

            struct Texture {
                u32 nameStrOffset;
                u8 type;
                u8 pad[3];
            };

            struct TextureList {
                DataBlockHeader blockHeader;
                u16 texNum;
                u8 pad[2];
            };

            struct TexMap {
                TexMap() {
                    texIdx = 0;
                    wrapSflt = 0;
                    wrapTflt = 0;
                }

                u16 texIdx;
                u8 wrapSflt;
                u8 wrapTflt;
            };

            struct Pane {
                DataBlockHeader blockHeader;
                u8 flag;
                u8 basePosition;
                u8 alpha;
                u8 pad;
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
                u8 pad;
            };

            struct TextBox : Pane {
                u16 textBufBytes;
                u16 textStrBytes;
                u16 materialIdx;
                u16 fontIdx;
                u8 textPosition;
                u8 textAlignment;
                u16 pad;
                u32 textStrOffset;
                u32 textCols[2];
                Size fontSize;
                f32 charSpace;
                f32 lineSpace;
            };

            struct WindowFrame {
                u16 materialIdx;
                u8 textureFlip;
                u8 pad;
            };

            struct WindowContent {
                u32 vtxCols[4];
                u16 materialIdx;
                u8 texCoordNum;
                u8 pad;
            };

            struct Window : Pane {
                InflationLRTB inflation;
                u8 frameNum;
                u8 pad[3];
                u32 contentOffset;
                u32 frameOffsetTableOffset;
            };

            struct Bounding : Pane {

            };

            struct Group {
                DataBlockHeader blockHeader;
                char name[16];
                u16 paneNum;
                u16 pad;
            };
        };
    };
};