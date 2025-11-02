#include "Game/System/FileHolder.hpp"

FileHolderFileEntry::FileHolderFileEntry(const char *pName, JKRHeap *pHeap, void *pData) {
    mEntryNum = DVDConvertPathToEntrynum(pName);
    mContext = 0;
    mHeap = pHeap;
    mState = 0;
    mContextSet = true;

    OSInitMessageQueue(&mQueue, &mMessage, 1);

    if (pData) {
        mContextSet = false;
        mContext = pData;
    }
}

FileHolderFileEntry::~FileHolderFileEntry() {
    if (mContextSet) {
        if (mContext) { 
            delete (u8*)mContext;
        }
    }
}

void FileHolderFileEntry::waitReadDone() {
    if (mState != 2) {
        OSMessage msg;
        OSReceiveMessage(&mQueue, &msg, 1);
        mState = 2;
    }
}

void FileHolderFileEntry::setContext(void *pData, JKRHeap *pHeap) {
    mContext = pData;
    mHeap = pHeap;

    OSSendMessage(&mQueue, 0, 0);
    mState = 1;
}

FileHolder::FileHolder() {
    mArray.init(0x180);
}

FileHolderFileEntry* FileHolder::add(const char *pName, JKRHeap *pHeap, void *pData) {
    FileHolderFileEntry* pEntry = new(pHeap, 0) FileHolderFileEntry(pName, pHeap, pData);

    mArray.push_back(pEntry);

    return pEntry;
}

bool FileHolder::isExist(const char *pFile) const {
    return findEntry(pFile);
}

void* FileHolder::getContext(const char *pFile) const {
    return findEntry(pFile)->mContext;
}

// FileHolder::removeIfIsEqualHeap

FileHolderFileEntry** FileHolder::removeFile(const char *pFile) {
    FileHolderFileEntry** p;
    FileHolderFileEntry* pEntry = findEntry(pFile);

    for (p = mArray.begin(); p != mArray.end() && *p != pEntry; p++) {
        
    }

    delete *p;

    return mArray.erase(p);
}

FileHolderFileEntry* FileHolder::findEntry(const char *pFile) const {
    s32 entryNum = DVDConvertPathToEntrynum(pFile);

    for (FileHolderFileEntry* const* pEntry = mArray.begin(); pEntry != mArray.end(); pEntry++) {
        if (entryNum == (*pEntry)->mEntryNum) {
            return *pEntry;
        }
    }

    return nullptr;
}