#pragma once

#include "JSystem/JKernel/JKRArchive.hpp"

class JKRDvdArchive : public JKRArchive {
public:
    JKRDvdArchive(long, EMountDirection);
    virtual ~JKRDvdArchive();

    virtual s32 getExpandedResSize(const void*) const;
    virtual void* fetchResource(SDIFileEntry*, unsigned long*);
    virtual void* fetchResource(void*, unsigned long, SDIFileEntry*, unsigned long*);

    bool open(long);
    void fetchResource_subroutine(long, unsigned long, unsigned long, unsigned char*, unsigned long, int, int);
    void fetchResource_subroutine(long, unsigned long, unsigned long, JKRHeap*, int, int, unsigned char**);

    u8 _64[4];
    void* _68;
};