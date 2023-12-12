#pragma once

#include <revolution.h>

class StageFileLoader {
public:
    StageFileLoader(const char *);

    void startLoadingStageFile();
    void waitLoadedStageFile();
    void makeStageArchiveNameList();
    static void makeStageArchiveName(char *, u32, const char *);
    void mountFilesInStageMapFile(const char *);

    char* mStageFiles[0x18];  // _0
    s32 mZoneCount;                 // _60
};