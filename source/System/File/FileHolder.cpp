#include "System/File/FileHolder.h"
#include "revolution/dvd.h"

FileHolderFileEntry::FileHolderFileEntry(const char *pName, JKRHeap *pHeap, void *pSrc)
{
    mEntryNum = DVDConvertPathToEntrynum(pName);

    mSource = 0;
    mHeap = pHeap;
    _C = 0;
    _34 = 1;
    OSInitMessageQueue(&mQueue, &mMessage, 1);

    if (pSrc != 0)
    {
        _34 = 0;
        mSource = pSrc;
    }
}

void FileHolderFileEntry::waitReadDone()
{
    if (_C != 2)
    {
        OSMessage msg;
        OSReceiveMessage(&mQueue, &msg, 1);
        _C = 2;
    }
}

void FileHolderFileEntry::setContext(void *pSrc, JKRHeap *pHeap)
{
    mSource = pSrc;
    mHeap = pHeap;
    OSSendMessage(&mQueue, 0, 0);
    _C = 1;
}

bool FileHolder::isExist(const char *pName) const
{
    return findEntry(pName);
}

void* FileHolder::getContext(const char *pName) const
{
    return findEntry(pName)->mSource;
}