#pragma once

#include <revolution/types.h>

class StageFileLoader {
public:
    StageFileLoader(const char*);

    void startLoadingStageFile();
    void waitLoadedStageFile();
    void makeStageArchiveNameList();
    static void makeStageArchiveName(char*, u32, const char*);
    void mountFilesInStageMapFile(const char*);

    /* 0x00 */ char* mStageFiles[0x18];
    /* 0x60 */ s32 mZoneCount;
};
