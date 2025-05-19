#include "Game/System/FileRipper.hpp"

namespace {
    u32 sReadBufferLimit;
    u32 sReadDvdLeftSize;
}

s32 FileRipper::checkCompressed(const u8 *pData)
{
    if (pData[0] == 'Y' && pData[1] == 'a' && pData[3] == '0') {
        if (pData[2] == 'y') {
            return 1;
        }

        if (pData[2] == 'z') {
            return 2;
        }
    }

    return 0;
}

bool FileRipper::decompressSzsSub(u8 *src, u8 *dest) {
    u8 *copy_src;
    u32 group_count = 0;
    u32 group_head = 0;
    
    if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0') {
        return false;
    }
    
    u32 data_len = *(u32 *)(src + 4);
    u8 *dest_end = dest + data_len;
    src += 0x10;

    do {
        if (!group_count) {
            if ((u32)src > sReadBufferLimit && sReadDvdLeftSize) {
                if (!(src = readSrcDataNext(src))) {
                    return false;
                }
            }
            group_head = *src;
            group_count = 8;
            src = src + 1;
        }

        if (group_head & 0x80) {
            u8 b = *src++;
            *dest++ = b;
            if (dest == dest_end) {
                break;
            }
        }
        else {
            u8 b1 = *src++;
            u8 b2 = *src++;

            u32 offset = ((b1 & 0x0f) << 8);
            offset |= b2;

            copy_src = dest - offset - 1;
            u32 copy_len = b1 >> 4;
            if (copy_len == 0) {
                copy_len = *src++ + 0x12;
            }
            else {
                copy_len += 2;
            }
            do {
                *dest++ = *copy_src;
                if (dest == dest_end) {
                    break;
                }
                ++copy_src;
                --copy_len;
            } while (copy_len != 0);
        }
        group_head <<= 1;
        --group_count;
    } while (dest < dest_end);
    
    return true;
}
