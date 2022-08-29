#pragma once

#include "JSystem/JKernel/JKRArchive.h"

enum JKRMemBreakFlag {
    JKR_MEM_BREAK_FLAG_1 = 1
};

class JKRMemArchive : public JKRArchive {
public:
    JKRMemArchive();
    JKRMemArchive(long, EMountDirection);
    virtual ~JKRMemArchive();

    virtual void removeResourceAll();
    virtual bool removeResource(void *);
    virtual s32 getExpandedResSize(const void *) const;
    virtual void *fetchResource(SDIFileEntry *, unsigned long *);
    virtual void *fetchResource(void *, unsigned long, SDIFileEntry *, unsigned long *);

    void fixedInit(long);
    bool mountFixed(void *, JKRMemBreakFlag);
    bool open(long, EMountDirection);
    bool open(void *, unsigned long, JKRMemBreakFlag);
    static s32 fetchResource_subroutine(unsigned char *, unsigned long, unsigned char *, unsigned long, int);

    RarcHeader *mHeader;        // _64
    u8 *mFileDataStart;         // _68
    bool _6C;
    u8 _6D[3];
};