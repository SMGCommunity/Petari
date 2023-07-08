#include <revolution/nand.h>
#include <revolution/fs.h>
#include <cstdio>

static void  nandOpenCallback(ISFSError, void *);
static void nandReadCloseCallback(ISFSError, void *);
static void nandCloseCallback(ISFSError, void *);
static void nandSafeCloseCallback(ISFSError, void *);
static void nandSafeOpenCallback(const ISFSError, void *);
void nandReadOpenCallback(ISFSError, void *);
s32 nandSafeCloseAsync(NANDFileInfo *, NANDCallback, NANDCommandBlock *, BOOL);
s32 nandSafeOpenAsync(const char *, NANDFileInfo *, const u8, void *, const u32, NANDCallback, NANDCommandBlock *, BOOL, BOOL);

static u32 nandGetUniqueNumber(void) {
    static u32 s_counter = 0;
    u32 ret;
    BOOL enabled = OSDisableInterrupts();
    ret = s_counter++;
    OSRestoreInterrupts(enabled);
    return ret;
}

IOSFd nandOpen(const char* path, const u8 accType, NANDCommandBlock* block, const BOOL async_flag, const BOOL privilege_flag) {
    IOSFd fd = -117;
    char absPath[64] = "";
    u32 access = 0;

    nandGenerateAbsPath(absPath, path);

    if (!privilege_flag && nandIsPrivatePath(absPath)) {
        return ISFS_ERROR_ACCESS;
    }
    else {
        switch (accType) {
            case 3:
                access = 3;
                break;
            case 1:
                access = 1;
                break;
            case 2:
                access = 2;
                break;
            default:
                break;
        }

        if (async_flag) {
            fd = ISFS_OpenAsync((const u8*)absPath, access, nandOpenCallback, block);
        }
        else {
            fd = ISFS_Open((const u8*)absPath, access);
        }

        return fd;
    }
}

s32 NANDOpen(const char* path, NANDFileInfo* info, const u8 accType) {
    IOSFd fd = ISFS_ERROR_UNKNOWN;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    fd = nandOpen(path, accType, NULL, FALSE, FALSE);

    if (fd >= 0) {
        info->fileDescriptor = fd;
        info->mark = 1;
        return NAND_RESULT_OK;
    }
    else {
        return nandConvertErrorCode(fd);
    }
}

s32 NANDPrivateOpen(const char* path, NANDFileInfo* info, const u8 accType) {
    IOSFd fd = ISFS_ERROR_UNKNOWN;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    fd = nandOpen(path, accType, NULL, FALSE, TRUE);

    if (fd >= 0) {
        info->fileDescriptor = fd;
        info->mark = 1;
        return NAND_RESULT_OK;
    }
    else {
        return nandConvertErrorCode(fd);
    }
}

s32 NANDOpenAsync(const char* path, NANDFileInfo* info, const u8 accType, NANDCallback cb, NANDCommandBlock* block) {
    IOSFd fd = ISFS_ERROR_UNKNOWN;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = cb;
    block->fileInfo = info;
    fd = nandOpen(path, accType, block, TRUE, FALSE);
    return nandConvertErrorCode(fd);
}

s32 NANDPrivateOpenAsync(const char* path, NANDFileInfo* info, const u8 accType, NANDCallback cb, NANDCommandBlock* block) {
    IOSFd fd = ISFS_ERROR_UNKNOWN;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    block->callback = cb;
    block->fileInfo = info;
    fd = nandOpen(path, accType, block, TRUE, TRUE);
    return nandConvertErrorCode(fd);
}

void nandOpenCallback(ISFSError result, void* ctxt) {
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;

    if (result >= 0) {
        ((NANDFileInfo*)(b->fileInfo))->fileDescriptor = result;
        ((NANDFileInfo*)(b->fileInfo))->stage = 2;
        ((NANDFileInfo*)(b->fileInfo))->mark = 1;
        ((NANDCallback)(b->callback))(NAND_RESULT_OK, b);
    }
    else {
        ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
    }
}

s32 NANDClose(NANDFileInfo* info) {
    ISFSError err = ISFS_ERROR_UNKNOWN;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (info->mark != 1) {
        return NAND_RESULT_INVALID;
    }

    err = ISFS_Close(info->fileDescriptor);

    if (err == ISFS_ERROR_OK) {
        info->mark = 2;
    }

    return nandConvertErrorCode(err);
}

s32 NANDCloseAsync(NANDFileInfo* info, NANDCallback cb, NANDCommandBlock* block) {
    ISFSError err = ISFS_ERROR_UNKNOWN;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (info->mark != 1) {
        return NAND_RESULT_INVALID;
    }

    block->callback = cb;
    block->fileInfo = info;
    err = ISFS_CloseAsync(info->fileDescriptor, nandCloseCallback, block);
    return nandConvertErrorCode(err);
}

s32 NANDPrivateSafeOpenAsync(const char* path, NANDFileInfo* info, const u8 accType, void* buf, const u32 length, NANDCallback cb, NANDCommandBlock* block) {
    return nandSafeOpenAsync(path, info, accType, buf, length, cb, block, TRUE, FALSE);
}

