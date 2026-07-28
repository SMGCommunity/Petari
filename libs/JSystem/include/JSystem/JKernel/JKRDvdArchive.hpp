#pragma once

#include "JSystem/JKernel/JKRArchive.hpp"
#include "revolution/types.h"

class JKRDvdFile;

class JKRDvdArchive : public JKRArchive {
public:
    JKRDvdArchive(long, EMountDirection);
    virtual ~JKRDvdArchive();

    virtual s32 getExpandedResSize(const void*) const;
    virtual void* fetchResource(SDIFileEntry*, unsigned long*);
    virtual void* fetchResource(void*, unsigned long, SDIFileEntry*, unsigned long*);

    bool open(long);
    static u32 fetchResource_subroutine(long, unsigned long, unsigned long, unsigned char*, unsigned long, int, int);
    static u32 fetchResource_subroutine(long, unsigned long, unsigned long, JKRHeap*, int, int, unsigned char**);

    u32 _64;
    /* 0x68 */ JKRDvdFile* mFile;
};
