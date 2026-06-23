#include "Game/System/SaveDataHandler.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NANDManager.hpp"
#include "Game/System/SaveDataBannerCreator.hpp"
#include "Game/System/SaveDataFileAccessor.hpp"
#include "Game/System/SysConfigFile.hpp"
#include "Game/System/UserFile.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <cstdio>

namespace {
    struct SaveFileSpec {
        /* 0x00 */ const char* mName;
        /* 0x04 */ u32 mBufferSize;
    };

    const char cSaveFileName[] = "GameData.bin";
    const char cBannerFileName[] = "banner.bin";
    const SaveFileSpec cSaveFileSpecTable[] = {{"mario", 0xF80}, {"luigi", 0xF80}, {"config", 0x60}};
    const SaveFileSpec cSaveFileSpecSystem = {"sysconf", 0x80};
};  // namespace

namespace {
    NEW_NERVE(SaveDataHandlerWait, SaveDataHandler, Wait);
    NEW_NERVE(SaveDataHandlerProcessing, SaveDataHandler, Processing);
    NEW_NERVE(SaveDataHandlerSaveProcessingGameData, SaveDataHandler, SaveProcessingGameData);
    NEW_NERVE(SaveDataHandlerSaveProcessingBanner, SaveDataHandler, SaveProcessingBanner);
    NEW_NERVE(SaveDataHandlerRemoveProcessingBanner, SaveDataHandler, RemoveProcessingBanner);
    NEW_NERVE(SaveDataHandlerRemoveProcessingGameData, SaveDataHandler, RemoveProcessingGameData);
};  // namespace

SaveDataHandler::SaveDataHandler(const SysConfigFile* pSysConfigFile, const UserFile* pUserFile)
    : NerveExecutor("SaveDataHandler"), mNANDRequestInfo(nullptr), _C(0), _10(0), _14(nullptr), _18(nullptr), mBannerCreator(nullptr) {
    mNANDRequestInfo = new NANDRequestInfo();

    createCommunicationBuffer();
    resetSaveData(_18);
    initializeAllFileInSaveData(_18, pSysConfigFile, pUserFile);

    mBannerCreator = new SaveDataBannerCreator();

    initNerve(&::SaveDataHandlerWait::sInstance);
}

void SaveDataHandler::update() {
    updateNerve();
    mBannerCreator->updateNerve();
}

// FIXME: Function isn't fetching `mNANDRequestInfo` again before call to `MR::addRequestToNANDManager`.
void SaveDataHandler::requestCheckEnableToCreate() {
    mNANDRequestInfo->setCheck(5, 2, &_10);
    MR::addRequestToNANDManager(mNANDRequestInfo);

    setNerve(&::SaveDataHandlerProcessing::sInstance);
}

void SaveDataHandler::requestLoadSaveData() {
    MR::zeroMemory(_14, 0x10000);
    mNANDRequestInfo->setReadSeq(::cSaveFileName, _14, 0x10000, &_C);
    MR::addRequestToNANDManager(mNANDRequestInfo);

    setNerve(&::SaveDataHandlerProcessing::sInstance);
}

bool SaveDataHandler::requestVerifyAfterLoadGameDataFile() {
    if (!isCorrectFileHeader(_14)) {
        return false;
    }

    SaveDataFileHeader* pFileHeader = reinterpret_cast< SaveDataFileHeader* >(_14);

    if (_C != OSRoundUp32B(pFileHeader->mFileSize)) {
        return false;
    }

    if (pFileHeader->mCheckSum != MR::calcCheckSum(_14 + sizeof(u32), pFileHeader->mFileSize - sizeof(u32))) {
        return false;
    }

    copySaveDataEachFile(_18, _14);

    return true;
}

void SaveDataHandler::initializeUserFileMemory(int index, const UserFile* pUserFile) {
    SaveDataFileAccessor fileAccessor = SaveDataFileAccessor(_18);

    for (u32 i = 0; i < sizeof(::cSaveFileSpecTable) / sizeof(*::cSaveFileSpecTable); i++) {
        char fileName[16];
        snprintf(fileName, sizeof(fileName), "%s%1d", ::cSaveFileSpecTable[i].mName, index);

        SaveDataUserFileInfo userFileInfo;
        fileAccessor.makeUserFileInfo(&userFileInfo, fileName);

        if (userFileInfo.mKind == 0) {
            pUserFile->makeGameDataBinary(userFileInfo.mData, userFileInfo.mDataSize);
        } else if (userFileInfo.mKind == 1) {
            pUserFile->makeConfigDataBinary(userFileInfo.mData, userFileInfo.mDataSize);
        }
    }
}

