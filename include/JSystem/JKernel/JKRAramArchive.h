#pragma once

#include "JSystem/JKernel/JKRArchive.h"

class JKRAramArchive : public JKRArchive {
public:
    JKRAramArchive(long, EMountDirection);
    virtual ~JKRAramArchive();

    virtual s32 getExpandedResSize(const void *) const;
    virtual void *fetchResource(SDIFileEntry *, unsigned long *);
    virtual void *fetchResource(void *, unsigned long, SDIFileEntry *, unsigned long *);

    bool open(long);
    void fetchResource_subroutine(unsigned long, unsigned long, unsigned char *, unsigned long, int);
    void fetchResource_subroutine(unsigned long, unsigned long, JKRHeap *, int, unsigned char **);

    void *_64;
    void *_68;
};