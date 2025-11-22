#include "Game/System/FileLoader.hpp"
#include "Game/System/FileRipper.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/Util.hpp"
#include "Game/SingletonHolder.hpp"

#include <cstdio>
#include "Game/Util/SystemUtil.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"
#include "JSystem/JKernel/JKRFileFinder.hpp"
#include "JSystem/JKernel/JKRFileLoader.hpp"
#include "JSystem/JKernel/JKRDecomp.hpp"
#include "revolution.h"

namespace MR {
    extern const char* getCurrentLanguagePrefix();

    bool isFileExist(const char* pFile, bool considerLanguage) {
        s32 entryNum;

        if (considerLanguage) {
            entryNum = MR::convertPathToEntrynumConsideringLanguage(pFile);
        } else {
            entryNum = DVDConvertPathToEntrynum(pFile);
        }

        return entryNum >= 0;
    }

    u32 getFileSize(const char* pFile, bool considerLanguage) {
        s32 entryNum;

        if (considerLanguage) {
            entryNum = MR::convertPathToEntrynumConsideringLanguage(pFile);
        } else {
            entryNum = DVDConvertPathToEntrynum(pFile);
        }

        DVDFileInfo fileInfo;
        DVDFastOpen(entryNum, &fileInfo);
        u32 size = fileInfo.length;
        DVDClose(&fileInfo);

        return size;
    }

    s32 convertPathToEntrynumConsideringLanguage(const char* pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        return DVDConvertPathToEntrynum(buf);
    }

    void* loadToMainRAM(
        const char*                   pFile,
        u8*                           pData,
        JKRHeap*                      pHeap,
        JKRDvdRipper::EAllocDirection allocDir) {
        MR::loadAsyncToMainRAM(pFile, pData, pHeap, allocDir);
        return MR::receiveFile(pFile);
    }