void SaveDataHandler::copyUserFileMemory(int indexDst, int indexSrc) {
    SaveDataFileAccessor fileAccessor = SaveDataFileAccessor(_18);

    for (u32 i = 0; i < sizeof(::cSaveFileSpecTable) / sizeof(*::cSaveFileSpecTable); i++) {
        char srcFileName[16];
        snprintf(srcFileName, sizeof(srcFileName), "%s%1d", ::cSaveFileSpecTable[i].mName, indexSrc);

        SaveDataUserFileInfo srcUserFileInfo;
        fileAccessor.makeUserFileInfo(&srcUserFileInfo, srcFileName);

        char dstFileName[16];
        snprintf(dstFileName, sizeof(dstFileName), "%s%1d", ::cSaveFileSpecTable[i].mName, indexDst);

        SaveDataUserFileInfo dstUserFileInfo;
        fileAccessor.makeUserFileInfo(&dstUserFileInfo, dstFileName);

        MR::copyMemory(dstUserFileInfo.mData, srcUserFileInfo.mData, dstUserFileInfo.mDataSize);
    }
}

void SaveDataHandler::restoreGameDataFile(const char* pUserFileName, void* pParam2, u32 param3) {
    SaveDataFileAccessor fileAccessor = SaveDataFileAccessor(_18);

    SaveDataUserFileInfo userFileInfo;
    fileAccessor.makeUserFileInfo(&userFileInfo, pUserFileName);

    MR::copyMemory(pParam2, userFileInfo.mData, userFileInfo.mDataSize);
}

void SaveDataHandler::storeUserFile(const UserFile* pUserFile) {
    SaveDataFileAccessor fileAccessor = SaveDataFileAccessor(_18);

    SaveDataUserFileInfo gameFileInfo;
    fileAccessor.makeUserFileInfo(&gameFileInfo, pUserFile->getGameDataName());
    pUserFile->makeGameDataBinary(gameFileInfo.mData, gameFileInfo.mDataSize);

    SaveDataUserFileInfo configFileInfo;
    fileAccessor.makeUserFileInfo(&configFileInfo, pUserFile->getConfigDataName());
    pUserFile->makeConfigDataBinary(configFileInfo.mData, configFileInfo.mDataSize);
}

void SaveDataHandler::storeSysConfigFile(const SysConfigFile* pSysConfigFile) {
    SaveDataFileAccessor fileAccessor = SaveDataFileAccessor(_18);

    SaveDataUserFileInfo userFileInfo;
    fileAccessor.makeUserFileInfo(&userFileInfo, ::cSaveFileSpecSystem.mName);
    pSysConfigFile->makeDataBinary(userFileInfo.mData, userFileInfo.mDataSize);
}

void SaveDataHandler::requestSaveSaveData() {
    SaveDataFileAccessor fileAccessorA = SaveDataFileAccessor(_18);
    SaveDataFileAccessor fileAccessorB = SaveDataFileAccessor(_14);

    MR::copyMemory(_14, _18, fileAccessorA.getHeader()->mFileSize);
    MR::fillMemory(_14 + fileAccessorA.getHeader()->mFileSize, 0, OSRoundUp32B(fileAccessorA.getHeader()->mFileSize));
    fileAccessorB.getHeader()->mCheckSum = MR::calcCheckSum(_14 + sizeof(u32), *(_14 + 0xC) - sizeof(u32));

    mNANDRequestInfo->setWriteSeq(::cSaveFileName, _14, OSRoundUp32B(fileAccessorB.getHeader()->mFileSize), 60, 0);
    MR::addRequestToNANDManager(mNANDRequestInfo);

    setNerve(&::SaveDataHandlerSaveProcessingGameData::sInstance);
}

