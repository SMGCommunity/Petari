#include <revolution/nand.h>
#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

#include <mem.h>

#define NAND_RETRY_COUNT 3

#define INVALIDATE_ALIGN (1 << 31)

#define INSTANCE_IDGEN_INIT (1 << 31)

#define SLEEP_MSEC(x) OSSleepTicks(OS_MSEC_TO_TICKS((s64)(x)))

static u32 RdBufferMutex = 0;
static u32 WrBufferMutex = 0;
static u32 InstanceIdGen = 0;

static NWC24Err BufferedWrite(const void* pSrc, s32 size, NWC24File* pFile);
static NWC24Err BufferedWriteFlush(NWC24File* pFile);
static NWC24Err BufferedRead(void* pDst, s32 size, NWC24File* pFile);
static NWC24Err AlignedSeek(NWC24File* pFile, s32 offset, NWC24SeekMode whence);
static NWC24Err ConvertError(s32 nanderr, NWC24Err wc24err);
static NWC24Err ConvertVfError(s32 vferr, NWC24Err wc24err);

NWC24Err NWC24FOpen(NWC24File* pFile, const char* pPath, u32 mode) {
    InstanceIdGen++;
    InstanceIdGen |= INSTANCE_IDGEN_INIT;

    pFile->id = InstanceIdGen;
    pFile->align = 0;
    pFile->mode = mode;

    if (mode == NWC24_OPEN_NAND_WBUFF || mode == NWC24_OPEN_NAND_RBUFF || mode == NWC24_OPEN_VF_WBUFF || mode == NWC24_OPEN_VF_RBUFF) {
        if (!NWC24IsMsgLibOpened() && !NWC24IsMsgLibOpenedByTool()) {
            return NWC24_ERR_LIB_NOT_OPENED;
        }
    }

    if (mode == NWC24_OPEN_NAND_WBUFF || mode == NWC24_OPEN_VF_WBUFF) {
        if (WrBufferMutex != 0) {
            return NWC24_ERR_MUTEX;
        }

        WrBufferMutex = pFile->id;
    }

    if (mode & NWC24_OPEN_VF) {
        return NWC24iFOpenVF(pFile, pPath, mode);
    }

    return NWC24iFOpenNand(pFile, pPath, mode);
}

NWC24Err NWC24iFOpenNand(NWC24File* pFile, const char* pPath, u32 mode) {
    s32 result;
    NANDAccessType access;
    u32 i;

    switch (mode) {
    case NWC24_OPEN_NAND_W:
    case NWC24_OPEN_NAND_WBUFF: {
        result = NANDPrivateCreate(pPath, NAND_PERM_RWALL, 0);
        if (result != NAND_RESULT_OK && result != NAND_RESULT_EXISTS) {
            return NWC24_ERR_FILE_OTHER;
        }
        access = NAND_ACCESS_WRITE;
        break;
    }

    case NWC24_OPEN_NAND_R:
    case NWC24_OPEN_NAND_RBUFF: {
        access = NAND_ACCESS_READ;
        break;
    }

    case NWC24_OPEN_NAND_RW: {
        access = NAND_ACCESS_RW;
        break;
    }

    default: {
        return NWC24_ERR_INVALID_VALUE;
    }
    }

    for (i = 0; i < NAND_RETRY_COUNT; i++) {
        result = NANDPrivateOpen(pPath, &pFile->nandf, access);
        if ((u32)(result - NAND_RESULT_BUSY) > (u32)(NAND_RESULT_ALLOC_FAILED - NAND_RESULT_BUSY)) {
            break;
        }

        SLEEP_MSEC(1000);
    }

    if (result != NAND_RESULT_OK) {
        if (mode == NWC24_OPEN_NAND_WBUFF) {
            WrBufferMutex = 0;
        }

        if (result == NAND_RESULT_NOEXISTS) {
            return NWC24_ERR_FILE_NOEXISTS;
        }

        return NWC24_ERR_FILE_OPEN;
    }

    return NWC24_OK;
}

