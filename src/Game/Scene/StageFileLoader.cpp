#include "Game/Scene/StageFileLoader.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/Util.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"
#include "JSystem/JKernel/JKRFileFinder.hpp"
#include <cstring>
#include <cstdio>

StageFileLoader::StageFileLoader(const char *pName) {
    mZoneCount = 0;
    makeStageArchiveNameList();
}

void StageFileLoader::startLoadingStageFile() {
    for (int i = 0; i < mZoneCount; i++) {
        MR::mountAsyncArchive(mStageFiles[i], MR::getAproposHeapForSceneArchive(0.029999999f));
    }
}

void StageFileLoader::waitLoadedStageFile() {
    for (int i = 0; i < mZoneCount; i++) {
        const char* file = mStageFiles[i];
        MR::receiveFile(file);
        mountFilesInStageMapFile(file);
    }
}

void StageFileLoader::makeStageArchiveNameList() {
    GalaxyStatusAccessor access = MR::makeCurrentGalaxyStatusAccessor();
    mZoneCount = access.getZoneNum();

    for (int i = 0; i < mZoneCount; i++) {
        const char* zoneName = access.getZoneName(i);
        char path[0x100];
        snprintf(path, 0x100, "/StageData/%s.arc", zoneName);
        u32 len = strlen(path) + 1;
        mStageFiles[i] = new char[len];
        MR::copyString(mStageFiles[i], path, len);
    }
}

void StageFileLoader::makeStageArchiveName(char *buf, u32 len, const char *pZoneName) {
    snprintf(buf, len, "/StageData/%s.arc", pZoneName);
}

void StageFileLoader::mountFilesInStageMapFile(const char *pName) {
    JKRArchive* archive = 0;
    JKRHeap* heap = 0;
    MR::getMountedArchiveAndHeap(pName, &archive, &heap);
    void* res;
    const char* curName;
    s32 fileCount = archive->countFile("/arc") - 2;

    if (fileCount > 0) {
        JKRArcFinder* firstFile = archive->getFirstFile("/arc");

        for (int i = 0; i < fileCount;) {
            if (!MR::isMountedArchive(firstFile->mName)) {
                curName = firstFile->mName;
                res = archive->getResource(QUESTIONMARK_MAGIC, curName);
                MR::createAndAddArchive(res, heap, curName);
            }

            i++;
            firstFile->findNextFile();
        }

        delete firstFile;
    }
}