#ifndef STAGEFILELOADER_H
#define STAGEFILELOADER_H

#include "types.h"

class StageFileLoader
{
public:
    StageFileLoader(const char *);

    void startLoadingStageFile();
    void waitLoadedStageFile();
    void makeStageArchiveNameList();
    void makeStageArchiveName(char *, u32, const char *);
    void mountFilesInStageMapFile(const char *);

    const char* mNames[0x18]; // _0
    u32 mZoneNum; // _60
};

#endif // STAGEFILELOADER_H