NWC24Err NWC24iFOpenVF(NWC24File* pFile, const char* pPath, u32 mode) {
    s32 result;
    const char* access;

    switch (mode) {
    case NWC24_OPEN_VF_W:
    case NWC24_OPEN_VF_WBUFF: {
        access = "w";
        break;
    }

    case NWC24_OPEN_VF_R:
    case NWC24_OPEN_VF_RBUFF: {
        access = "r";
        break;
    }

    default: {
        return NWC24_ERR_INVALID_VALUE;
    }
    }

    pFile->vff = VFOpenFile(pPath, access, 0);

    if (pFile->vff == NULL) {
        result = VFGetLastError();

        if (mode == NWC24_OPEN_VF_WBUFF) {
            WrBufferMutex = 0;
        }

        if (*access == 'r' && result == VF_ERROR_0002) {
            return NWC24_ERR_FILE_NOEXISTS;
        }

        return ConvertVfError(result, NWC24_ERR_FILE_OPEN);
    }

    return NWC24_OK;
}

NWC24Err NWC24FClose(NWC24File* pFile) {
    NWC24Err close = NWC24_OK;
    NWC24Err read = NWC24_OK;

    if (pFile->mode == NWC24_OPEN_NAND_WBUFF || pFile->mode == NWC24_OPEN_VF_WBUFF) {
        read = BufferedWriteFlush(pFile);
        WrBufferMutex = 0;
    }

    if (pFile->mode & NWC24_OPEN_VF) {
        close = NWC24iFCloseVF(pFile);
    } else {
        close = NWC24iFCloseNand(pFile);
    }

    return read != NWC24_OK ? read : close;
}

NWC24Err NWC24iFCloseNand(NWC24File* pFile) {
    s32 result;
    NWC24Err err;
    u32 i;

    err = NWC24_OK;

    for (i = 0; i < NAND_RETRY_COUNT; i++) {
        result = NANDClose(&pFile->nandf);
        if ((u32)(result - NAND_RESULT_BUSY) > (u32)(NAND_RESULT_ALLOC_FAILED - NAND_RESULT_BUSY)) {
            break;
        }

        SLEEP_MSEC(1000);
    }

    if (result == NAND_RESULT_CORRUPT) {
        err = NWC24_ERR_NAND_CORRUPT;
    } else if (result != NAND_RESULT_OK) {
        err = NWC24_ERR_FILE_CLOSE;
    }

    return err;
}

NWC24Err NWC24iFCloseVF(NWC24File* pFile) {
    s32 result;

    result = VFCloseFile(pFile->vff);
    if (result != VF_OK) {
        return ConvertVfError(result, NWC24_ERR_FILE_CLOSE);
    }

    return NWC24_OK;
}

NWC24Err NWC24FSeek(NWC24File* pFile, s32 offset, NWC24SeekMode whence) {
    s32 result;
    u32 i;

    if (pFile->mode == NWC24_OPEN_NAND_RBUFF || pFile->mode == NWC24_OPEN_VF_RBUFF) {
        return AlignedSeek(pFile, offset, whence);
    }

    if (pFile->mode & NWC24_OPEN_VF) {
        result = VFSeekFile(pFile->vff, offset, whence);
        if (result != VF_OK) {
            return ConvertVfError(result, NWC24_ERR_FILE_OTHER);
        }

        return NWC24_OK;
    }

    for (i = 0; i < NAND_RETRY_COUNT; i++) {
        result = NANDSeek(&pFile->nandf, offset, (s32)whence);
        if ((u32)(result - NAND_RESULT_BUSY) > (u32)(NAND_RESULT_ALLOC_FAILED - NAND_RESULT_BUSY)) {
            break;
        }

        SLEEP_MSEC(1000);
    }

    if (result < 0) {
        return NWC24_ERR_FILE_OTHER;
    }

    return NWC24_OK;
}

