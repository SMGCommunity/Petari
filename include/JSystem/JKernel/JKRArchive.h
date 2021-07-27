#pragma once

#include "JSystem/JKernel/JKRFileFinder.h"
#include "JSystem/JKernel/JKRFileLoader.h"
#include <revolution.h>

enum JKRMemBreakFlag
{
    
};

class JKRArchive : public JKRFileLoader
{
public:
    enum EMountMode
    {
        MEM = 0x1,
        ARAM = 0x2,
        DVD = 0x3,
        CMP = 0x4
    };

    enum EMountDirection
    {

    };

    struct SDIFileEntry
    {

    };

    JKRArchive();
    JKRArchive(s32, EMountMode);

    virtual ~JKRArchive();
    virtual bool becomeCurrent(const char *);
    virtual void* getResource(const char *);
    virtual void* getResource(u32, const char *);
    virtual u32* readResource(void *, u32, const char *);
    virtual u32* readResource(void *, u32, u32, const char *);
    virtual void removeResourceAll();
    virtual bool removeResource(void *);
    virtual bool detatchResource(void *);
    virtual s32 getResSize(const void *) const;
    virtual u32 countFile(const char *) const;
    virtual JKRArcFinder* getFirstFile(const char *) const;
    virtual s32 getExpandedResSize(const void *) const;
    virtual void* fetchResource(SDIFileEntry *, u32 *) = 0;
    virtual void* fetchResource(void *, u32, SDIFileEntry *, u32 *) = 0;
    virtual void setExpandSize(SDIFileEntry *, u32);
    virtual u32 getExpandSize(SDIFileEntry *) const;

    u32 countResource() const;

    JKRHeap* _38;
    u8 _3C;
    u8 _3D;
    u8 _3E;
    u8 _3F;
    s32 _40;
    u8 _44[0x64-0x44];

    static u32 sCurrentDirID;
};