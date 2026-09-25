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

    SaveDataFileHeader* pHeader = getHeader();
    SaveDataFileInfo* pInfo = getFileInfo(0);

    for (u32 i = 0; i < pHeader->mUserFileInfoNum; i++) {
        if (!MR::isEqualString(pInfo[i].mName, pName)) {
            continue;
        }

        if (i == pHeader->mUserFileInfoNum - 1) {
            pUserFileInfo->mDataSize = pHeader->mFileSize - pInfo[i].mOffset;
        } else {
            pUserFileInfo->mDataSize = pInfo[i + 1].mOffset - pInfo[i].mOffset;
        }

        SaveDataFileInfo* pFileInfo = &pInfo[i];
        pUserFileInfo->mData = reinterpret_cast< u8* >(getHeader()) + pFileInfo->mOffset;

        if (strstr(pFileInfo->mName, "mario") != nullptr || strstr(pFileInfo->mName, "luigi") != nullptr) {
            pUserFileInfo->mKind = 0;
        }

        if (strstr(pFileInfo->mName, "sysconf") != nullptr) {
            pUserFileInfo->mKind = 2;
        }
    }
}
