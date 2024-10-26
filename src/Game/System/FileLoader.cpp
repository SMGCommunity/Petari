#include "Game/System/FileLoader.hpp"
#include "Game/Util.hpp"

FileLoader::FileLoader() {
    mLoaderThread = nullptr;
    mRequestFileInfos = nullptr;
    mRequestedFileCount = 0;
    mFileHolder = nullptr;
    mArchiveHolder = nullptr;
    mLoaderThread = new FileLoaderThread(13, 320, MR::getStationedHeapNapa());
    OSResumeThread(mLoaderThread->mThread);
    OSInitMutex(&mMutex);
    mRequestFileInfos = new RequestFileInfo[0x140];
    mFileHolder = new FileHolder();
    mArchiveHolder = new ArchiveHolder();
    mRequestedFileCount = 0;
}

void FileLoader::requestLoadToMainRAM(const char *pName, u8 *pData, JKRHeap* pHeap, JKRDvdRipper::EAllocDirection direction, bool jam) {
    if (isNeedToLoad(pName)) {
        if (pHeap == nullptr) {
            pHeap = MR::getCurrentHeap();
        }

        RequestFileInfo* info = addRequest(pName);
        info->_0 = 0;
        info->mRequestType = direction;
        info->mFileEntry = mFileHolder->add(pName, pHeap, pData);

        if (jam) {
            OSJamMessage(&mLoaderThread->mQueue, info, OS_MESSAGE_NOBLOCK);
        }
        else {
            OSSendMessage(&mLoaderThread->mQueue, info, OS_MESSAGE_NOBLOCK);
        }
    }
}

void FileLoader::requestMountArchive(const char *pName, JKRHeap* pHeap, bool jam) {
    if (isNeedToLoad(pName)) {
        if (pHeap == nullptr) {
            pHeap = MR::getCurrentHeap();
        }

        RequestFileInfo* info = addRequest(pName);
        info->_0 = 1;
        info->mRequestType = 1;
        info->mFileEntry = mFileHolder->add(pName, pHeap, nullptr);

        if (jam) {
            OSJamMessage(&mLoaderThread->mQueue, info, OS_MESSAGE_NOBLOCK);
        }
        else {
            OSSendMessage(&mLoaderThread->mQueue, info, OS_MESSAGE_NOBLOCK);
        }
    }
}

bool FileLoader::isLoaded(const char *pName) const {
    return mFileHolder->isExist(pName);
}

bool FileLoader::isMountedArchive(const char *pName) const {
    return mArchiveHolder->getArchive(pName) != nullptr;
}

void* FileLoader::receiveFile(const char *pName) {
    const RequestFileInfo* info = getRequestFileInfoConst(pName);
    if (info != nullptr) {
        info->mFileEntry->waitReadDone();
    }

    return mFileHolder->getContext(pName);
}

JKRMemArchive* FileLoader::receiveArchive(const char *pName) {
    const RequestFileInfo* info = getRequestFileInfoConst(pName);
    if (info != nullptr) {
        info->mFileEntry->waitReadDone();
    }

    return mArchiveHolder->getArchive(pName);
}

void FileLoader::receiveAllRequestedFile() {
    for (u32 i = 0; i < mRequestedFileCount; i++) {
        mRequestFileInfos[i].mFileEntry->waitReadDone();
    }
}

JKRMemArchive* FileLoader::createAndAddArchive(void* pData, JKRHeap* pHeap, const char* pName) {
    return mArchiveHolder->createAndAdd(pData, pHeap, pName)->mArchive;
}

void FileLoader::getMountedArchiveAndHeap(const char* pName, JKRArchive** pArchive, JKRHeap** pHeap) const {
    mArchiveHolder->getArchiveAndHeap(pName, pArchive, pHeap);
}

void FileLoader::clearRequestFileInfo(bool) {
    mRequestedFileCount = nullptr;
}

void FileLoader::removeHolderIfIsEqualHeap(JKRHeap* pHeap) {
    mFileHolder->removeIfIsEqualHeap(pHeap);
    mArchiveHolder->removeIfIsEqualHeap(pHeap);
}

void FileLoader::removeFile(const char* pName) {
    mFileHolder->removeFile(pName);
}

bool FileLoader::isNeedToLoad(const char* pName) const {
    const RequestFileInfo* info = getRequestFileInfoConst(pName);
    return (info == nullptr) ? false : mFileHolder->isExist(pName) == false;
}

const RequestFileInfo* FileLoader::getRequestFileInfoConst(const char* pName) const {
    for (u32 i = 0; i < mRequestedFileCount; i++) {
        RequestFileInfo* info = &mRequestFileInfos[i];

        if (MR::isEqualStringCase(info->mFileName, pName)) {
            return info;
        }
    }

    return nullptr;
}

#ifdef NON_MATCHING
// string load is in the wrong spot for some reason
RequestFileInfo* FileLoader::addRequest(const char* pName) {
    OSMutex* mutex = &mMutex;
    OSLockMutex(mutex);
    s32 count = mRequestedFileCount;
    mRequestedFileCount = count + 1;
    RequestFileInfo* info = &mRequestFileInfos[count];
    info->mFileEntry = nullptr;
    info->_88 = 0;
    snprintf(info->mFileName, sizeof(info->mFileName), "%s", pName);
    OSUnlockMutex(mutex);
    return info;
}
#endif