#ifndef RESOURCEINFO_H
#define RESOURCEINFO_H

#include "types.h"

class ResFileInfo
{
public:
    ResFileInfo();

    void setName(const char *, bool);

    void* mRes; // _0
    u32 _4;
    u32 _8;
    u32 _C;
    const char* mResName; // _10
    s32 mHashCode; // _14
    u32 _18;
};

class ResTable
{
public:
    ResTable();

    void newFileInfoTable(u32);
    void add(const char *, void *, bool);
    const char* getResName(u32) const;
    void* getRes(u32) const;
    void* getRes(const char *) const;

    const char* getResName(const void *) const;

    ResFileInfo* mFileInfo; // _0
    u32 _4;
};

#endif // RESOURCEINFO_H