s32 nandSafeOpenAsync(const char* path, NANDFileInfo* info, const u8 accType, void* buf, const u32 length, NANDCallback cb, NANDCommandBlock* block, BOOL privilege_flag, BOOL simple_flag) {
    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    if (simple_flag && ((length & 0x3FFF) != 0)) {
        return NAND_RESULT_INVALID;
    }
    
    info->accType = accType;
    info->stage = 0;
    block->simpleFlag = simple_flag;
    nandGenerateAbsPath(info->origPath, path);

    if (!privilege_flag && nandIsPrivatePath(info->origPath)) {
        return NAND_RESULT_ACCESS;
    }

    if (accType == 1) {
        IOSFd fd = -1;
        block->fileInfo = info;
        block->callback = cb;
        fd = ISFS_OpenAsync((u8*)(info->origPath), 1, nandReadOpenCallback, block);

        if (fd == ISFS_ERROR_OK) {
            return NAND_RESULT_OK;
        }
        else {
            return nandConvertErrorCode(fd);
        }
    }
    else if (accType == 2 || accType == 3) {
        ISFSError ret = ISFS_ERROR_UNKNOWN;
        block->fileInfo = info;
        block->callback = cb;
        block->state = 0;
        block->copyBuf = buf;
        block->bufLength = length;
        ret = ISFS_CreateDirAsync((u8*)"/tmp/sys", 0, 3, 3, 3, nandSafeOpenCallback, block);
    
        if (ret == ISFS_ERROR_OK) {
            return NAND_RESULT_OK;
        }
        else {
            return nandConvertErrorCode(ret);
        }
    }
    else {
        return NAND_RESULT_INVALID;
    }
}

void nandSafeOpenCallback(const ISFSError result, void* ctxt) {
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;

    if ((result >= 0) || ((result == ISFS_ERROR_EXISTS) && b->state == 0)) {
        NANDFileInfo* info = b->fileInfo;
        ISFSError ret = ISFS_ERROR_UNKNOWN;

        if (b->state == 0) {
            info->stage = 1;
        }

        if (b->state == 2) {
            info->origFd = result;
            info->stage = 2;
        }

        if ((b->state == 2) && b->simpleFlag) {
            b->state += 2;
        }
        else {
            ++b->state;
        }

        if (b->state == 1) {
            ret = ISFS_GetAttrAsync((u8*)(info->origPath), &b->ownerId, &b->groupId, &b->attr, &b->ownerAcc, &b->groupAcc, &b->othersAcc, nandSafeOpenCallback, ctxt);
        }
        else if (b->state == 2) {
            ret = ISFS_OpenAsync((u8*)(info->origPath), 1, nandSafeOpenCallback, ctxt);
        }
        else if (b->state == 3) {
            char tmpDir[64];
            b->uniqNo = nandGetUniqueNumber();
            sprintf(tmpDir, "%s/%08x", "/tmp/sys", b->uniqNo);
            ret = ISFS_CreateDirAsync((u8*)tmpDir, 0, 3, 0, 0, nandSafeOpenCallback, ctxt);
        }
        else if (b->state == 4) {
            char filename[13];
            nandGetRelativeName(filename, info->origPath);

            if (!b->simpleFlag) {
                info->stage = 3;
                sprintf(info->tmpPath, "%s/%08x/%s", "/tmp/sys", b->uniqNo, filename);
            }
            else {
                sprintf(info->tmpPath, "%s/%s", "/tmp/sys", filename);
            }

            ret = ISFS_CreateFileAsync((u8*)info->tmpPath, b->attr, b->ownerAcc, b->groupAcc, b->othersAcc, nandSafeOpenCallback, ctxt);
        }
        else if (b->state == 5) {
            info->stage = 4;

            if (info->accType == 2) {
                ret = ISFS_OpenAsync((u8*)info->tmpPath, 2, nandSafeOpenCallback, ctxt);
            }
            else if (info->accType == 3) {
                ret = ISFS_OpenAsync((u8*)info->tmpPath, 3, nandSafeOpenCallback, ctxt);
            }
            else {
                ret = ISFS_ERROR_UNKNOWN;
            }
        }
        else if (b->state == 6) {
            info->fileDescriptor = result;
            info->stage = 5;
            b->state = 7;
            ret = ISFS_ReadAsync(info->origFd, b->copyBuf, b->bufLength, nandSafeOpenCallback, ctxt);
        }
        else if (b->state == 7) {
            ret = ISFS_ReadAsync(info->origFd, b->copyBuf, b->bufLength, nandSafeOpenCallback, ctxt);
        }
        else if (b->state == 8) {
            if (result > 0) {
                b->state = 6;
                ret = ISFS_WriteAsync(info->fileDescriptor, b->copyBuf, (u32)result, nandSafeOpenCallback, ctxt);
            }
            else if (result == 0) {
                ret = ISFS_SeekAsync(info->fileDescriptor, 0, 0, nandSafeOpenCallback, ctxt);
            }
        }
        else if (b->state == 9) {
            if (result == 0) {
                if (!b->simpleFlag) {
                    info->mark = 3;
                }
                else {
                    info->mark = 5;
                }

                ((NANDCallback)(b->callback))(nandConvertErrorCode(ISFS_ERROR_OK), b);
            }
            else {
                ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
            }

            return;
        }

        if (ret != ISFS_ERROR_OK) {
            ((NANDCallback)(b->callback))(nandConvertErrorCode(ret), b);
        }
    }
    else {
        ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
    }
}

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
    NANDCommandBlock* b = (NANDCommandBlock*)ctxt;

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
    else {
        ((NANDCallback)(b->callback))(nandConvertErrorCode(result), b);
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