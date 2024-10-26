#include "Game/System/NANDManager.hpp"
#include "Game/Util.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <cstdio>

NANDRequestInfo::NANDRequestInfo() {
    init();
}

void NANDRequestInfo::init() {
    _40 = 0;
    mRequestStatus = 4;
    mRequestResult = 0;
    mReqStr[0] = 0;
    mFSBlock = 0;
    _4C = nullptr;
    _50 = nullptr;
    _54 = 0;
}

bool NANDRequestInfo::isDone() const {
    return mRequestStatus == 0;
}

const char* NANDRequestInfo::setMove(const char *pDest, const char *pSrc) {
    init();
    mRequestStatus = 0;
    snprintf(mReqStr, sizeof(mReqStr), "%s", pDest);
    _50 = (void*)pSrc;
    return mReqStr;
}

const char* NANDRequestInfo::setWriteSeq(const char *pName, const void *a2, u32 block, u8 permission, u8 attr) {
    init();
    _4C = a2;
    mRequestStatus = 2;
    mFSBlock = block;
    mPermission = permission;
    mAttribute = attr;
    snprintf(mReqStr, sizeof(mReqStr), "%s", pName);
    return mReqStr;
}

const char* NANDRequestInfo::setReadSeq(const char *pName, void *a2, u32 block, u32 *a4) {
    init();
    _4C = a2;
    mRequestStatus = 3;
    mFSBlock = block;
    _50 = a4;
    snprintf(mReqStr, sizeof(mReqStr), "%s", pName);
    return mReqStr;
}

const char* NANDRequestInfo::setCheck(u32 block, u32 node, u32 *a3) {
    init();
    mFSBlock = block;
    mRequestStatus = 4;
    mINode = node;
    _50 = a3;
    return mReqStr;
}

NANDManager::NANDManager() {
    mManager = nullptr;
    mManager = new NANDManagerThread(0xD, 0x100, MR::getStationedHeapNapa());
    OSResumeThread(mManager->mThread);
    OSInitMutex(&mMutex);
}

bool NANDManager::addRequest(NANDRequestInfo *pReq) {
    OSLockMutex(&mMutex);
    pReq->_40 = 1;
    bool ret = OSSendMessage(&mManager->mQueue, pReq, 0);
    OSUnlockMutex(&mMutex);
    return ret;
}

s32 NANDResultCode::getCode() const {
    return mCode;
}

bool NANDResultCode::isSuccess() const {
    return mCode == 0;
}

bool NANDResultCode::isSaveDataCorrupted() const {
    return (mCode == -5 || mCode == -15);
}

bool NANDResultCode::isNANDCorrupted() const {
    return mCode == -4;
}

bool NANDResultCode::isMaxBlocks() const {
    return mCode == -9;
}

bool NANDResultCode::isMaxFiles() const {
    return mCode == -11;
}

bool NANDResultCode::isNoExistFile() const {
    return mCode == -12;
}

bool NANDResultCode::isBusyOrAllocFailed() const {
    return (mCode == -3 || mCode == -2);
}

bool NANDResultCode::isUnknown() const {
    return mCode == -64;
}

namespace MR {
    void addRequestToNANDManager(NANDRequestInfo *pInfo) {
        SingletonHolder<NANDManager>::sInstance->addRequest(pInfo);
    }
};

const char* NANDRequestInfo::setDelete(const char *pName) {
    init();
    mRequestStatus = 1;
    snprintf(mReqStr, sizeof(mReqStr), "%s", pName);
    return mReqStr;
}