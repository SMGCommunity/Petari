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

    s32 mEntryNum;              // _0
    void* mContext;             // _4
    JKRHeap* mHeap;             // _8
    s32 mState;                 // _C
    OSMessageQueue mQueue;      // _10
    OSMessage mMessage;         // _30
    bool mContextSet;           // _34
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

    MR::Vector<MR::AssignableArray<FileHolderFileEntry*> > mArray; // _0
};
