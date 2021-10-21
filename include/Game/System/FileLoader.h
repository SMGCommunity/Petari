#pragma once

#include "revolution.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRDvdRipper.h"

class JKRArchive;

class FileLoader {
public:
    void* receiveFile(const char *);
    void* receiveArchive(const char *);

    void requestMountArchive(const char *, JKRHeap *, bool);
    void requestLoadToMainRAM(const char *, u8 *, JKRHeap *, JKRDvdRipper::EAllocDirection, bool);

    void receiveAllRequestedFile();
    void* createAndAddArchive(void *, JKRHeap *, const char *);

    void getMountedArchiveAndHeap(const char *, JKRArchive **, JKRHeap **);
    void removeFile(const char *);

    void removeHolderIfIsEqualHeap(JKRHeap *);

    bool isLoaded(const char *) const;
    bool isMountedArchive(const char *) const;
};