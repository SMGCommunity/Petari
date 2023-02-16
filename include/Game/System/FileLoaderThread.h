#pragma once

#include "Game/System/OSThreadWrapper.h"
#include "Game/System/FileHolder.h"

struct RequestFileInfo {
    u32 _0;
    s32 mRequestType;                       // _4
    char mFileName[0x80];                  // _8
    u32 _88;
    FileHolderFileEntry* mFileEntry;        // _8C
};

class FileLoaderThread : OSThreadWrapper {
public:
    FileLoaderThread(int, int, JKRHeap *);

    virtual ~FileLoaderThread();
    virtual s32 run();

    void loadToMainRAM(RequestFileInfo *);
    void mountArchiveAndStartCreateResource(RequestFileInfo *);
};