NWC24Err NWC24FRead(void* pDst, s32 size, NWC24File* pFile) {
    s32 result;
    u32 i;

    switch (pFile->mode) {
    case NWC24_OPEN_NAND_W:
    case NWC24_OPEN_NAND_WBUFF:
    case NWC24_OPEN_VF_W: {
        return NWC24_ERR_PROTECTED;
    }

    case NWC24_OPEN_NAND_RBUFF:
    case NWC24_OPEN_VF_RBUFF: {
        return BufferedRead(pDst, size, pFile);
    }
    }

    if (pFile->mode & NWC24_OPEN_VF) {
        result = VFReadFile(pFile->vff, pDst, size, NULL);
        if (result != VF_OK) {
            return ConvertVfError(result, NWC24_ERR_FILE_READ);
        }

        return NWC24_OK;
    }

    for (i = 0; i < NAND_RETRY_COUNT; i++) {
        result = NANDRead(&pFile->nandf, pDst, size);
        if ((u32)(result - NAND_RESULT_BUSY) > (u32)(NAND_RESULT_ALLOC_FAILED - NAND_RESULT_BUSY)) {
            break;
        }

        SLEEP_MSEC(1000);
    }

    if (result == NAND_RESULT_CORRUPT) {
        return NWC24_ERR_NAND_CORRUPT;
    }

    if (result < 0) {
        return ConvertError(result, NWC24_ERR_FILE_READ);
    }

    return NWC24_OK;
}

NWC24Err NWC24FWrite(const void* pSrc, s32 size, NWC24File* pFile) {
    s32 result;
    u32 i;

    switch (pFile->mode) {
    case NWC24_OPEN_NAND_WBUFF:
    case NWC24_OPEN_VF_WBUFF: {
        return BufferedWrite(pSrc, size, pFile);
    }

    case NWC24_OPEN_NAND_R:
    case NWC24_OPEN_NAND_RBUFF:
    case NWC24_OPEN_VF_R: {
        return NWC24_ERR_PROTECTED;
    }
    }

    if (pFile->mode & NWC24_OPEN_VF) {
        result = VFWriteFile(pFile->vff, (void*)pSrc, size);
        if (result != VF_OK) {
            return ConvertVfError(result, NWC24_ERR_FILE_WRITE);
        }

        return NWC24_OK;
    }

    for (i = 0; i < NAND_RETRY_COUNT; i++) {
        result = NANDWrite(&pFile->nandf, pSrc, size);
        if ((u32)(result - NAND_RESULT_BUSY) > (u32)(NAND_RESULT_ALLOC_FAILED - NAND_RESULT_BUSY)) {
            break;
        }

        SLEEP_MSEC(1000);
    }

    if (result == NAND_RESULT_CORRUPT) {
        return NWC24_ERR_NAND_CORRUPT;
    }

    if (result < 0) {
        return ConvertError(result, NWC24_ERR_FILE_WRITE);
    }

    if (result != size) {
        return NWC24_ERR_FILE_WRITE;
    }

    return NWC24_OK;
}

NWC24Err NWC24FGetLength(NWC24File* pFile, u32* pLength) {
    s32 error;

    if (pFile->mode & NWC24_OPEN_VF) {
        error = VFGetFileSizeByFd(pFile->vff);
        if (error < 0) {
            return ConvertVfError(error, NWC24_ERR_FILE_OTHER);
        }

        *pLength = error;
        return NWC24_OK;
    }

    error = NANDGetLength(&pFile->nandf, pLength);
    if (error != NAND_RESULT_OK) {
        return NWC24_ERR_FILE_OTHER;
    }

    return NWC24_OK;
}

NWC24Err NWC24FDeleteVF(const char* pPath) {
    s32 result;

    result = VFDeleteFile(pPath);
    if (result != VF_OK) {
        return ConvertVfError(result, NWC24_ERR_FILE_OTHER);
    }

    return NWC24_OK;
}

