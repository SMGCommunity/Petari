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

    u8 _0[0x60];
    u32 mZoneNum; // _60
};

#endif // STAGEFILELOADER_H