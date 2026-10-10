#pragma once

#include <revolution/types.h>

class ResFileInfo {
public:
    ResFileInfo();

    void setName(const char*, bool);

    inline bool isEqualHashCode(u32 hash) const {
        return mHashCode == hash;
    }

    /* 0x00 */ void* mResource;
    /* 0x04 */ u32 _4;
    /* 0x08 */ void* _8;
    /* 0x0C */ u32 _C;
    /* 0x10 */ char* mName;
    /* 0x14 */ u32 mHashCode;
};

class ResTable {
public:
    ResTable();

    void newFileInfoTable(u32);
    ResFileInfo* add(const char*, void*, bool);
    const char* getResName(u32) const;
    void* getRes(u32) const;
    void* getRes(const char*) const;
    ResFileInfo* findFileInfo(const char*) const;
    ResFileInfo* getFileInfo(u32) const;
    bool isExistRes(const char*) const;
    int getResIndex(const char*) const;
    const char* findResName(const void*) const;
    const char* getResName(const void*) const;

    /* 0x0 */ ResFileInfo* mFileInfoTable;
    /* 0x4 */ u32 mCount;

private:
    void* findRes(const char*) const;
};
