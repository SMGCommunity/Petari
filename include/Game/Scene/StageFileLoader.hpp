#pragma once

#include <revolution.h>

class StageFileLoader {
public:
    StageFileLoader(const char*);

    void startLoadingStageFile();
    void waitLoadedStageFile();
    void makeStageArchiveNameList();
    static void makeStageArchiveName(char*, u32, const char*);
    void mountFilesInStageMapFile(const char*);

    char* mStageFiles[0x18];  // 0x0
    s32 mZoneCount;           // 0x60
};