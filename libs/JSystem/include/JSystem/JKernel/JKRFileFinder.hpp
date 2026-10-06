#pragma once

#include <revolution.h>

class JKRArchive;

class JKRFileFinder {
public:
    /* 0x00 */ const char* mName;
    /* 0x04 */ s32 mDirIndex;
    /* 0x08 */ u16 mFileID;
    /* 0x0A */ u16 mFileFlag;

    JKRFileFinder();
    virtual ~JKRFileFinder() {
    }

    virtual bool findNextFile() = 0;

    /* 0x10 */ bool mHasMoreFiles;
    /* 0x11 */ bool mFileIsFolder;
    /* 0x12 */ u8 _12[2];
};

class JKRArcFinder : public JKRFileFinder {
public:
    JKRArcFinder(JKRArchive*, long, long);
    virtual ~JKRArcFinder();

    virtual bool findNextFile();

    /* 0x14 */ JKRArchive* mArchive;
    /* 0x18 */ s32 mFirstIndex;
    /* 0x1C */ s32 mLastIndex;
    /* 0x20 */ s32 mCurrentIndex;
};
