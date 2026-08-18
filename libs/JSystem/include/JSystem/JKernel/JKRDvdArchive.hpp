#pragma once

#include "JSystem/JKernel/JKRArchive.hpp"
#include "revolution/types.h"

class JKRDvdFile;

class JKRDvdArchive : public JKRArchive {
public:
    JKRDvdArchive(s32, EMountDirection);
    virtual ~JKRDvdArchive();

    virtual u32 getExpandedResSize(const void*) const;
    virtual void* fetchResource(SDIFileEntry*, u32*);
    virtual void* fetchResource(void*, u32, SDIFileEntry*, u32*);

    bool open(s32);
    static u32 fetchResource_subroutine(s32, u32, u32, u8*, u32, int, int);
    static u32 fetchResource_subroutine(s32, u32, u32, JKRHeap*, int, int, u8**);

    u32 _64;
    /* 0x68 */ JKRDvdFile* mFile;
};
