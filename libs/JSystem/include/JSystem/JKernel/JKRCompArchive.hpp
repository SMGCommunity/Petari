#pragma once

#include "JSystem/JKernel/JKRArchive.hpp"

class JKRCompArchive : public JKRArchive {
public:
    JKRCompArchive(long, EMountDirection);
    virtual ~JKRCompArchive(void);

    virtual void removeResourceAll();
    virtual bool removeResource(void*);
    virtual s32 getExpandedResSize(const void*) const;
    virtual void* fetchResource(SDIFileEntry*, unsigned long*);
    virtual void* fetchResource(void*, unsigned long, SDIFileEntry*, unsigned long*);

    void open(long);

    u8 _64[0x1C];
};