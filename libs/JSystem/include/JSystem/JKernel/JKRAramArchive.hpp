#pragma once

#include "JSystem/JKernel/JKRArchive.hpp"

class JKRAramBlock;
class JKRFile;

class JKRAramArchive : public JKRArchive {
public:
    JKRAramArchive(long, EMountDirection);
    virtual ~JKRAramArchive();

    virtual u32 getExpandedResSize(const void*) const;
    virtual void* fetchResource(SDIFileEntry*, unsigned long*);
    virtual void* fetchResource(void*, unsigned long, SDIFileEntry*, unsigned long*);

    bool open(long);
    static u32 fetchResource_subroutine(unsigned long, unsigned long, unsigned char*, unsigned long, int);
    static u32 fetchResource_subroutine(unsigned long, unsigned long, JKRHeap*, int, unsigned char**);

    JKRAramBlock* mBlock;
    JKRFile* mDvdFile;
};

inline int JKRConvertAttrToCompressionType(int arg) {
    if ((arg & 0x4) == 0) {
        return 0;
    }

    return ((arg & 0x80) != 0) + 1;
}
