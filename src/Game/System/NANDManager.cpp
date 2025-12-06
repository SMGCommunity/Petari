#include "Game/System/NANDManager.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/NANDManagerThread.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <cstdio>

NANDRequestInfo::NANDRequestInfo() {
    init();
}

void NANDRequestInfo::init() {
    _40 = 0;
    mType = 4;
    mResult = NAND_RESULT_OK;
    mPath[0] = '\0';
    mFsBlock = 0;
    mReadBuf = nullptr;
    mReadLength = nullptr;
    _54 = nullptr;
}

bool NANDRequestInfo::isDone() const {
    return _40 == 0;
}

const char* NANDRequestInfo::setMove(const char* pPath, const char* pDestDir) {
    init();

    mType = 0;
    snprintf(mPath, sizeof(mPath), "%s", pPath);
    mMoveDestDir = pDestDir;

    return mPath;
}

const char* NANDRequestInfo::setWriteSeq(const char* pName, const void* pBuf, u32 fsBlock, u8 permission, u8 attr) {
    init();

    mWriteBuf = pBuf;
    mType = 2;
    mFsBlock = fsBlock;
    mPermission = permission;
    mAttribute = attr;
    snprintf(mPath, sizeof(mPath), "%s", pName);

    return mPath;
}

const char* NANDRequestInfo::setReadSeq(const char* pName, void* pBuf, u32 fsBlock, u32* pLength) {
    init();

    mReadBuf = pBuf;
    mType = 3;
    mFsBlock = fsBlock;
    mReadLength = pLength;
    snprintf(mPath, sizeof(mPath), "%s", pName);

    return mPath;
}

const char* NANDRequestInfo::setCheck(u32 fsBlock, u32 inode, u32* pAnswer) {
    init();

    mFsBlock = fsBlock;
    mType = 4;
    mInode = inode;
    mCheckAnswer = pAnswer;

    return mPath;
}

NANDManager::NANDManager() : mManagerThread(nullptr) {
    mManagerThread = new NANDManagerThread(13, 256, MR::getStationedHeapNapa());

    OSResumeThread(mManagerThread->mThread);
    OSInitMutex(&mMutex);
}

bool NANDManager::addRequest(NANDRequestInfo* pRequestInfo) {
    OSLockMutex(&mMutex);
    pRequestInfo->_40 = 1;
    bool ret = OSSendMessage(&mManagerThread->mQueue, pRequestInfo, OS_MESSAGE_NOBLOCK);
    OSUnlockMutex(&mMutex);

    return ret;
}

s32 NANDResultCode::getCode() const {
    return mCode;
}

bool NANDResultCode::isSuccess() const {
    return getCode() == NAND_RESULT_OK;
}

bool NANDResultCode::isSaveDataCorrupted() const {
    return getCode() == NAND_RESULT_ECC_CRIT || getCode() == NAND_RESULT_AUTHENTICATION;
}

bool NANDResultCode::isNANDCorrupted() const {
    return getCode() == NAND_RESULT_CORRUPT;
}

bool NANDResultCode::isMaxBlocks() const {
    return getCode() == NAND_RESULT_MAXBLOCKS;
}

bool NANDResultCode::isMaxFiles() const {
    return getCode() == NAND_RESULT_MAXFILES;
}

bool NANDResultCode::isNoExistFile() const {
    return getCode() == NAND_RESULT_NOEXISTS;
}

bool NANDResultCode::isBusyOrAllocFailed() const {
    return getCode() == NAND_RESULT_BUSY || getCode() == NAND_RESULT_ALLOC_FAILED;
}

bool NANDResultCode::isUnknown() const {
    return getCode() == NAND_RESULT_UNKNOWN;
}

namespace MR {
    void addRequestToNANDManager(NANDRequestInfo* pRequestInfo) { SingletonHolder< NANDManager >::get()->addRequest(pRequestInfo); }
};  // namespace MR

const char* NANDRequestInfo::setDelete(const char* pName) {
    init();
    mType = 1;
    snprintf(mPath, sizeof(mPath), "%s", pName);
    return mPath;
}