NWC24Err NWC24MountVF(const char* pDrive, const char* pFileName) {
    s32 result;

    result = VFMountDriveNANDFlashEx(pDrive, pFileName);
    if (result == VF_ERROR_B001) {
        return NWC24_ERR_FILE_NOEXISTS;
    }

    if (result != VF_OK) {
        return ConvertVfError(result, NWC24_ERR_INTERNAL_VF);
    }

    result = VFSetSyncMode(pDrive, 1);
    if (result != VF_OK) {
        return ConvertVfError(result, NWC24_ERR_INTERNAL_VF);
    }

    return NWC24_OK;
}

NWC24Err NWC24UnmountVF(const char* pDrive) {
    s32 result;

    result = VFUnmountDrive(pDrive);
    if (result != VF_OK) {
        return ConvertVfError(result, NWC24_ERR_INTERNAL_VF);
    }

    return NWC24_OK;
}

NWC24Err NWC24CheckSizeVF(const char* pDrive, u32* pSize) {
    s32 size;

    size = VFGetDriveFreeSize(pDrive);
    *pSize = size;

    if (size < 0) {
        return ConvertVfError(VFGetLastError(), NWC24_ERR_INTERNAL_VF);
    }

    return NWC24_OK;
}

static NWC24Err BufferedWrite(const void* pSrc, s32 size, NWC24File* pFile) {
    u32 now;
    u32 left;
    u32 pos;
    u8* pBuf;
    const u8* pByteSrc;
    s32 result;
    NWC24Err err;
    u32 i;

    left = size;
    pos = pFile->align;
    pBuf = NWC24WorkP->writeBuffer;
    pByteSrc = (u8*)pSrc;
    err = NWC24_OK;

    while (left != 0) {
        now = (s32)sizeof(NWC24WorkP->writeBuffer) - pos;

        if (left < now) {
            now = left;
        }

        memcpy(pBuf + pos, pByteSrc, now);

        pos += now;
        left -= now;
        pByteSrc += now;

        if (pos < (s32)sizeof(NWC24WorkP->writeBuffer)) {
            continue;
        }

        pos = 0;

        if (pFile->mode & NWC24_OPEN_VF) {
            result = VFWriteFile(pFile->vff, pBuf, (s32)sizeof(NWC24WorkP->writeBuffer));
            if (result == VF_OK) {
                continue;
            }

            err = ConvertVfError(result, NWC24_ERR_FILE_WRITE);
            break;
        }

        for (i = 0; i < NAND_RETRY_COUNT; i++) {
            result = NANDWrite(&pFile->nandf, pBuf, (s32)sizeof(NWC24WorkP->writeBuffer));
            if ((u32)(result - NAND_RESULT_BUSY) > (u32)(NAND_RESULT_ALLOC_FAILED - NAND_RESULT_BUSY)) {
                break;
            }

            SLEEP_MSEC(1000);
        }

        if (result == NAND_RESULT_CORRUPT) {
            return NWC24_ERR_NAND_CORRUPT;
        }

        if (result != (s32)sizeof(NWC24WorkP->writeBuffer)) {
            err = NWC24_ERR_FILE_WRITE;
            break;
        }
    }

    pFile->align = pos;
    return err;
}

static NWC24Err BufferedWriteFlush(NWC24File* pFile) {
    s32 result;
    u32 pos;
    u8* pBuf;
    NWC24Err err;
    u32 i;

    pos = pFile->align;
    pBuf = NWC24WorkP->writeBuffer;
    err = NWC24_OK;

    if (pos == 0) {
        return err;
    }

    while (pos % 32 != 0) {
        pBuf[pos++] = 0x00;
    }

    if (pFile->mode & NWC24_OPEN_VF) {
        result = VFWriteFile(pFile->vff, pBuf, pos);
        if (result != VF_OK) {
            err = ConvertVfError(result, NWC24_ERR_FILE_WRITE);
        }

        return err;
    }

    for (i = 0; i < NAND_RETRY_COUNT; i++) {
        result = NANDWrite(&pFile->nandf, pBuf, pos);
        if ((u32)(result - NAND_RESULT_BUSY) > (u32)(NAND_RESULT_ALLOC_FAILED - NAND_RESULT_BUSY)) {
            break;
        }

        SLEEP_MSEC(1000);
    }

    if (result == NAND_RESULT_CORRUPT) {
        return NWC24_ERR_NAND_CORRUPT;
    }

    if (result != pos) {
        err = NWC24_ERR_FILE_WRITE;
    }

    return err;
}

