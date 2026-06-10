#include "JSystem/JAudio2/JAUStreamFileTable.hpp"

JAUStreamFileTable::JAUStreamFileTable() {
    mData = nullptr;
}

void JAUStreamFileTable::init(void const* data) {
    const BinaryStreamFileTable* binaryTable = (const BinaryStreamFileTable*)data;
    if (binaryTable == nullptr) {
        mData = nullptr;
        return;
    }
    if (binaryTable->mIdentifier[0] == 'b' && binaryTable->mIdentifier[1] == 's' && binaryTable->mIdentifier[2] == 'f' &&
        binaryTable->mIdentifier[3] == 't') {
        mData = binaryTable;
    }
}

u32 JAUStreamFileTable::getNumFiles() const {
    return mData->mNumFiles;
}

const char* JAUStreamFileTable::getFilePath(int index) const {
    if (mData == nullptr) {
        return 0;
    }

    char* data = (char*)mData;
    return (char*)(data + *(int*)(data + 8 + (index * sizeof(s32))));
}

s32 JAUStreamDataMgr_StreamFileTable::getStreamFileEntry(JAISoundID soundID) {
    const char* filePath = getFilePath(soundID.getWaveID());
    if (filePath == nullptr) {
        return -1;
    }
    return DVDConvertPathToEntrynum(filePath);
}
