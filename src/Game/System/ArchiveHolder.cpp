#include "Game/System/ArchiveHolder.hpp"
#include "Game/Util.hpp"
#include <cstring>

ArchiveHolderArchiveEntry::ArchiveHolderArchiveEntry(void *pData, JKRHeap *pHeap, const char *pArchiveName) :
    mHeap(pHeap),
    mArchive(nullptr),
    mArchiveName(nullptr)
{
    JKRMemArchive *archive = new(pHeap, 0) JKRMemArchive();
    archive->mountFixed(pData, JKR_MEM_BREAK_FLAG_0);
    mArchive = archive;
    s32 len = strlen(pArchiveName) + 1;
    mArchiveName = new(pHeap, 0) char[len];
    MR::copyString(mArchiveName, pArchiveName, len);
}

ArchiveHolderArchiveEntry::~ArchiveHolderArchiveEntry() {
    mArchive->unmount();
    delete mArchiveName;
}

ArchiveHolder::ArchiveHolder() :
    mEntries(nullptr),
    mMaxEntries(0),
    mCurEntryNum(0)
{
    mEntries = new ArchiveHolderArchiveEntry*[0x180];
    mMaxEntries = 0x180;
    OSInitMutex(&mMutex);
}

ArchiveHolderArchiveEntry *ArchiveHolder::createAndAdd(void *pData, JKRHeap *pHeap, const char *pArchiveName) {
    ArchiveHolderArchiveEntry *entry = new(pHeap, 0) ArchiveHolderArchiveEntry(pData, pHeap, pArchiveName);
    OSMutex *mutex = &mMutex;
    OSLockMutex(mutex);
    s32 num = mCurEntryNum;
    mCurEntryNum = num + 1;
    mEntries[num] = entry;
    OSUnlockMutex(mutex);
    return entry;
}

JKRMemArchive* ArchiveHolder::getArchive(const char *pArchiveName) const {
    ArchiveHolderArchiveEntry *entry = findEntry(pArchiveName);
    return (entry != nullptr) ? entry->mArchive : nullptr;
}

void ArchiveHolder::getArchiveAndHeap(const char *pArchiveName, JKRArchive **pArchive, JKRHeap **pHeap) const {
    ArchiveHolderArchiveEntry *entry = findEntry(pArchiveName);

    if (entry != nullptr) {
        *pArchive = entry->mArchive;
        *pHeap = entry->mHeap;
    }
}

void ArchiveHolder::removeIfIsEqualHeap(JKRHeap *pHeap) {
    if (pHeap == nullptr) {
        return;
    }

    for (ArchiveHolderArchiveEntry **i = first(); i != last(); ) {
        if ((*i)->mHeap == pHeap
            || MR::getHeapNapa((*i)->mHeap) == pHeap
            || MR::getHeapGDDR3((*i)->mHeap) == pHeap)
        {
            delete *i;

            if (last() - i - 1 > 0) {
                for (ArchiveHolderArchiveEntry **j = i; j + 1 != last(); j++) {
                    *j = *(j + 1);
                }
            }

            mCurEntryNum--;
        } else {
            i++;
        }
    }
}

ArchiveHolderArchiveEntry *ArchiveHolder::findEntry(const char *pArchiveName) const {
    for (ArchiveHolderArchiveEntry **i = first(); i != last(); i++) {
        if (MR::isEqualStringCase((*i)->mArchiveName, pArchiveName)) {
            return *i;
        }
    }

    return nullptr;
}
