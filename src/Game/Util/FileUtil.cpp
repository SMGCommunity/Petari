#include "Game/System/FileLoader.hpp"
#include "Game/Util.hpp"
#include "Game/SingletonHolder.hpp"

#include <cstdio>
#include "revolution.h"

namespace MR {
    extern const char* getCurrentLanguagePrefix();

    bool isFileExist(const char *pFile, bool considerLanguage) {
        s32 entryNum;

        if (considerLanguage) {
            entryNum = MR::convertPathToEntrynumConsideringLanguage(pFile);
        }
        else {
            entryNum = DVDConvertPathToEntrynum(pFile);
        }

        return entryNum >= 0;
    }

    u32 getFileSize(const char *pFile, bool considerLanguage) {
        s32 entryNum;

        if (considerLanguage) {
            entryNum = MR::convertPathToEntrynumConsideringLanguage(pFile);
        }
        else {
            entryNum = DVDConvertPathToEntrynum(pFile);
        }

        DVDFileInfo fileInfo;
        DVDFastOpen(entryNum, &fileInfo);
        u32 size = fileInfo.length;
        DVDClose(&fileInfo);

        return size;
    }

    s32 convertPathToEntrynumConsideringLanguage(const char *pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        return DVDConvertPathToEntrynum(buf);
    }

    void* loadToMainRAM(const char *pFile, u8 *pData, JKRHeap *pHeap, JKRDvdRipper::EAllocDirection allocDir) {
        MR::loadAsyncToMainRAM(pFile, pData, pHeap, allocDir);
        return MR::receiveFile(pFile);
    }

    void loadAsyncToMainRAM(const char *pFile, u8 *pData, JKRHeap *pHeap, JKRDvdRipper::EAllocDirection allocDir) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        SingletonHolder<FileLoader>::sInstance->requestLoadToMainRAM(buf, pData, pHeap, allocDir, false);
    }

    void* mountArchive(const char *pFile, JKRHeap *pHeap) {
        MR::mountAsyncArchive(pFile, pHeap);
        return MR::receiveArchive(pFile);
    }

    void mountAsyncArchive(const char *pFile, JKRHeap *pHeap) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        SingletonHolder<FileLoader>::sInstance->requestMountArchive(buf, pHeap, false);
    }

    void mountAsyncArchiveByObjectOrLayoutName(const char *pFile, JKRHeap *pHeap) {
        JKRHeap* heap;

        if (pHeap) {
            heap = pHeap;
        }
        else {
            heap = MR::getAproposHeapForSceneArchive(0.029999999f);
        }

        char objArch[0x100];
        bool objArchiveName = MR::makeObjectArchiveFileNameFromPrefix(objArch, 0x100, pFile, false);

        char layArch[0x100];
        bool layoutArchiveName = MR::makeLayoutArchiveFileNameFromPrefix(layArch, 0x100, pFile, false);

        if (objArchiveName) {
            MR::mountAsyncArchive(objArch, heap);
        }
        else if (layoutArchiveName) {
            MR::mountAsyncArchive(layArch, heap);
        }
    }

    void* receiveFile(const char *pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        return SingletonHolder<FileLoader>::sInstance->receiveFile(buf);
    }

    void* receiveArchive(const char *pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        return SingletonHolder<FileLoader>::sInstance->receiveArchive(buf);
    }

    void receiveAllRequestedFile() {
        SingletonHolder<FileLoader>::sInstance->receiveAllRequestedFile();
    }

    void* createAndAddArchive(void *pData, JKRHeap *pHeap, const char *pFile) {
        return SingletonHolder<FileLoader>::sInstance->createAndAddArchive(pData, pHeap, pFile);
    }

    void getMountedArchiveAndHeap(const char *pFile, JKRArchive **pArchive, JKRHeap **pHeap) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        SingletonHolder<FileLoader>::sInstance->getMountedArchiveAndHeap(pFile, pArchive, pHeap);
    }

    void removeFileConsideringLanguage(const char *pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        SingletonHolder<FileLoader>::sInstance->removeFile(pFile);
    }

    // MR::removeResourceAndFileHolderIfIsEqualHeap
    // MR::decompressFileFromArchive

    bool isLoadedFile(const char *pFile) {
       const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        return SingletonHolder<FileLoader>::sInstance->isLoaded(pFile);
    }

    bool isMountedArchive(const char *pFile) {
       const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, 0x100, pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, 0x100, "%s", pFile);
        }

        return SingletonHolder<FileLoader>::sInstance->isMountedArchive(pFile);
    }

    bool isLoadedObjectOrLayoutArchive(const char *pFile) {
        char objFile[0x100];
        bool obj_arch = MR::makeObjectArchiveFileNameFromPrefix(objFile, 0x100, pFile, false);

        char archFile[0x100];
        bool lay_arch = MR::makeLayoutArchiveFileNameFromPrefix(archFile, 0x100, pFile, false);

        if (obj_arch) {
            return MR::isLoadedFile(objFile);
        }
        else {
            return MR::isLoadedFile(archFile);
        }
    }

    void makeFileNameConsideringLanguage(char *pName, u32 length, const char *pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        MR::addFilePrefix(pName, length, pFile, langPrefix);

        if (!isFileExist(pName, false)) {
            snprintf(pName, length, "%s", pFile);
        }
    }

    bool makeObjectArchiveFileName(char *pName, u32 length, const char *pFile) {
        snprintf(pName, length, "/ObjectData/%s", pFile);

        if (MR::isFileExist(pName, true)) {
            return true;
        }

        snprintf(pName, length, "/MapPartsData/%s", pFile);

        if (MR::isFileExist(pName, false)) {
            return true;
        }

        snprintf(pName, length, "%s", pFile);
        return MR::isFileExist(pName, true);
    }

    bool makeObjectArchiveFileNameFromPrefix(char *pName, u32 length, const char *pFile, bool /*unused*/) {
        char buf[0x100];
        snprintf(buf, 0x100, "%s.arc", pFile);
        return MR::makeObjectArchiveFileName(pName, length, buf);
    }

    bool makeLayoutArchiveFileName(char *pName, u32 length, const char *pFile) {
        snprintf(pName, length, "/Region/LayoutData/%s", pFile);

        if (MR::isFileExist(pName, false)) {
            return true;
        }

        snprintf(pName, length, "/LayoutData/%s", pFile);

        if (MR::isFileExist(pName, true)) {
            return true;
        }

        snprintf(pName, length, "%s", pFile);
        return MR::isFileExist(pName, false);
    }

    // MR::makeLayoutArchiveFileNameFromPrefix

    void makeScenarioArchiveFileName(char *pName, u32 length, const char *pFile) {
        snprintf(pName, length, "/StageData/%s/%sScenario.arc", pFile, pFile);
    }
};