void SaveDataHandler::requestRemoveSaveData() {
    setNerve(&::SaveDataHandlerRemoveProcessingBanner::sInstance);
}

u32 SaveDataHandler::getEnoughtTempBufferSize() {
    return 0xF80;
}

bool SaveDataHandler::isDone() const {
    bool result = isNerve(&::SaveDataHandlerWait::sInstance) && mNANDRequestInfo->isDone();

    return isNerve(&::SaveDataHandlerWait::sInstance);
}

NANDResultCode SaveDataHandler::getLastResultCode() const {
    return NANDResultCode(mNANDRequestInfo->mResult);
}

void SaveDataHandler::exeWait() {
}

void SaveDataHandler::exeProcessing() {
    if (!mNANDRequestInfo->isDone()) {
        return;
    }

    setNerve(&::SaveDataHandlerWait::sInstance);
}

void SaveDataHandler::exeSaveProcessingGameData() {
    bool isErr = false;

    if (!trySave(&isErr, mNANDRequestInfo->isDone())) {
        return;
    }

    if (!isErr) {
        setNerve(&::SaveDataHandlerSaveProcessingBanner::sInstance);
    } else {
        setNerve(&::SaveDataHandlerWait::sInstance);
    }
}

void SaveDataHandler::exeSaveProcessingBanner() {
    if (MR::isFirstStep(this)) {
        mBannerCreator->execute();
    } else if (mBannerCreator->isDone()) {
        NANDResultCode resultCode = mBannerCreator->getResultCode();
        mNANDRequestInfo->mResult = resultCode.getCode();

        if (resultCode.isSuccess()) {
            setNerve(&::SaveDataHandlerWait::sInstance);
            return;
        } else {
            setNerve(&::SaveDataHandlerWait::sInstance);
        }
    }
}

void SaveDataHandler::exeRemoveProcessingGameData() {
    bool isRemoved = false;

    if (!tryRemoveFile(::cSaveFileName, &isRemoved)) {
        return;
    }

    setNerve(&::SaveDataHandlerWait::sInstance);
}

void SaveDataHandler::exeRemoveProcessingBanner() {
    bool isRemoved = false;

    if (!tryRemoveFile(::cBannerFileName, &isRemoved)) {
        return;
    }

    if (isRemoved) {
        setNerve(&::SaveDataHandlerRemoveProcessingGameData::sInstance);
    } else {
        setNerve(&::SaveDataHandlerWait::sInstance);
    }
}

void SaveDataHandler::resetSaveData(u8* pBuffer) {
    int i;
    int j;
    SaveDataFile* pFile = reinterpret_cast< SaveDataFile* >(pBuffer);
    u32 offset = 0x140;

    pFile->mHeader.mCheckSum = 0;
    pFile->mHeader.mVersion = 2;
    pFile->mHeader.mUserFileInfoNum = 19;
    pFile->mHeader.mFileSize = 0;

    for (i = 0; i < 6; i++) {
        for (j = 0; j < sizeof(::cSaveFileSpecTable) / sizeof(*::cSaveFileSpecTable); j++) {
            SaveDataFileInfo* pFileInfo = &pFile->mInfo[i * j];

            MR::zeroMemory(pFileInfo->mName, sizeof(pFileInfo->mName));

            char fileName[32];
            snprintf(fileName, sizeof(fileName), "%s%1d", ::cSaveFileSpecTable[j].mName);

            snprintf(pFileInfo->mName, sizeof(pFileInfo->mName), "%s", fileName);
            pFileInfo->mOffset = offset;

            MR::zeroMemory(pBuffer + pFileInfo->mOffset, ::cSaveFileSpecTable[j].mBufferSize);
        }
    }

    SaveDataFileInfo* pFileInfo = &pFile->mInfo[i * j];

    MR::zeroMemory(pFileInfo->mName, sizeof(pFileInfo->mName));

    snprintf(pFileInfo->mName, sizeof(pFileInfo->mName), "%s", ::cSaveFileSpecSystem.mName);
    pFileInfo->mOffset = offset;

    MR::zeroMemory(pBuffer + pFileInfo->mOffset, ::cSaveFileSpecSystem.mBufferSize);

    pFile->mHeader.mFileSize = offset + ::cSaveFileSpecSystem.mBufferSize;
}

