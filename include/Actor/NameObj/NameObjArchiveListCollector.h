#ifndef NAMEOBJARCHIVELISTCOLLECTOR_H
#define NAMEOBJARCHIVELISTCOLLECTOR_H

#include "types.h"

class NameObjArchiveListCollector
{
public:
    NameObjArchiveListCollector();

    void addArchive(const char *);
    const char* getArchive(u32) const;

    char mArchives[0x20][0x40]; // _0
    s32 mNumArchives; // _800
};

#endif // NAMEOBJARCHIVELISTCOLLECTOR_H