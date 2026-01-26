#include "JSystem/JAudio2/JASResArcLoader.hpp"
#include "JSystem/JAudio2/JASDvdThread.hpp"
#include "JSystem/JAudio2/JASTaskThread.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"

u32 JASResArcLoader::getResSize(JKRArchive const* archive, u16 id) {
    JKRArchive::SDIFileEntry* fileEntry = archive->findIdResource(id);
    if (fileEntry == nullptr) {
        return 0;
    }
    return fileEntry->mDataSize;
}

static void JASResArcLoader::loadResourceCallback(void* args) {
    TLoadResInfo* castedArgs = (TLoadResInfo*)args;
    u32 readResult = castedArgs->mArchive->readResource(castedArgs->mBuffer, castedArgs->mBufferSize, castedArgs->mResID);
    if (castedArgs->mCallback != nullptr) {
        castedArgs->mCallback(readResult, castedArgs->mCallbackArg);
    }

    if (readResult == 0) {
        if (castedArgs->mMsgQueue != nullptr) {
            OSSendMessage(castedArgs->mMsgQueue, (void*)RESARC_MESSAGE_ERROR, OS_MESSAGE_BLOCK);
        }
    } else if (castedArgs->mMsgQueue != nullptr) {
        OSSendMessage(castedArgs->mMsgQueue, (void*)RESARC_MESSAGE_SUCCESS, OS_MESSAGE_BLOCK);
    }
}

int JASResArcLoader::loadResourceAsync(JKRArchive* archive, u16 id, u8* buffer, u32 size, LoadCallback callback, u32 callbackArg) {
    TLoadResInfo args(archive, id, buffer, size);
    args.mCallback = callback;
    args.mCallbackArg = callbackArg;

    int result = JASDvd::getThreadPointer()->sendCmdMsg(&loadResourceCallback, (void*)&args, sizeof(TLoadResInfo));
    return result;
}
