#pragma once

#include "JSystem/JKernel/JKRArchive.hpp"

class JKRAramBlock;
class JKRDvdFile;

class JKRCompArchive : public JKRArchive {
public:
    JKRCompArchive(long, EMountDirection);
    virtual ~JKRCompArchive(void);

    virtual void removeResourceAll();
    virtual bool removeResource(void*);
    virtual s32 getExpandedResSize(const void*) const;
    virtual void* fetchResource(SDIFileEntry*, u32*);
    virtual void* fetchResource(void*, u32, SDIFileEntry*, u32*);

    bool open(s32);

    /* 0x64 */ int field_0x64;
    /* 0x68 */ JKRAramBlock* mAramPart;
    /* 0x6C */ int field_0x6c;
    /* 0x70 */ JKRDvdFile* mDvdFile;
    /* 0x74 */ u32 mSizeOfMemPart;
    /* 0x78 */ u32 mSizeOfAramPart;
    /* 0x7C */ int field_0x7c;
};
