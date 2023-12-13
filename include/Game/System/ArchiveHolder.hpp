#pragma once

#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <revolution.h>

class ArchiveHolderArchiveEntry {
public:
    ArchiveHolderArchiveEntry(void *, JKRHeap *, const char *);
    ~ArchiveHolderArchiveEntry();

    JKRMemArchive* mArchive;        // _0
    JKRHeap* mHeap;                 // _4
    char* mArchiveName;             // _8
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

    ArchiveHolderArchiveEntry** mEntries;           // _0
    s32 mMaxEntries;                                // _4
    s32 mCurEntryNum;                               // _8
    OSMutex mMutex;                                 // _C
};