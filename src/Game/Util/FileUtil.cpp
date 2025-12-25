#include "Game/Util/FileUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/FileLoader.hpp"
#include "Game/System/FileRipper.hpp"
#include "Game/System/Language.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace MR {
    bool isFileExist(const char* pFilePath, bool considerLanguage) {
        s32 entryNum;

        if (considerLanguage) {
            entryNum = convertPathToEntrynumConsideringLanguage(pFilePath);
        } else {
            entryNum = DVDConvertPathToEntrynum(pFilePath);
        }

        return entryNum >= 0;
    }

    u32 getFileSize(const char* pFilePath, bool considerLanguage) {
        s32 entryNum;

        if (considerLanguage) {
            entryNum = convertPathToEntrynumConsideringLanguage(pFilePath);
        } else {
            entryNum = DVDConvertPathToEntrynum(pFilePath);
        }

        DVDFileInfo fileInfo;
        DVDFastOpen(entryNum, &fileInfo);
        u32 size = fileInfo.length;
        DVDClose(&fileInfo);

        return size;
    }

    s32 convertPathToEntrynumConsideringLanguage(const char* pFilePath) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        return DVDConvertPathToEntrynum(filePath);
    }

    void* loadToMainRAM(const char* pFilePath, u8* pDst, JKRHeap* pHeap, JKRDvdRipper::EAllocDirection allocDir) {
        loadAsyncToMainRAM(pFilePath, pDst, pHeap, allocDir);

        return receiveFile(pFilePath);
    }

    void loadAsyncToMainRAM(const char* pFilePath, u8* pDst, JKRHeap* pHeap, JKRDvdRipper::EAllocDirection allocDir) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        SingletonHolder< FileLoader >::get()->requestLoadToMainRAM(filePath, pDst, pHeap, allocDir, false);
    }

    void* mountArchive(const char* pFilePath, JKRHeap* pHeap) {
        mountAsyncArchive(pFilePath, pHeap);

        return receiveArchive(pFilePath);
    }

    void mountAsyncArchive(const char* pFilePath, JKRHeap* pHeap) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        SingletonHolder< FileLoader >::get()->requestMountArchive(filePath, pHeap, false);
    }

    void mountAsyncArchiveByObjectOrLayoutName(const char* pFilePrefix, JKRHeap* pHeap) {
        JKRHeap* pLocalHeap;

        if (pHeap != nullptr) {
            pLocalHeap = pHeap;
        } else {
            pLocalHeap = getAproposHeapForSceneArchive(0.03f);
        }

        char objArcPath[256];
        bool isExistObjArc = makeObjectArchiveFileNameFromPrefix(objArcPath, sizeof(objArcPath), pFilePrefix, false);

        char layoutArcPath[256];
        bool isExistLayoutArc = makeLayoutArchiveFileNameFromPrefix(layoutArcPath, sizeof(layoutArcPath), pFilePrefix, false);

        if (isExistObjArc) {
            mountAsyncArchive(objArcPath, pLocalHeap);
        } else if (isExistLayoutArc) {
            mountAsyncArchive(layoutArcPath, pLocalHeap);
        }
    }

    void* receiveFile(const char* pFilePath) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        return SingletonHolder< FileLoader >::get()->receiveFile(filePath);
    }

    void* receiveArchive(const char* pFilePath) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        return SingletonHolder< FileLoader >::get()->receiveArchive(filePath);
    }

    void receiveAllRequestedFile() {
        SingletonHolder< FileLoader >::get()->receiveAllRequestedFile();
    }

    void* createAndAddArchive(void* pArcData, JKRHeap* pHeap, const char* pFilePath) {
        return SingletonHolder< FileLoader >::get()->createAndAddArchive(pArcData, pHeap, pFilePath);
    }

    void getMountedArchiveAndHeap(const char* pFilePath, JKRArchive** ppArchive, JKRHeap** ppHeap) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        SingletonHolder< FileLoader >::get()->getMountedArchiveAndHeap(filePath, ppArchive, ppHeap);
    }

    void removeFileConsideringLanguage(const char* pFilePath) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        SingletonHolder< FileLoader >::get()->removeFile(filePath);
    }

    void removeResourceAndFileHolderIfIsEqualHeap(JKRHeap* pHeap) {
        SingletonHolder< ResourceHolderManager >::get()->removeIfIsEqualHeap(pHeap);
        SingletonHolder< FileLoader >::get()->removeHolderIfIsEqualHeap(pHeap);
    }

    void* decompressFileFromArchive(JKRArchive* pArchive, const char* pFilePath, JKRHeap* pHeap, int align) {
        u8* fileData = (u8*)pArchive->getResource(pFilePath);
        s32 compressionType = FileRipper::checkCompressed(fileData);
        u32 fileSize = pArchive->getResSize(fileData);

        u32 adjustedFileSize = fileSize;

        if (compressionType == 2)  // Yaz0
        {
            u32 decompressedSize = (fileData[4] << 24) | (fileData[5] << 16) | (fileData[6] << 8) | fileData[7];

            adjustedFileSize = decompressedSize;
        }

        u8* buffer = new (pHeap, align) u8[adjustedFileSize];

        JKRMemArchive::fetchResource_subroutine(fileData, fileSize, buffer, adjustedFileSize, compressionType);

        return buffer;
    }

    bool isLoadedFile(const char* pFilePath) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        return SingletonHolder< FileLoader >::get()->isLoaded(filePath);
    }

    bool isMountedArchive(const char* pFilePath) {
        char filePath[256];
        makeFileNameConsideringLanguage(filePath, sizeof(filePath), pFilePath);

        return SingletonHolder< FileLoader >::get()->isMountedArchive(filePath);
    }

    bool isLoadedObjectOrLayoutArchive(const char* pFilePrefix) {
        char objArcPath[256];
        bool isExistObjArc = makeObjectArchiveFileNameFromPrefix(objArcPath, sizeof(objArcPath), pFilePrefix, false);

        char layoutArcPath[256];
        bool isExistLayoutArc = makeLayoutArchiveFileNameFromPrefix(layoutArcPath, sizeof(layoutArcPath), pFilePrefix, false);

        if (isExistObjArc) {
            return isLoadedFile(objArcPath);
        } else {
            return isLoadedFile(layoutArcPath);
        }
    }

    void makeFileNameConsideringLanguage(char* pDst, u32 size, const char* pFilePath) {
        addFilePrefix(pDst, size, pFilePath, getCurrentLanguagePrefix());

        if (!isFileExist(pDst, false)) {
            snprintf(pDst, size, "%s", pFilePath);
        }
    }

    bool makeObjectArchiveFileName(char* pDst, u32 size, const char* pFileName) {
        snprintf(pDst, size, "/ObjectData/%s", pFileName);

        if (isFileExist(pDst, true)) {
            return true;
        }

        snprintf(pDst, size, "/MapPartsData/%s", pFileName);

        if (isFileExist(pDst, false)) {
            return true;
        }

        snprintf(pDst, size, "%s", pFileName);

        return isFileExist(pDst, true);
    }

    bool makeObjectArchiveFileNameFromPrefix(char* pDst, u32 size, const char* pFilePrefix, bool) {
        char fileName[256];
        snprintf(fileName, sizeof(fileName), "%s.arc", pFilePrefix);

        return makeObjectArchiveFileName(pDst, size, fileName);
    }

    bool makeLayoutArchiveFileName(char* pDst, u32 size, const char* pFileName) {
        snprintf(pDst, size, "/Region/LayoutData/%s", pFileName);

        if (isFileExist(pDst, false)) {
            return true;
        }

        snprintf(pDst, size, "/LayoutData/%s", pFileName);

        if (isFileExist(pDst, true)) {
            return true;
        }

        snprintf(pDst, size, "%s", pFileName);

        return isFileExist(pDst, false);
    }

    bool makeLayoutArchiveFileNameFromPrefix(char* pDst, u32 size, const char* pFilePrefix, bool fallback) {
        char fileName[64];
        snprintf(fileName, sizeof(fileName), "%s.arc", pFilePrefix);
        bool isExistArc = makeLayoutArchiveFileName(pDst, size, fileName);

        const char* pAspectSuffix = isScreen16Per9() ? "16x9" : "4x3";
        snprintf(fileName, sizeof(fileName), "%s%s.arc", pFilePrefix, pAspectSuffix);
        bool isExistAspectArc = makeLayoutArchiveFileName(pDst, size, fileName);

        snprintf(fileName, sizeof(fileName), "%sReplace.arc", pFilePrefix);
        bool isExistReplaceArc = makeLayoutArchiveFileName(pDst, size, fileName);

        bool fileFound = isExistArc || isExistAspectArc || isExistReplaceArc;

        if (!fileFound && !fallback) {
            return false;
        }

        if (isExistAspectArc) {
            pAspectSuffix = isScreen16Per9() ? "16x9" : "4x3";
            snprintf(fileName, sizeof(fileName), "%s%s.arc", pFilePrefix, pAspectSuffix);
        } else if (isExistReplaceArc) {
            snprintf(fileName, sizeof(fileName), "%sReplace.arc", pFilePrefix);
        } else {
            snprintf(fileName, sizeof(fileName), "%s.arc", pFilePrefix);
        }

        makeLayoutArchiveFileName(pDst, size, fileName);

        return true;
    }

    void makeScenarioArchiveFileName(char* pDst, u32 size, const char* pStageName) {
        snprintf(pDst, size, "/StageData/%s/%sScenario.arc", pStageName, pStageName);
    }
};  // namespace MR
