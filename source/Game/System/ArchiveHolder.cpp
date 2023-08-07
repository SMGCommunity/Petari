#include "Game/System/ArchiveHolder.h"
#include "Game/Util.h"
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