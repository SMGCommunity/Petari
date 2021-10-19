#include "Game/System/FileLoader.h"
#include "Game/Util.h"
#include "Game/SingletonHolder.h"

#include <stdio.h>
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
        const char* objArchiveName = MR::makeObjectArchiveFileNameFromPrefix(objArch, 0x100, pFile, false);

        char layArch[0x100];
        const char* layoutArchiveName = MR::makeLayoutArchiveFileNameFromPrefix(layArch, 0x100, pFile, false);

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
};