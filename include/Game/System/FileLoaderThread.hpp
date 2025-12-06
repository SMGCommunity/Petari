#pragma once

#include "Game/System/FileHolder.hpp"
#include "Game/System/OSThreadWrapper.hpp"

struct RequestFileInfo {
    u32 _0;
    s32 mRequestType;      // 0x4
    char mFileName[0x80];  // 0x8
    u32 _88;
    FileHolderFileEntry* mFileEntry;  // 0x8C
};

class FileLoaderThread : public OSThreadWrapper {
public:
    FileLoaderThread(int, int, JKRHeap*);

    virtual void* run();

    void loadToMainRAM(RequestFileInfo*);
    void mountArchiveAndStartCreateResource(RequestFileInfo*);
};