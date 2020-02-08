#ifndef RESOURCEINFO_H
#define RESOURCEINFO_H

#include <revolution.h>

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
};

class ResTable
{
public:
    ResTable();

    void newFileInfoTable(u32);
    ResFileInfo* add(const char *, void *, bool);
    const char* getResName(u32) const;
    void* getRes(u32) const;
    void* getRes(const char *) const;
    ResFileInfo* findFileInfo(const char *) const;
    ResFileInfo* getFileInfo(u32) const;
    bool isExistRes(const char *) const;
    void* findRes(const char *) const;
    s32 getResIndex(const char *) const;
    const char* findResName(const void *) const;
    const char* getResName(const void *) const;

    ResFileInfo* mFileInfo; // _0
    u32 mResCount; // _4
};

#endif // RESOURCEINFO_H