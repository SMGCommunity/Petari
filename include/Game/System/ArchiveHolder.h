#pragma once

#include <JSystem/JKernel/JKRMemArchive.h>

class ArchiveHolderArchiveEntry {
public:
    ArchiveHolderArchiveEntry(void *, JKRHeap *, const char *);
    ~ArchiveHolderArchiveEntry();

    JKRMemArchive* mArchive;        // _0
    JKRHeap* mHeap;                 // _4
    char* mArchiveName;             // _8
};