#include "Game/System/NANDManagerThread.hpp"
#include "Game/System/NANDManager.hpp"

NANDManagerThread::NANDManagerThread(int priority, int msgCount, JKRHeap *pHeap) :
    OSThreadWrapper(0x8000, msgCount, priority, pHeap)
{
    
}

void* NANDManagerThread::run() {
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
        OSReceiveMessage(&mQueue, &msg, OS_MESSAGE_BLOCK);

        NANDRequestInfo* pInfo = static_cast<NANDRequestInfo*>(msg);
        pInfo->_40 = 2;

        switch (pInfo->mRequestStatus) {
            case 1:
                pInfo->mRequestResult = NANDDelete(pInfo->mReqStr);
                break;
            case 0:
                pInfo->mRequestResult = NANDMove(pInfo->mReqStr, static_cast<const char*>(pInfo->_50));
                break;
            case 2:
                executeWriteSequence(pInfo);
                break;
            case 3:
                executeReadSequence(pInfo);
                break;
            case 4:
                pInfo->mRequestResult = NANDCheck(pInfo->mFSBlock, pInfo->mINode, static_cast<u32*>(pInfo->_50));
                break;
        }

        pInfo->_40 = 0;
       
        if (pInfo->_54 != nullptr) {
            (pInfo->_54)(pInfo);
        }
    }
}

void NANDManagerThread::executeWriteSequence(NANDRequestInfo *pInfo) {
    pInfo->mRequestResult = NANDCreate(pInfo->mReqStr, pInfo->mPermission, pInfo->mAttribute);

    if (pInfo->mRequestResult == NAND_RESULT_OK || pInfo->mRequestResult == NAND_RESULT_EXISTS) {
        NANDFileInfo info;
        pInfo->mRequestResult = NANDOpen(pInfo->mReqStr, &info, NAND_ACCESS_WRITE);

        if (pInfo->mRequestResult != NAND_RESULT_OK) {
            pInfo->mFSBlock = 0;
        }
        else {
            pInfo->mRequestResult = NANDWrite(&info, pInfo->_4C, pInfo->mFSBlock);

            if (pInfo->mRequestResult < NAND_RESULT_OK) {
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
    pInfo->mRequestResult = NANDOpen(pInfo->mReqStr, &info, NAND_ACCESS_READ);

    if (pInfo->mRequestResult != NAND_RESULT_OK) {
        pInfo->mFSBlock = 0;
        return;
    }

    u32 length = 0;
    pInfo->mRequestResult = NANDGetLength(&info, &length);

    if (pInfo->mRequestResult != NAND_RESULT_OK) {
        NANDClose(&info);
        pInfo->mFSBlock = 0;
        return;
    }

    if (pInfo->_50 != nullptr) {
        *static_cast<u32*>(pInfo->_50) = length;
    }

    if (pInfo->mFSBlock < length) {
        pInfo->mFSBlock = 0;
        pInfo->mRequestResult = NAND_RESULT_AUTHENTICATION;
        NANDClose(&info);
    }
    else {
        pInfo->mRequestResult = NANDRead(&info, const_cast<void*>(pInfo->_4C), length);

        if (pInfo->mRequestResult < NAND_RESULT_OK) {
            NANDClose(&info);
            pInfo->mFSBlock = 0;
        }
        else {
            pInfo->mFSBlock = pInfo->mRequestResult;
            pInfo->mRequestResult = NANDClose(&info);
        }
    }
}
