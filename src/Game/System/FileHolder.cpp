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
    mArray.mArray.mArr = new FileHolderFileEntry*[0x180];
    mArray.mArray.mMaxSize = 0x180;
}

FileHolderFileEntry* FileHolder::add(const char *pName, JKRHeap *pHeap, void *pData) {
    FileHolderFileEntry* entry = new(pHeap, 0) FileHolderFileEntry(pName, pHeap, pData);
    s32 count = mArray.mCount;
    mArray.mCount++;
    mArray.mArray.mArr[count] = entry;
    return entry;
}

bool FileHolder::isExist(const char *pFile) const {
    return findEntry(pFile);
}

void* FileHolder::getContext(const char *pFile) const {
    return findEntry(pFile)->mContext;
}

// FileHolder::removeIfIsEqualHeap

FileHolderFileEntry** FileHolder::removeFile(const char *pFile) {
    FileHolderFileEntry* entry = findEntry(pFile);
    FileHolderFileEntry** i;

    for (i = mArray.mArray.mArr; i != &mArray.mArray.mArr[mArray.mCount] && *i != entry; i++) {
    }

    delete *i;

    return mArray.erase(i);
}

FileHolderFileEntry* FileHolder::findEntry(const char *pFile) const {
    s32 entryNum = DVDConvertPathToEntrynum(pFile);

    FileHolderFileEntry** curEntry;

    for (curEntry = mArray.mArray.mArr; curEntry != &mArray.mArray.mArr[mArray.mCount]; curEntry++) {
        if (entryNum == (*(curEntry))->mEntryNum) {
            return *curEntry;
        }
    }

    return 0;
}