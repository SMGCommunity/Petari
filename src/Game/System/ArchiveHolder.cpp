#include "Game/System/ArchiveHolder.hpp"
#include "Game/Util.hpp"
#include <cstring>

ArchiveHolderArchiveEntry::ArchiveHolderArchiveEntry(void *pData, JKRHeap *pHeap, const char *pName) {
    mHeap = pHeap;
    mArchive = nullptr;
    mArchiveName = nullptr;
    JKRMemArchive* archive = new(pHeap, 0) JKRMemArchive();
    archive->mountFixed(pData, JKR_MEM_BREAK_FLAG_0);
    mArchive = archive;
    s32 len = strlen(pName) + 1;
    char* name = new(pHeap, 0) char[len];
    mArchiveName = name;
    MR::copyString(mArchiveName, pName, len);
}

ArchiveHolderArchiveEntry::~ArchiveHolderArchiveEntry() {
    mArchive->unmount();
    delete mArchiveName;
}

ArchiveHolder::ArchiveHolder() {
    mEntries = nullptr;
    mMaxEntries = 0;
    mCurEntryNum = 0;
    mEntries = new ArchiveHolderArchiveEntry*[0x180];
    mMaxEntries = 0x180;
    OSInitMutex(&mMutex);
}

ArchiveHolderArchiveEntry* ArchiveHolder::createAndAdd(void *pData, JKRHeap *pHeap, const char *pName) {
    ArchiveHolderArchiveEntry* entry = new(pHeap, 0) ArchiveHolderArchiveEntry(pData, pHeap, pName);
    OSMutex* mutex = &mMutex;
    OSLockMutex(mutex);
    s32 num = mCurEntryNum;
    mCurEntryNum = num + 1;
    mEntries[num] = entry;
    OSUnlockMutex(mutex);
    return entry;
}

JKRMemArchive* ArchiveHolder::getArchive(const char *pName) const {
    ArchiveHolderArchiveEntry* entry = findEntry(pName);
    return (entry != nullptr) ? entry->mArchive : nullptr;
}

void ArchiveHolder::getArchiveAndHeap(const char *pName, JKRArchive** pArchive, JKRHeap** pHeap) const {
    ArchiveHolderArchiveEntry* entry = findEntry(pName);

    if (entry != nullptr) {
        *pArchive = entry->mArchive;
        *pHeap = entry->mHeap;
    }
}

// void ArchiveHolder::removeIfIsEqualHeap(JKRHeap *pHeap)

ArchiveHolderArchiveEntry* ArchiveHolder::findEntry(const char *pName) const {
    for (ArchiveHolderArchiveEntry** i = first(); i != last(); i++) {
        if (MR::isEqualStringCase((*i)->mArchiveName, pName)) {
            return *i;
        }
    }

    return nullptr;
}