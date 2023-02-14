#include "Game/System/NANDManagerThread.h"
#include "Game/System/NANDManager.h"

NANDManagerThread::NANDManagerThread(int a1, int a2, JKRHeap *pHeap) : OSThreadWrapper(0x8000, a2, a1, pHeap) {

}

s32 NANDManagerThread::run() {
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
        NANDRequestInfo* info = (NANDRequestInfo*)msg;
        info->_40 = 2;

        switch (info->mRequestStatus) {
            case 1:
                info->mRequestResult = NANDDelete(info->mReqStr);
                break;
            case 0:
                info->mRequestResult = NANDMove(info->mReqStr, (const char*)info->_50);
                break;
            case 2:
                executeWriteSequence(info);
                break;
            case 3:
                executeReadSequence(info);
                break;
            case 4:
                info->mRequestResult = NANDCheck(info->mFSBlock, info->mINode, (u32*)info->_50);
                break;
        }

        info->_40 = 0;
       
        if (info->_54 != nullptr) {
            (info->_54)(info);
        }
    }
}

void NANDManagerThread::executeWriteSequence(NANDRequestInfo *pInfo) {
    pInfo->mRequestResult = NANDCreate(pInfo->mReqStr, pInfo->mPermission, pInfo->mAttribute);
    if (pInfo->mRequestResult == 0 || pInfo->mRequestResult == -6) {
        NANDFileInfo info;
        pInfo->mRequestResult = NANDOpen(pInfo->mReqStr, &info, 2);

        if (pInfo->mRequestResult != 0) {
            pInfo->mFSBlock = 0;
        }
        else {
            pInfo->mRequestResult = NANDWrite(&info, pInfo->_4C, pInfo->mFSBlock);

            if (pInfo->mRequestResult < 0) {
                NANDClose(&info);
                pInfo->mFSBlock = 0;
            }
            else {
                pInfo->mFSBlock = pInfo->mRequestResult;
                pInfo->mRequestResult = NANDClose(&info);
            }
        }
    }
}

void NANDManagerThread::executeReadSequence(NANDRequestInfo *pInfo) {
    NANDFileInfo info;
    pInfo->mRequestResult = NANDOpen(pInfo->mReqStr, &info, 1);

    if (pInfo->mRequestResult != 0) {
        pInfo->mFSBlock = 0;
        return;
    }

    u32 length = 0;
    pInfo->mRequestResult = NANDGetLength(&info, &length);

    if (pInfo->mRequestResult != 0) {
        NANDClose(&info);
        pInfo->mFSBlock = 0;
        return;
    }

    if (pInfo->_50 != nullptr) {
        *(u32*)(pInfo->_50) = length;
    }

    if (pInfo->mFSBlock < length) {
        pInfo->mFSBlock = 0;
        pInfo->mRequestResult = -15;
        NANDClose(&info);
    }
    else {
        pInfo->mRequestResult = NANDRead(&info, (void*)pInfo->_4C, length);

        if (pInfo->mRequestResult < 0) {
            NANDClose(&info);
            pInfo->mFSBlock = 0;
            return;
        }
        else {
            pInfo->mFSBlock = pInfo->mRequestResult;
            pInfo->mRequestResult = NANDClose(&info);
        }
    }
}

NANDManagerThread::~NANDManagerThread() {

}
