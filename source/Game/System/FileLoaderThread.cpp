#include "Game/System/FileLoaderThread.h"
#include "Game/System/FileRipper.h"

#include "Inline.h"

namespace {
    void* loadFileUsingRipper(RequestFileInfo *pInfo) NO_INLINE {
        s32 val = 0;

        if (pInfo->mRequestType != 1) {
            val = 1;
        }

        return FileRipper::loadToMainRAM(pInfo->mFileName, (u8*)pInfo->mFileEntry->mContents, true, pInfo->mFileEntry->mHeap, (FileRipper::AllocDirection)val);
    }
};

FileLoaderThread::FileLoaderThread(int a1, int a2, JKRHeap *pHeap) : OSThreadWrapper(0x8000, a2, a1, pHeap) {

} 

s32 FileLoaderThread::run() {
    // OSInitFastCast
    __asm {
        li r3, 4
        oris r3, r3, 4
        mtspr 0x392, r3
        li r3, 5
        oris r3, r3, 5
        mtspr 0x393, r3
        li r3, 6
        oris r3, r3, 6
        mtspr 0x394, r3
        li r3, 7
        oris r3, r3, 7
        mtspr 0x395, r3
    };

    while (true) {
        OSMessage msg;
        OSReceiveMessage(&mQueue, &msg, 1);
        RequestFileInfo* info = (RequestFileInfo*)msg;

        switch (info->_0) {
            case 0:
                loadToMainRAM(info);
                break;
            case 1:
                mountArchiveAndStartCreateResource(info);
                break;
        }
    }
}

/* this function matches in other compiler versions */
void FileLoaderThread::loadToMainRAM(RequestFileInfo *pInfo) {
    pInfo->_88 = 1;
    void* data = ::loadFileUsingRipper(pInfo);
    pInfo->mFileEntry->setContext(data, pInfo->mFileEntry->mHeap);
    pInfo->_88 = 2;
}

/* same with this one */
void FileLoaderThread::mountArchiveAndStartCreateResource(RequestFileInfo *pInfo) {
    pInfo->_88 = 1;
    void* data = ::loadFileUsingRipper(pInfo);
    MR::createAndAddArchive(data, pInfo->mFileEntry->mHeap, pInfo->mFileName);
    pInfo->mFileEntry->setContext(data, pInfo->mFileEntry->mHeap);
    pInfo->_88 = 2;
}

FileLoaderThread::~FileLoaderThread() {

}
