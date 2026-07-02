#pragma once

#include <revolution/types.h>

struct SaveDataFileHeader {
    /* 0x00 */ u32 mCheckSum;
    /* 0x04 */ u32 mVersion;
    /* 0x08 */ u32 mUserFileInfoNum;
    /* 0x0C */ u32 mFileSize;
};

struct SaveDataFileInfo {
    /* 0x00 */ char mName[12];
    /* 0x0C */ u32 mOffset;
};

struct SaveDataFile {
    /* 0x00 */ SaveDataFileHeader mHeader;
    /* 0x10 */ SaveDataFileInfo mInfo[1];
};

struct SaveDataUserFileInfo {
    /* 0x00 */ u8* mData;
    /* 0x04 */ u32 mDataSize;
    /* 0x08 */ u8 mKind;
};

class SaveDataFileAccessor {
public:
    SaveDataFileAccessor(u8*);

    SaveDataFileHeader* getHeader();
    SaveDataFileInfo* getFileInfo(int);
    void makeUserFileInfo(SaveDataUserFileInfo*, const char*);

private:
    /* 0x00 */ SaveDataFile* mFile;
};
