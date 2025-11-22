#pragma once

#include "Game/System/ArchiveHolder.hpp"
#include "Game/System/FileHolder.hpp"
#include "Game/System/FileLoaderThread.hpp"
#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "revolution.h"
#include <cstdio>

class JKRArchive;

class FileLoader {
public:
    FileLoader();

    void requestLoadToMainRAM(const char*, u8*, JKRHeap*, JKRDvdRipper::EAllocDirection, bool);
    void requestMountArchive(const char*, JKRHeap*, bool);
    bool isLoaded(const char*) const;
    bool isMountedArchive(const char*) const;
    void* receiveFile(const char*);
    JKRMemArchive* receiveArchive(const char*);
    void receiveAllRequestedFile();
    JKRMemArchive* createAndAddArchive(void*, JKRHeap*, const char*);
    void getMountedArchiveAndHeap(const char*, JKRArchive**, JKRHeap**) const;
    void clearRequestFileInfo(bool);
    void removeHolderIfIsEqualHeap(JKRHeap*);
    void removeFile(const char*);
    bool isNeedToLoad(const char*) const;
    const RequestFileInfo* getRequestFileInfoConst(const char*) const;
    RequestFileInfo* addRequest(const char*);

    FileLoaderThread* mLoaderThread;     // 0x0
    OSMutex mMutex;                      // 0x4
    RequestFileInfo* mRequestFileInfos;  // 0x1C
    s32 mRequestedFileCount;             // 0x20
    FileHolder* mFileHolder;             // 0x24
    ArchiveHolder* mArchiveHolder;       // 0x28
};