void SaveDataHandler::initializeAllFileInSaveData(u8* pParam1, const SysConfigFile* pSysConfigFile, const UserFile* pUserFile) {
    SaveDataFileAccessor fileAccessor = SaveDataFileAccessor(pParam1);

    for (int i = 0; i < fileAccessor.getHeader()->mUserFileInfoNum; i++) {
        SaveDataUserFileInfo userFileInfo;
        fileAccessor.makeUserFileInfo(&userFileInfo, fileAccessor.getFileInfo(i)->mName);

        if (userFileInfo.mKind == 0) {
            pUserFile->makeGameDataBinary(userFileInfo.mData, userFileInfo.mDataSize);
        } else if (userFileInfo.mKind == 1) {
            pUserFile->makeConfigDataBinary(userFileInfo.mData, userFileInfo.mDataSize);
        } else {
            pSysConfigFile->makeDataBinary(userFileInfo.mData, userFileInfo.mDataSize);
        }
    }
}

bool SaveDataHandler::isCorrectFileHeader(const u8* pBuffer) {
    SaveDataFileHeader* pFileHeader = SaveDataFileAccessor(const_cast< u8* >(pBuffer)).getHeader();

    if (pFileHeader->mVersion != 2) {
        return false;
    }

    if (pFileHeader->mFileSize >= 0x10000) {
        return false;
    }

    return pFileHeader->mUserFileInfoNum < 24;
}

void SaveDataHandler::copySaveDataEachFile(u8* pFileDst, const u8* pFileSrc) {
    SaveDataFileAccessor srcFileAccessor = SaveDataFileAccessor(const_cast< u8* >(pFileSrc));
    SaveDataFileAccessor dstFileAccessor = SaveDataFileAccessor(pFileDst);

    for (int i = 0; i < srcFileAccessor.getHeader()->mUserFileInfoNum; i++) {
        const char* pFileName = srcFileAccessor.getFileInfo(i)->mName;

        SaveDataUserFileInfo srcUserFileInfo;
        srcFileAccessor.makeUserFileInfo(&srcUserFileInfo, pFileName);

        SaveDataUserFileInfo dstUserFileInfo;
        dstFileAccessor.makeUserFileInfo(&dstUserFileInfo, pFileName);

        if (dstUserFileInfo.mData != nullptr) {
            int deltaSize = dstUserFileInfo.mDataSize - srcUserFileInfo.mDataSize;

            MR::copyMemory(dstUserFileInfo.mData, srcUserFileInfo.mData, dstUserFileInfo.mDataSize);

            if (deltaSize > 0) {
                MR::fillMemory(dstUserFileInfo.mData + dstUserFileInfo.mDataSize, 0, deltaSize);
            }
        }
    }
}

void SaveDataHandler::createCommunicationBuffer() {
    _14 = new (MR::getStationedHeapGDDR3(), 32) u8[0x10000];
    _18 = new (MR::getStationedHeapGDDR3(), 32) u8[0x10000];
}

// FIXME: Function isn't fetching `mNANDRequestInfo` again before call to `MR::addRequestToNANDManager`.
bool SaveDataHandler::tryRemoveFile(const char* pFileName, bool* pIsRemoved) {
    if (MR::isFirstStep(this)) {
        mNANDRequestInfo->setDelete(pFileName);
        MR::addRequestToNANDManager(mNANDRequestInfo);
    } else if (mNANDRequestInfo->isDone()) {
        NANDResultCode resultCode = NANDResultCode(mNANDRequestInfo->mResult);

        if (resultCode.isSuccess() || resultCode.isNoExistFile()) {
            *pIsRemoved = true;
            return true;
        } else {
            *pIsRemoved = false;
            return true;
        }
    }

    return false;
}

bool SaveDataHandler::trySave(bool* pIsErr, bool isDone) {
    if (!isDone) {
        return false;
    }

    NANDResultCode resultCode = NANDResultCode(mNANDRequestInfo->mResult);

    if (resultCode.isSuccess()) {
        *pIsErr = false;
        return true;
    } else {
        *pIsErr = true;
        return true;
    }
}
