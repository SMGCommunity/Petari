#pragma once

#include "JSystem/JKernel/JKRArchive.h"

enum JKRMemBreakFlag {

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
    virtual void fetchResource(void *, unsigned long, SDIFileEntry *, unsigned long *);

    void fixedInit(long);
    void mountFixed(void *, JKRMemBreakFlag);
    void open(long, EMountDirection);
    void open(void *, JKRMemBreakFlag);
    void fetchResource_subroutine(unsigned char *, unsigned long, unsigned char *, unsigned long, int);

    u8 _64[12];
};