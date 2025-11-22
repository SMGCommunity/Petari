#include "Game/System/NANDManagerThread.hpp"
#include "Game/System/NANDManager.hpp"

NANDManagerThread::NANDManagerThread(int priority, int msgCount, JKRHeap* pHeap)
    : OSThreadWrapper(0x8000, msgCount, priority, pHeap) {
}

void* NANDManagerThread::run() {
    OSInitFastCast();

    while (true) {
        OSMessage msg;
        OSReceiveMessage(&mQueue, &msg, OS_MESSAGE_BLOCK);

        NANDRequestInfo* pRequestInfo = static_cast< NANDRequestInfo* >(msg);
        pRequestInfo->_40 = 2;

        switch (pRequestInfo->mType) {
        case 1:
            pRequestInfo->mResult = NANDDelete(pRequestInfo->mPath);
            break;
        case 0:
            pRequestInfo->mResult = NANDMove(pRequestInfo->mPath, pRequestInfo->mMoveDestDir);
            break;
        case 2:
            executeWriteSequence(pRequestInfo);
            break;
        case 3:
            executeReadSequence(pRequestInfo);
            break;
        case 4:
            pRequestInfo->mResult = NANDCheck(pRequestInfo->mFsBlock, pRequestInfo->mInode, pRequestInfo->mCheckAnswer);
            break;
        }

        pRequestInfo->_40 = 0;

        if (pRequestInfo->_54 != nullptr) {
            (pRequestInfo->_54)(pRequestInfo);
        }
    }
}

void NANDManagerThread::executeWriteSequence(NANDRequestInfo* pRequestInfo) {
    pRequestInfo->mResult = NANDCreate(pRequestInfo->mPath, pRequestInfo->mPermission, pRequestInfo->mAttribute);

    if (pRequestInfo->mResult == NAND_RESULT_OK || pRequestInfo->mResult == NAND_RESULT_EXISTS) {
        NANDFileInfo info;
        pRequestInfo->mResult = NANDOpen(pRequestInfo->mPath, &info, NAND_ACCESS_WRITE);

        if (pRequestInfo->mResult != NAND_RESULT_OK) {
            pRequestInfo->mFsBlock = 0;
        } else {
            pRequestInfo->mResult = NANDWrite(&info, pRequestInfo->mWriteBuf, pRequestInfo->mFsBlock);

            if (pRequestInfo->mResult < NAND_RESULT_OK) {
                NANDClose(&info);
                pRequestInfo->mFsBlock = 0;
            } else {
                pRequestInfo->mFsBlock = pRequestInfo->mResult;
                pRequestInfo->mResult = NANDClose(&info);
            }
        }
    }
}

void NANDManagerThread::executeReadSequence(NANDRequestInfo* pRequestInfo) {
    NANDFileInfo info;
    pRequestInfo->mResult = NANDOpen(pRequestInfo->mPath, &info, NAND_ACCESS_READ);

    if (pRequestInfo->mResult != NAND_RESULT_OK) {
        pRequestInfo->mFsBlock = 0;
        return;
    }

    u32 length = 0;
    pRequestInfo->mResult = NANDGetLength(&info, &length);

    if (pRequestInfo->mResult != NAND_RESULT_OK) {
        NANDClose(&info);
        pRequestInfo->mFsBlock = 0;
        return;
    }

    if (pRequestInfo->mReadLength != nullptr) {
        *pRequestInfo->mReadLength = length;
    }

    if (pRequestInfo->mFsBlock < length) {
        pRequestInfo->mFsBlock = 0;
        pRequestInfo->mResult = NAND_RESULT_AUTHENTICATION;
        NANDClose(&info);
    } else {
        pRequestInfo->mResult = NANDRead(&info, pRequestInfo->mReadBuf, length);

        if (pRequestInfo->mResult < NAND_RESULT_OK) {
            NANDClose(&info);
            pRequestInfo->mFsBlock = 0;
        } else {
            pRequestInfo->mFsBlock = pRequestInfo->mResult;
            pRequestInfo->mResult = NANDClose(&info);
        }
    }
}
