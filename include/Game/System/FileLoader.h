#pragma once

#include "revolution.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRDvdRipper.h"

class FileLoader {
public:
    void* receiveFile(const char *);
    void* receiveArchive(const char *);

    void requestMountArchive(const char *, JKRHeap *, bool);
    void requestLoadToMainRAM(const char *, u8 *, JKRHeap *, JKRDvdRipper::EAllocDirection, bool);
};