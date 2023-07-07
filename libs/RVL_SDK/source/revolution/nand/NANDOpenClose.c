#include <revolution/nand.h>
#include <revolution/fs.h>

static void nandReadCloseCallback(ISFSError, void *);
static void nandCloseCallback(ISFSError, void *);
static void nandSafeCloseCallback(ISFSError, void *);
s32 nandSafeCloseAsync(NANDFileInfo *, NANDCallback, NANDCommandBlock *, BOOL);

void nandReadOpenCallback(ISFSError result, void* ctxt) {
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;

    if (result >= 0) {
        ((NANDFileInfo*)(b->fileInfo))->fileDescriptor = result;
        ((NANDFileInfo*)(b->fileInfo))->stage = 2;

        if (!(b->simpleFlag)) {
            ((NANDFileInfo*)(b->fileInfo))->mark = 3;
        }
        else {
            ((NANDFileInfo*)(b->fileInfo))->mark = 5;
        }

        ((NANDCallback)(b->callback))(NAND_RESULT_OK, b);
    }
    else {
        ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
    }
}

s32 NANDSafeCloseAsync(NANDFileInfo* info, NANDCallback cb, NANDCommandBlock* block) {
    return nandSafeCloseAsync(info, cb, block, FALSE);
}

s32 nandSafeCloseAsync(NANDFileInfo* info, NANDCallback cb, NANDCommandBlock* block, BOOL simple_flag) {
    ISFSError err = ISFS_ERROR_UNKNOWN;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (!(((info->mark == 3) && !simple_flag) || ((info->mark == 5) && simple_flag))) {
        return NAND_RESULT_INVALID;
    }

    block->simpleFlag = simple_flag;
    if (info->accType == 1) {
        block->fileInfo = info;
        block->callback = cb;
        err = ISFS_CloseAsync(info->fileDescriptor, nandReadCloseCallback, block);
    }
    else if ((info->accType == 2) || (info->accType == 3)) {
        block->fileInfo = info;
        block->callback = cb;
        block->state = 10;
        err = ISFS_CloseAsync(info->fileDescriptor, nandSafeCloseCallback, block);
    }
    else {
        err = ISFS_ERROR_INVALID;
    }

    return nandConvertErrorCode(err);
}

void nandSafeCloseCallback(ISFSError result, void* ctxt) {
    NANDCommandBlock* b= (NANDCommandBlock*)ctxt;

    if (result == 0) {
        NANDFileInfo* info = b->fileInfo;
        ISFSError ret = ISFS_ERROR_UNKNOWN;

        if (b->state == 12) {
            info->stage = 8;
        }

        if ((b->state == 12) && b->simpleFlag) {
            b->state += 2;
        }
        else {
            ++b->state;
        }

        if (b->state == 11) {
            info->stage = 6;
            ret = ISFS_CloseAsync(info->origFd, nandSafeCloseCallback, ctxt);
        }
        else if (b->state == 12) {
            info->stage = 7;
            ret = ISFS_RenameAsync((u8*)(info->tmpPath), (u8*)(info->origPath), nandSafeCloseCallback, ctxt);
        }
        else if (b->state == 13) {
            char tmpdir[64] = "";
            nandGetParentDirectory(tmpdir, info->tmpPath);
            ret = ISFS_DeleteAsync((u8*)tmpdir, nandSafeCloseCallback, ctxt);
        }
        else if (b->state == 14) {
            if (!(b->simpleFlag)) {
                info->stage = 9;
            }

            info->mark = 4;
            ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
            return;
        }

        if (ret != 0) {
            ((NANDCallback)(b->callback))(nandConvertErrorCode(ret), b);
        }
    }
}

static void nandReadCloseCallback(ISFSError result, void* ctxt) {
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;

    if (result == 0) {
        ((NANDFileInfo*)(b->fileInfo))->stage = 7;
        ((NANDFileInfo*)(b->fileInfo))->mark = 4;
    }
    
    ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
}

static void nandCloseCallback(ISFSError result, void* ctxt) {
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;

    if (result == 0) {
        ((NANDFileInfo*)(b->fileInfo))->stage = 7;
        ((NANDFileInfo*)(b->fileInfo))->mark = 2;
    }
    
    ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
}