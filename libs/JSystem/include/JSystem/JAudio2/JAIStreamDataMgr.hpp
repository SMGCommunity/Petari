#pragma once

#include "JSystem/JAudio2/JAISound.hpp"

struct JAIStreamDataMgr {
    virtual s32 getStreamFileEntry(JAISoundID) = 0;
    virtual ~JAIStreamDataMgr();
};

struct JAIStreamAramMgr {
    virtual void* newStreamAram(u32*) = 0;
    virtual bool deleteStreamAram(u32) = 0;
    virtual ~JAIStreamAramMgr();
};
