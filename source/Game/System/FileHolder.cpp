#include "Game/System/FileHolder.h"

FileHolderFileEntry::FileHolderFileEntry(const char *pName, JKRHeap *pHeap, void *pData) {
    mEntryNum = DVDConvertPathToEntrynum(pName);
    mContents = 0;
    mHeap = pHeap;
    _C = 0;
    _34 = 1;

    OSInitMessageQueue(&mQueue, &mMessage, 1);

    if (pData) {
        _34 = 0;
        mContents = pData;
    }
}

FileHolderFileEntry::~FileHolderFileEntry() {
    if (_34) {
        if (mContents) { 
            delete (u8*)mContents;
        }
    }
}

void FileHolderFileEntry::waitReadDone() {
    if (_C != 2) {
        OSMessage msg;
        OSReceiveMessage(&mQueue, &msg, 1);
        _C = 2;
    }
}

void FileHolderFileEntry::setContext(void *pData, JKRHeap *pHeap) {
    mContents = pData;
    mHeap = pHeap;

    OSSendMessage(&mQueue, 0, 0);
    _C = 1;
}

FileHolder::FileHolder() {
    mArray.mArray.mArr = new FileHolderFileEntry*[0x180];
    mArray.mArray.mMaxSize = 0x180;
}

void FileHolder::add(const char *pName, JKRHeap *pHeap, void *pData) {
    FileHolderFileEntry* entry = new FileHolderFileEntry(pName, pHeap, pData);
    s32 count = mArray.mArray.mCount;
    mArray.mArray.mCount++;
    mArray.mArray.mArr[count] = entry;
}

bool FileHolder::isExist(const char *pFile) const {
    return findEntry(pFile);
}

void* FileHolder::getContext(const char *pFile) const {
    return findEntry(pFile)->mContents;
}

// FileHolder::removeIfIsEqualHeap

FileHolderFileEntry** FileHolder::removeFile(const char *pFile) {
    FileHolderFileEntry* entry = findEntry(pFile);
    FileHolderFileEntry** i;

    for (i = mArray.mArray.mArr; i != &mArray.mArray.mArr[mArray.mArray.mCount] && *i != entry; i++) {
    }

    delete *i;

    return mArray.erase(i);
}

FileHolderFileEntry* FileHolder::findEntry(const char *pFile) const {
    s32 entryNum = DVDConvertPathToEntrynum(pFile);

    FileHolderFileEntry** curEntry;

    for (curEntry = mArray.mArray.mArr; curEntry != &mArray.mArray.mArr[mArray.mArray.mCount]; curEntry++) {
        if (entryNum == (*(curEntry))->mEntryNum) {
            return *curEntry;
        }
    }

    return 0;
}