static NWC24Err BufferedRead(void* pDst, s32 size, NWC24File* pFile) {
    u32 total;
    u32 now;
    u32 left;
    s32 result;
    u32 pos;
    u8* pBuf;
    u8* pByteDst;
    NWC24Err err;
    u32 i;
    u32 bytesread;

    pByteDst = (u8*)pDst;
    total = size;
    pBuf = NWC24WorkP->readBuffer;
    err = NWC24_OK;

    if (RdBufferMutex != 0) {
        return NWC24_ERR_MUTEX;
    }

    RdBufferMutex = pFile->id;

    if (pFile->align & INVALIDATE_ALIGN) {
        return NWC24_ERR_INVALID_OPERATION;
    }

    pos = pFile->align;

    while (total > 0) {
        now = NWC24i_IO_BUFFER_SIZE;
        left = now - pos;

        if (total < left) {
            left = total;
            now = ((total + pos + 31) & ~31);
        }

        if (pFile->mode & NWC24_OPEN_VF) {
            result = VFReadFile(pFile->vff, pBuf, now, &bytesread);
            if (result != VF_OK) {
                err = ConvertVfError(result, NWC24_ERR_FILE_READ);
                break;
            }

            result = bytesread;
        } else {
            for (i = 0; i < NAND_RETRY_COUNT; i++) {
                result = NANDRead(&pFile->nandf, pBuf, now);
                if ((u32)(result - NAND_RESULT_BUSY) > (u32)(NAND_RESULT_ALLOC_FAILED - NAND_RESULT_BUSY)) {
                    break;
                }

                SLEEP_MSEC(1000);
            }

            if (result == NAND_RESULT_CORRUPT) {
                return NWC24_ERR_NAND_CORRUPT;
            }
        }

        if (result != now) {
            err = NWC24_ERR_FILE_READ;
            break;
        } else {
            memcpy(pByteDst, pBuf + pos, left);
            pByteDst += left;
            total -= left;
            pos = 0;
        }
    }

    pFile->align = (pFile->align + size) % 32;
    pFile->align |= INVALIDATE_ALIGN;
    RdBufferMutex = 0;
    return err;
}

static NWC24Err AlignedSeek(NWC24File* pFile, s32 offset, NWC24SeekMode whence) {
    s32 result;
    s32 alignofs;

    pFile->align = offset % 32;
    alignofs = offset - pFile->align;

    if (pFile->mode & NWC24_OPEN_VF) {
        result = VFSeekFile(pFile->vff, alignofs, whence);
        if (result != VF_OK) {
            return ConvertVfError(result, NWC24_ERR_FILE_OTHER);
        }

        return NWC24_OK;
    }

    result = NANDSeek(&pFile->nandf, alignofs, (s32)whence);
    if (result < 0) {
        return NWC24_ERR_FILE_OTHER;
    }

    return NWC24_OK;
}

static NWC24Err ConvertError(s32 nanderr, NWC24Err wc24err) {
    switch (nanderr) {
    case NAND_RESULT_ECC_CRIT:
    case NAND_RESULT_AUTHENTICATION: {
        return NWC24_ERR_FILE_BROKEN;
    }

    case NAND_RESULT_CORRUPT: {
        return NWC24_ERR_NAND_CORRUPT;
    }

    default: {
        return wc24err;
    }
    }
}

static NWC24Err ConvertVfError(s32 vferr, NWC24Err wc24err) {
    if (vferr == VF_ERROR_0005) {
        return ConvertError(VFGetLastDeviceError(NWC24i_VF_DRIVE), wc24err);
    }

    return wc24err;
}
