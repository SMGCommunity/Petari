#pragma once

#include "Game/System/FileHolder.hpp"
#include "Game/System/OSThreadWrapper.hpp"

struct RequestFileInfo {
    /* 0x00 */ u32 _0;
    /* 0x04 */ s32 mRequestType;
    /* 0x08 */ char mFileName[0x80];
    /* 0x88 */ volatile u32 _88;
    /* 0x8C */ FileHolderFileEntry* mFileEntry;
};

class FileLoaderThread : public OSThreadWrapper {
public:
    FileLoaderThread(int, int, JKRHeap*);

    virtual void* run();

    void loadToMainRAM(RequestFileInfo*);
    void mountArchiveAndStartCreateResource(RequestFileInfo*);
};
