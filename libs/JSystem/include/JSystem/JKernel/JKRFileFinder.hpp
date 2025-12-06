#pragma once

#include "revolution.h"

class JKRArchive;

class JKRFileFinder {
public:
    char* mName;    // 0x0
    s32 mDirIndex;  // 0x4
    u16 mFileID;    // 0x8
    u16 mFileFlag;  // 0xA

    JKRFileFinder();
    virtual ~JKRFileFinder();

    virtual bool findNextFile() = 0;

    bool mHasMoreFiles;  // 0x10
    bool mFileIsFolder;  // 0x11
    u8 _12[2];
};

class JKRArcFinder : public JKRFileFinder {
public:
    JKRArcFinder(JKRArchive*, long, long);
    virtual ~JKRArcFinder();

    virtual bool findNextFile();

    JKRArchive* mArchive;  // 0x14
    s32 mFirstIndex;       // 0x18
    s32 mLastIndex;        // 0x1C
    s32 mCurrentIndex;     // 0x20
};