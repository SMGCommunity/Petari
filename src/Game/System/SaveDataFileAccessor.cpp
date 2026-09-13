#include "Game/System/SaveDataFileAccessor.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstring>

SaveDataFileAccessor::SaveDataFileAccessor(u8* pSaveDataFile) : mFile(reinterpret_cast< SaveDataFile* >(pSaveDataFile)) {
}

SaveDataFileHeader* SaveDataFileAccessor::getHeader() {
    return &mFile->mHeader;
}

SaveDataFileInfo* SaveDataFileAccessor::getFileInfo(int index) {
    return &mFile->mInfo[index];
}

void SaveDataFileAccessor::makeUserFileInfo(SaveDataUserFileInfo* pUserFileInfo, const char* pName) {
    pUserFileInfo->mData = nullptr;
    pUserFileInfo->mDataSize = 0;
    pUserFileInfo->mKind = 1;

    SaveDataFile* pFile = mFile;
    for (s32 i = 0; i < pFile->mHeader.mUserFileInfoNum; i++) {
        if (!MR::isEqualString(pFile->mInfo[i].mName, pName)) {
            continue;
        }

        if (i == pFile->mHeader.mUserFileInfoNum - 1) {
            pUserFileInfo->mDataSize = pFile->mHeader.mFileSize - pFile->mInfo[i].mOffset;
        } else {
            pUserFileInfo->mDataSize = pFile->mInfo[i + 1].mOffset - pFile->mInfo[i].mOffset;
        }

        SaveDataFileInfo* pFileInfo = &pFile->mInfo[i];
        pUserFileInfo->mData = (u8*)(&mFile->mHeader) + pFileInfo->mOffset;

        if (strstr(pFile->mInfo[i].mName, "mario") != nullptr || strstr(pFile->mInfo[i].mName, "luigi") != nullptr) {
            pUserFileInfo->mKind = 0;
        }

        if (strstr(pFile->mInfo[i].mName, "sysconf")) {
            pUserFileInfo->mKind = 2;
        }
    }
}
