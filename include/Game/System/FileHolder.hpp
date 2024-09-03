#pragma once

#include <revolution.h>
#include "Game/Util.hpp"

class JKRHeap;

class FileHolderFileEntry {
public:
    FileHolderFileEntry(const char *, JKRHeap *, void *);
    ~FileHolderFileEntry();
    
    void waitReadDone();
    void setContext(void *, JKRHeap *);

    s32 mEntryNum;              // 0x0
    void* mContext;             // 0x4
    JKRHeap* mHeap;             // 0x8
    s32 mState;                 // 0xC
    OSMessageQueue mQueue;      // 0x10
    OSMessage mMessage;         // 0x30
    bool mContextSet;           // 0x34
};

class FileHolder {
public:
    FileHolder();

    FileHolderFileEntry* add(const char *, JKRHeap *, void *);
    bool isExist(const char *) const;
    void* getContext(const char *) const;
    void removeIfIsEqualHeap(JKRHeap *);

    FileHolderFileEntry** removeFile(const char *);
    FileHolderFileEntry* findEntry(const char *) const;

    MR::Vector<MR::AssignableArray<FileHolderFileEntry*> > mArray; // 0x0
};