    void loadAsyncToMainRAM(
        const char*                   pFile,
        u8*                           pData,
        JKRHeap*                      pHeap,
        JKRDvdRipper::EAllocDirection allocDir) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        SingletonHolder< FileLoader >::get()->requestLoadToMainRAM(
            buf,
            pData,
            pHeap,
            allocDir,
            false);
    }

    void* mountArchive(const char* pFile, JKRHeap* pHeap) {
        MR::mountAsyncArchive(pFile, pHeap);
        return MR::receiveArchive(pFile);
    }

    void mountAsyncArchive(const char* pFile, JKRHeap* pHeap) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        SingletonHolder< FileLoader >::get()->requestMountArchive(buf, pHeap, false);
    }

    void mountAsyncArchiveByObjectOrLayoutName(const char* pFile, JKRHeap* pHeap) {
        JKRHeap* heap;

        if (pHeap) {
            heap = pHeap;
        } else {
            heap = MR::getAproposHeapForSceneArchive(0.029999999f);
        }

        char objArch[0x100];
        bool objArchiveName = MR::makeObjectArchiveFileNameFromPrefix(
            objArch,
            sizeof(objArch),
            pFile,
            false);

        char layArch[0x100];
        bool layoutArchiveName = MR::makeLayoutArchiveFileNameFromPrefix(
            layArch,
            sizeof(layArch),
            pFile,
            false);

        if (objArchiveName) {
            MR::mountAsyncArchive(objArch, heap);
        } else if (layoutArchiveName) {
            MR::mountAsyncArchive(layArch, heap);
        }
    }

    void* receiveFile(const char* pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        return SingletonHolder< FileLoader >::get()->receiveFile(buf);
    }

    void* receiveArchive(const char* pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        return SingletonHolder< FileLoader >::get()->receiveArchive(buf);
    }

    void receiveAllRequestedFile() {
        SingletonHolder< FileLoader >::get()->receiveAllRequestedFile();
    }

    void* createAndAddArchive(void* pData, JKRHeap* pHeap, const char* pFile) {
        return SingletonHolder< FileLoader >::get()->createAndAddArchive(pData, pHeap, pFile);
    }

    void getMountedArchiveAndHeap(const char* pFile, JKRArchive** pArchive, JKRHeap** pHeap) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        SingletonHolder< FileLoader >::get()->getMountedArchiveAndHeap(buf, pArchive, pHeap);
    }

    void removeFileConsideringLanguage(const char* pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        SingletonHolder< FileLoader >::get()->removeFile(buf);
    }

    void MR::removeResourceAndFileHolderIfIsEqualHeap(JKRHeap* heap) {
        SingletonHolder< ResourceHolderManager >::get()->removeIfIsEqualHeap(heap);
        SingletonHolder< FileLoader >::get()->removeHolderIfIsEqualHeap(heap);
    }

    void* MR::decompressFileFromArchive(
        JKRArchive* archive,
        const char* filename,
        JKRHeap*    heap,
        int         align) {
        u8* fileData = (u8*)archive->getResource(filename);
        s32 compressionType = FileRipper::checkCompressed(fileData);
        u32 fileSize = archive->getResSize(fileData);

        u32 adjustedFileSize = fileSize;

        if (compressionType == 2) //Yaz0
        {
            u32 decompressedSize =
                (fileData[4] << 24) | (fileData[5] << 16) | (fileData[6] << 8) | fileData[7];

            adjustedFileSize = decompressedSize;
        }

        u8* buffer = new (heap, align) u8[adjustedFileSize];

        JKRMemArchive::fetchResource_subroutine(
            fileData,
            fileSize,
            buffer,
            adjustedFileSize,
            compressionType);

        return buffer;
    }

    bool isLoadedFile(const char* pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        return SingletonHolder< FileLoader >::get()->isLoaded(buf);
    }

    bool isMountedArchive(const char* pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        char buf[0x100];
        MR::addFilePrefix(buf, sizeof(buf), pFile, langPrefix);

        if (!isFileExist(buf, false)) {
            snprintf(buf, sizeof(buf), "%s", pFile);
        }

        return SingletonHolder< FileLoader >::get()->isMountedArchive(buf);
    }

    bool isLoadedObjectOrLayoutArchive(const char* pFile) {
        char objFile[0x100];
        bool obj_arch = MR::makeObjectArchiveFileNameFromPrefix(objFile, sizeof(objFile), pFile, false);

        char archFile[0x100];
        bool lay_arch = MR::makeLayoutArchiveFileNameFromPrefix(archFile, sizeof(archFile), pFile, false);

        if (obj_arch) {
            return MR::isLoadedFile(objFile);
        } else {
            return MR::isLoadedFile(archFile);
        }
    }

    void makeFileNameConsideringLanguage(char* pName, u32 length, const char* pFile) {
        const char* langPrefix = getCurrentLanguagePrefix();

        MR::addFilePrefix(pName, length, pFile, langPrefix);

        if (!isFileExist(pName, false)) {
            snprintf(pName, length, "%s", pFile);
        }
    }

    bool makeObjectArchiveFileName(char* pName, u32 length, const char* pFile) {
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

    bool makeObjectArchiveFileNameFromPrefix(
        char*       pName,
        u32         length,
        const char* pFile,
        bool /*unused*/
    ) {
        char buf[0x100];
        snprintf(buf, sizeof(buf), "%s.arc", pFile);
        return MR::makeObjectArchiveFileName(pName, length, buf);
    }

    bool makeLayoutArchiveFileName(char* pName, u32 length, const char* pFile) {
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

    bool MR::makeLayoutArchiveFileNameFromPrefix(
        char*       pName,
        u32         length,
        const char* pFile,
        bool        fallback) {
        char buf[0x40];

        snprintf(buf, sizeof(buf), "%s.arc", pFile);
        bool hasBaseFile = MR::makeLayoutArchiveFileName(pName, length, buf);

        const char* aspectSuffix = MR::isScreen16Per9() ? "16x9" : "4x3";
        snprintf(buf, sizeof(buf), "%s%s.arc", pFile, aspectSuffix);
        bool hasAspectFile = MR::makeLayoutArchiveFileName(pName, length, buf);

        snprintf(buf, sizeof(buf), "%sReplace.arc", pFile);
        bool hasReplaceFile = MR::makeLayoutArchiveFileName(pName, length, buf);

        bool fileFound = hasBaseFile || hasAspectFile || hasReplaceFile;

        if (!fileFound && !fallback) {
            return false;
        } else {
            if (hasAspectFile) {
                aspectSuffix = MR::isScreen16Per9() ? "16x9" : "4x3";
                snprintf(buf, sizeof(buf), "%s%s.arc", pFile, aspectSuffix);
            } else if (hasReplaceFile) {
                snprintf(buf, sizeof(buf), "%sReplace.arc", pFile);
            } else {
                snprintf(buf, sizeof(buf), "%s.arc", pFile);
            }

            MR::makeLayoutArchiveFileName(pName, length, buf);
            return true;
        }
    }

    void makeScenarioArchiveFileName(char* pName, u32 length, const char* pFile) {
        snprintf(pName, length, "/StageData/%s/%sScenario.arc", pFile, pFile);
    }
}; // namespace MR
