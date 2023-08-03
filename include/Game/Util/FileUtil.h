#pragma once

#include "revolution.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRDvdRipper.h"

class JKRArchive;

namespace MR { 
    bool isFileExist(const char *, bool);
    u32 getFileSize(const char *, bool);
    s32 convertPathToEntrynumConsideringLanguage(const char *);
    void* loadToMainRAM(const char *, u8 *, JKRHeap *, JKRDvdRipper::EAllocDirection);
    void loadAsyncToMainRAM(const char *, u8 *, JKRHeap *, JKRDvdRipper::EAllocDirection);
    void* mountArchive(const char *, JKRHeap *);
    void mountAsyncArchive(const char *, JKRHeap *);
    void mountAsyncArchiveByObjectOrLayoutName(const char *, JKRHeap *);
    void* receiveFile(const char *);
    void* receiveArchive(const char *);
    void receiveAllRequestedFile();
    void* createAndAddArchive(void *, JKRHeap *, const char *);
    void getMountedArchiveAndHeap(const char *, JKRArchive **, JKRHeap **);
    void removeFileConsideringLanguage(const char *);
    void removeResourceAndFileHolderIfIsEqualHeap(JKRHeap *);
    void decompressFileFromArchive(JKRArchive *, const char *, JKRHeap *, int);
    bool isLoadedFile(const char *);
    bool isMountedArchive(const char *);
    bool isLoadedObjectOrLayoutArchive(const char *);
    void makeFileNameConsideringLanguage(char *, u32, const char *);
    bool makeObjectArchiveFileName(char *, u32, const char *);
    bool makeObjectArchiveFileNameFromPrefix(char *, u32, const char *, bool);
    bool makeLayoutArchiveFileName(char *, u32, const char *);
    bool makeLayoutArchiveFileNameFromPrefix(char *, u32, const char *, bool);
    void makeScenarioArchiveFileName(char *, u32, const char *);
};
