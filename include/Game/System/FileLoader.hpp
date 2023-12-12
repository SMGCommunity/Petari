#pragma once

#include "revolution.h"
#include "Game/System/ArchiveHolder.h"
#include "Game/System/FileLoaderThread.h"
#include "Game/System/FileHolder.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include <cstdio>

class JKRArchive;

class FileLoader {
public:
    FileLoader();

    void requestLoadToMainRAM(const char *, u8 *, JKRHeap *, JKRDvdRipper::EAllocDirection, bool);
    void requestMountArchive(const char *, JKRHeap *, bool);
    bool isLoaded(const char *) const;
    bool isMountedArchive(const char *) const;
    void* receiveFile(const char *);
    JKRMemArchive* receiveArchive(const char *);
    void receiveAllRequestedFile();
    JKRMemArchive* createAndAddArchive(void *, JKRHeap *, const char *);
    void getMountedArchiveAndHeap(const char *, JKRArchive **, JKRHeap **) const;
    void clearRequestFileInfo(bool);
    void removeHolderIfIsEqualHeap(JKRHeap *);
    void removeFile(const char *);
    bool isNeedToLoad(const char *) const;
    const RequestFileInfo* getRequestFileInfoConst(const char *) const;
    RequestFileInfo* addRequest(const char *);

    FileLoaderThread* mLoaderThread;        // _0
    OSMutex mMutex;                         // _4
    RequestFileInfo* mRequestFileInfos;     // _1C
    s32 mRequestedFileCount;                // _20
    FileHolder* mFileHolder;                // _24
    ArchiveHolder* mArchiveHolder;          // _28
};