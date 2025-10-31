#include "Game/System/FileHolder.hpp"
#include "Game/Util.hpp"

#define FILE_HOLDER_STATE_NONE      0
#define FILE_HOLDER_STATE_READING   1
#define FILE_HOLDER_STATE_DONE      2

FileHolderFileEntry::FileHolderFileEntry(const char *pFilePath, JKRHeap *pHeap, void *pContext) :
    mEntryNum(DVDConvertPathToEntrynum(pFilePath)),
    mContext(nullptr),
    mHeap(pHeap),
    mState(FILE_HOLDER_STATE_NONE),
    mContextSet(true)
{
    OSInitMessageQueue(&mQueue, &mMessage, 1);

    if (pContext) {
        mContextSet = false;
        mContext = pContext;
    }
}

FileHolderFileEntry::~FileHolderFileEntry() {
    if (mContextSet && (mContext != nullptr)) {
        delete (u8*)mContext;
    }
}

void FileHolderFileEntry::waitReadDone() {
    if (mState != FILE_HOLDER_STATE_DONE) {
        OSMessage msg;
        OSReceiveMessage(&mQueue, &msg, OS_MESSAGE_BLOCK);
        mState = FILE_HOLDER_STATE_DONE;
    }
}

void FileHolderFileEntry::setContext(void *pContext, JKRHeap *pHeap) {
    mContext = pContext;
    mHeap = pHeap;

    OSSendMessage(&mQueue, nullptr, OS_MESSAGE_NOBLOCK);
    mState = FILE_HOLDER_STATE_READING;
}

FileHolder::FileHolder() {
    mEntries.init(0x180);
}

FileHolderFileEntry *FileHolder::add(const char *pFilePath, JKRHeap *pHeap, void *pContext) {
    FileHolderFileEntry *pEntry = new(pHeap, 0) FileHolderFileEntry(pFilePath, pHeap, pContext);
    mEntries.push_back(pEntry);
    return pEntry;
}

bool FileHolder::isExist(const char *pFilePath) const {
    return findEntry(pFilePath);
}

void *FileHolder::getContext(const char *pFilePath) const {
    return findEntry(pFilePath)->mContext;
}

void FileHolder::removeIfIsEqualHeap(JKRHeap *pHeap) {
    if (pHeap == nullptr) {
        return;
    }

    for (FileHolderFileEntry **i = mEntries.begin(); i != mEntries.end(); ) {
        if ((*i)->mHeap == pHeap
            || MR::getHeapNapa((*i)->mHeap) == pHeap
            || MR::getHeapGDDR3((*i)->mHeap) == pHeap)
        {
            delete *i;
            mEntries.erase(i);
        } else {
            i++;
        }
    }
}

FileHolderFileEntry **FileHolder::removeFile(const char *pFilePath) {
    FileHolderFileEntry **p;
    FileHolderFileEntry *pEntry = findEntry(pFilePath);

    for (p = mEntries.begin(); p != mEntries.end() && *p != pEntry; p++);
    delete *p;

    return mEntries.erase(p);
}

FileHolderFileEntry *FileHolder::findEntry(const char *pFilePath) const {
    s32 entryNum = DVDConvertPathToEntrynum(pFilePath);
    for (FileHolderFileEntry * const *pEntry = mEntries.begin(); pEntry != mEntries.end(); pEntry++) {
        if (entryNum == (*pEntry)->mEntryNum) {
            return *pEntry;
        }
    }

    return nullptr;
}
