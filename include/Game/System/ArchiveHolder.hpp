#pragma once

#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <revolution.h>

class ArchiveHolderArchiveEntry {
public:
    ArchiveHolderArchiveEntry(void *, JKRHeap *, const char *);
    ~ArchiveHolderArchiveEntry();

    JKRMemArchive* mArchive;        // 0x0
    JKRHeap* mHeap;                 // 0x4
    char* mArchiveName;             // 0x8
};

class ArchiveHolder {
public:
    ArchiveHolder();

    ArchiveHolderArchiveEntry** first() const {
        return mEntries;
    }

    ArchiveHolderArchiveEntry** last() const {
        return &mEntries[mCurEntryNum];
    }

    ArchiveHolderArchiveEntry* createAndAdd(void *, JKRHeap *, const char *);
    JKRMemArchive* getArchive(const char *) const;
    void getArchiveAndHeap(const char *, JKRArchive **, JKRHeap **) const;
    void removeIfIsEqualHeap(JKRHeap *);
    ArchiveHolderArchiveEntry* findEntry(const char *) const;

    ArchiveHolderArchiveEntry** mEntries;           // 0x0
    s32 mMaxEntries;                                // 0x4
    s32 mCurEntryNum;                               // 0x8
    OSMutex mMutex;                                 // 0xC
};