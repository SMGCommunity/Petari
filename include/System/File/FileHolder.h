#pragma once

#include "JKernel/JKRHeap.h"
#include "revolution/os/OSMessage.h"

class FileHolderFileEntry
{
public:
    FileHolderFileEntry(const char *, JKRHeap *, void *);
    ~FileHolderFileEntry();

    void waitReadDone();
    void setContext(void *, JKRHeap *);

    s32 mEntryNum; // _0
    void* mSource; // _4
    JKRHeap* mHeap; // _8
    u32 _C;
    OSMessageQueue mQueue; // _10
    OSMessage mMessage; // _30
    u8 _34;
};

class FileHolder
{
public:
    FileHolder();

    void add(const char *, JKRHeap *, void *);
    bool isExist(const char *) const;
    void* getContext(const char *) const;

    FileHolderFileEntry* findEntry(const char *) const;
};