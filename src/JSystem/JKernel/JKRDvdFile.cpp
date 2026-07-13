#include "JSystem/JKernel/JKRDvdFile.hpp"
#include "JSystem/JUtility/JUTException.hpp"
#include <stdint.h>

JSUList< JKRDvdFile > JKRDvdFile::sDvdList;

JKRDvdFile::JKRDvdFile() : mDvdLink(this) {
    initiate();
}

JKRDvdFile::JKRDvdFile(s32 entryNum) : mDvdLink(this) {
    initiate();
    mIsAvailable = open(entryNum);
    if (!mIsAvailable) {
        return;
    } else {
        return;
    }
}

JKRDvdFile::~JKRDvdFile() {
    close();
}

void JKRDvdFile::initiate(void) {
    mDvdFile = this;
    OSInitMutex(&mMutex1);
    OSInitMutex(&mMutex2);
    OSInitMessageQueue(&mMessageQueue2, &mMessage2, 1);
    OSInitMessageQueue(&mMessageQueue1, &mMessage1, 1);
    mOSThread = NULL;
    field_0x50 = 0;
    field_0x58 = 0;
}

bool JKRDvdFile::open(const char* name) {
    if (!mIsAvailable) {
        mIsAvailable = DVDOpen(name, &mFileInfo);
        if (mIsAvailable) {
            sDvdList.append(&mDvdLink);
            getStatus();
        }
    }
    return mIsAvailable;
}

bool JKRDvdFile::open(s32 entryNum) {
    if (!mIsAvailable) {
        mIsAvailable = DVDFastOpen(entryNum, &mFileInfo);
        if (mIsAvailable) {
            sDvdList.append(&mDvdLink);
            getStatus();
        }
    }
    return mIsAvailable;
}

void JKRDvdFile::close() {
    if (mIsAvailable) {
        if (DVDClose(&mFileInfo) != 0) {
            mIsAvailable = false;
            sDvdList.remove(&mDvdLink);
        } else {
            JUTException::panic(__FILE__, 213, "cannot close DVD file\n");
        }
    }
}

s32 JKRDvdFile::readData(void* param_1, s32 length, s32 param_3) {
    OSLockMutex(&mMutex1);
    if (mOSThread) {
        OSUnlockMutex(&mMutex1);
        return -1;
    }

    mOSThread = OSGetCurrentThread();

    s32 result = -1;
    if (DVDReadAsyncPrio(&mFileInfo, param_1, length, param_3, JKRDvdFile::doneProcess, 2)) {
        result = sync();
    }

    mOSThread = NULL;
    OSUnlockMutex(&mMutex1);

    return result;
}

s32 JKRDvdFile::writeData(void const* param_0, s32 length, s32 param_2) {
    return -1;
}

s32 JKRDvdFile::sync(void) {
    OSMessage message;
    OSLockMutex(&mMutex1);
    OSReceiveMessage(&mMessageQueue2, &message, 1);
    mOSThread = NULL;
    OSUnlockMutex(&mMutex1);
    return (intptr_t)message;
}

void JKRDvdFile::doneProcess(s32 id, DVDFileInfo* fileInfo) {
    JKRDvdFile* dvdFile = *(JKRDvdFile**)((u8*)fileInfo + 0x3c);
    OSSendMessage(&dvdFile->mMessageQueue2, (OSMessage)(intptr_t)id, OS_MESSAGE_NOBLOCK);
}

JKRFile::JKRFile() : JKRDisposer() {
    mIsAvailable = false;
}

s32 JKRDvdFile::getFileSize(void) const {
    return mFileInfo.length;
}
