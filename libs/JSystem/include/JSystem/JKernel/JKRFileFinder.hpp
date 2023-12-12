#pragma once

#include "revolution.h"

class JKRArchive;

class JKRFileFinder {
public:
    char *mName;		// _0
    s32 mDirIndex;		// _4
    u16 mFileID;		// _8
    u16 mFileFlag;		// _A

    JKRFileFinder();
    virtual ~JKRFileFinder();

    virtual bool findNextFile() = 0;

    bool mHasMoreFiles;	// _10
    bool mFileIsFolder;	// _11
    u8 _12[2];
};

class JKRArcFinder : public JKRFileFinder {
public:
    JKRArcFinder(JKRArchive *, long, long);
    virtual ~JKRArcFinder();

    virtual bool findNextFile();

    JKRArchive *mArchive;	// _14
    s32 mFirstIndex;		// _18
    s32 mLastIndex;			// _1C
    s32 mCurrentIndex;		// _20
};