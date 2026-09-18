#pragma once

#include <revolution/nwc24.h>
#include <revolution/types.h>

class JKRHeap;
class NWC24SendThread;

class NWC24System {
public:
    NWC24System(JKRHeap*, s32);

    bool open(NWC24Err*, s32*);
    bool close(NWC24Err*);
    bool send(const u16*, const u16*, const u8*, u32, const u8*, u32, u16, bool, u8);
    bool isSent(NWC24Err*, u32*);

public:
    /* 0x0 */ bool _0;
    /* 0x4 */ u8* mWorkBuffer;
    /* 0x8 */ u8* mVFWorkBuffer;
    /* 0xC */ NWC24SendThread* mSendThread;
};
