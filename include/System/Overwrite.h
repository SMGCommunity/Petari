#ifndef OVERWRITE_H
#define OVERWRITE_H

#include <revolution.h>

namespace
{
    static s32 sAudioThreadMsgSize      = 0x20;
    static u32 sAudioThreadStackSize    = 0x8000;
    static u32 sAramThreadStackSize     = 0xC000;
    static s32 sAramThreadMsgSize       = 0x20;
};

#endif // OVERWRITE_H