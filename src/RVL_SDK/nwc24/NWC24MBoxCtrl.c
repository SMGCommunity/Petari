#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

#include <mem.h>

#define MBOX_CTRL_MAGIC FOURCC('W', 'c', 'T', 'f')
#define MBOX_CTRL_VERSION 4

#define MSG_ID_MIN 1
#define MSG_ID_MAX 1000000

#define SEND_MAIL_MBOX "/wc24send.mbx"
#define RECV_MAIL_MBOX "/wc24recv.mbx"

#define SEND_MAIL_CTRL "/wc24send.ctl"
#define RECV_MAIL_CTRL "/wc24recv.ctl"

#define RECV_MAIL_FMT NWC24i_VF_DRIVE ":/mb/r%07d.msg"
#define SEND_MAIL_FMT NWC24i_VF_DRIVE ":/mb/s%07d.msg"

typedef struct NWC24iMountInfo {
    s32 count;
    NWC24MsgBoxId id;
} NWC24iMountInfo;

static NWC24iMountInfo MountInfo;

static NWC24Err GetMBoxFilePath(NWC24MsgBoxId id, char* pDst, s32 maxlen);
static NWC24Err MakeMailPath(NWC24MsgBoxId id, u32 msgId, char* pDst, s32 maxlen);
static NWC24Err IsFileThere(const char* pPath);

static NWC24Err DeleteMsg(NWC24MsgBoxId id, u32 msgId, BOOL checkPermission);
static NWC24Err DeleteMsgFile(NWC24MsgBoxId id, u32 msgId);
static NWC24Err DuplicationCheck(NWC24iMBCHeader* pHeader, const NWC24iMsgObj* pMsg, NWC24File* pFile, NWC24MsgBoxId id);

static NWC24Err GetCachedMBCHeader(NWC24MsgBoxId id, NWC24iMBCHeader** ppHeader);
static NWC24Err LoadMBCHeader(NWC24iMBCHeader* pHeader, NWC24File* pFile);
static NWC24Err SaveMBCHeader(NWC24iMBCHeader* pHeader, NWC24File* pFile);
static NWC24Err AddMBCEntry(NWC24iMBCHeader* pHeader, const NWC24iMsgObj* pMsg, NWC24File* pFile);
static NWC24Err ClearMBCEntry(NWC24iMBCHeader* pHeader, NWC24File* pFile, u32 offset);

static NWC24Err GetNewMsgId(NWC24iMBCHeader* pHeader, u32* pMsgId);
static int CompareMsgId(u32 lhs, u32 rhs);

static NWC24Err MountVFMBox(NWC24MsgBoxId id) NO_INLINE;
static NWC24Err UnmountVFMBox(void);
static NWC24Err UnmountVFMBoxForced(void);

static u32 PackNWC24Data(const NWC24Data* pData);
static NWC24Err CopyMsgObjToMBCFmt(const NWC24iMsgObj* pSrc, NWC24iMBCEntry* pDst);

NWC24Err NWC24iMBoxGetCtrlFilePath(NWC24MsgBoxId id, char* pDst, s32 maxlen) {
    const char* pMBoxDir = NWC24GetMBoxDir();

    if (STD_strnlen(pMBoxDir, NWC24i_MBOX_DIR_LENGTH) + NWC24i_KSTRLEN("/wc24****.ctl") > maxlen) {
        return NWC24_ERR_NOMEM;
    }

    switch (id) {
    case NWC24_MSGBOX_SEND: {
        Mail_sprintf(pDst, "%s%s", pMBoxDir, SEND_MAIL_CTRL);
        break;
    }

    case NWC24_MSGBOX_RECV: {
        Mail_sprintf(pDst, "%s%s", pMBoxDir, RECV_MAIL_CTRL);
        break;
    }

    default: {
        return NWC24_ERR_INVALID_VALUE;
    }
    }

    return NWC24_OK;
}

NWC24Err NWC24iOpenMBox(void) {
    NWC24Err result;
    NWC24iMBCHeader* pHeader;
    char* pPath;
    const char* pMBoxDir;

    pPath = NWC24WorkP->pathWork;

    Mail_memset(&NWC24WorkP->sendCtrl, 0, sizeof(NWC24iMBCHeader));
    result = GetCachedMBCHeader(NWC24_MSGBOX_SEND, &pHeader);
    if (result != NWC24_OK) {
        return result;
    }

    Mail_memset(&NWC24WorkP->recvCtrl, 0, sizeof(NWC24iMBCHeader));
    result = GetCachedMBCHeader(NWC24_MSGBOX_RECV, &pHeader);
    if (result != NWC24_OK) {
        return result;
    }

    result = GetMBoxFilePath(NWC24_MSGBOX_RECV, pPath, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }

    result = IsFileThere(pPath);
    if (result != NWC24_OK) {
        return result;
    }

    result = GetMBoxFilePath(NWC24_MSGBOX_SEND, pPath, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }

    result = IsFileThere(pPath);
    if (result != NWC24_OK) {
        return result;
    }

    MountInfo.count = 0;
    MountInfo.id = NWC24_MSGBOX_SEND;

    return NWC24_OK;
}

static NWC24Err CreateCtrlFile(NWC24MsgBoxId id, BOOL forceCreate);

NWC24Err NWC24iInitMBox(void) {
    NWC24Err result;
    char* path = NWC24WorkP->pathWork;
    int renewSend, renewRecv;
    Mail_memset(&NWC24WorkP->sendCtrl, 0, sizeof(NWC24iMBCHeader));
    renewSend = CreateCtrlFile(NWC24_MSGBOX_SEND, FALSE);
    if (renewSend < 0) {
        return renewSend;
    }
    Mail_memset(&NWC24WorkP->recvCtrl, 0, sizeof(NWC24iMBCHeader));
    renewRecv = CreateCtrlFile(NWC24_MSGBOX_RECV, FALSE);
    if (renewRecv < 0) {
        return renewRecv;
    }
    result = GetMBoxFilePath(NWC24_MSGBOX_RECV, path, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }
    result = IsFileThere(path);
    if (result == NWC24_ERR_FILE_NOEXISTS || renewRecv) {
        result = NWC24CreateVF(path, 0x700000);
    }
    if (result != NWC24_OK) {
        return result;
    }
    result = GetMBoxFilePath(NWC24_MSGBOX_SEND, path, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }
    result = IsFileThere(path);
    if (result == NWC24_ERR_FILE_NOEXISTS || renewSend) {
        result = NWC24CreateVF(path, 0x200000);
    }
    if (result != NWC24_OK) {
        return result;
    }
    MountInfo.count = 0;
    MountInfo.id = NWC24_MSGBOX_SEND;
    NWC24FDelete("/shared2/wc24/mbox/recvtmp.msg");
    NWC24FDelete("/shared2/wc24/mbox/dlcnt.bin");
    return result;
}

NWC24Err NWC24iMBoxOpenNewMsg(NWC24MsgBoxId id, NWC24File* pFile, u32* pMsgId) {
    NWC24Err result;
    NWC24iMBCHeader* pHeader;
    char* pPath;

    result = GetCachedMBCHeader(id, &pHeader);
    if (result != NWC24_OK) {
        return result;
    }

    result = GetNewMsgId(pHeader, pMsgId);
    if (result != NWC24_OK) {
        return result;
    }

    result = MountVFMBox(id);
    if (result != NWC24_OK) {
        return result;
    }

    pPath = NWC24WorkP->pathWork;

    result = MakeMailPath(id, *pMsgId, pPath, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24FOpen(pFile, pPath, NWC24_OPEN_VF_WBUFF);
    return result;
}

NWC24Err NWC24iMBoxCloseMsg(NWC24File* pFile) {
    NWC24Err closeResult;
    NWC24Err unmountResult;

    closeResult = NWC24FClose(pFile);
    unmountResult = UnmountVFMBox();

    if (closeResult != NWC24_OK && unmountResult != NWC24_OK) {
        return unmountResult;
    }

    return closeResult;
}

NWC24Err NWC24iMBoxCancelMsg(NWC24File* pFile, NWC24MsgBoxId id, u32 msgId) {
    NWC24Err closeResult;
    NWC24Err deleteResult;
    NWC24Err unmountResult;

    closeResult = NWC24FClose(pFile);
    deleteResult = DeleteMsgFile(id, msgId);
    unmountResult = UnmountVFMBoxForced();

    if (closeResult != NWC24_OK) {
        return closeResult;
    }

    if (deleteResult != NWC24_OK) {
        return deleteResult;
    }

    if (unmountResult != NWC24_OK) {
        return unmountResult;
    }

    return NWC24_OK;
}

NWC24Err NWC24iMBoxAddMsgObj(NWC24MsgBoxId id, NWC24iMsgObj* pMsg) {
    NWC24Err result;
    NWC24Err closeResult;
    NWC24iMBCHeader* pHeader;
    char* pPath;
    NWC24File file;

    result = GetCachedMBCHeader(id, &pHeader);
    if (result != NWC24_OK) {
        return result;
    }

    if (pHeader->numMsgs == pHeader->capacity) {
        return NWC24_ERR_FULL;
    }

    pPath = NWC24WorkP->pathWork;
    result = NWC24iMBoxGetCtrlFilePath(id, pPath, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24FOpen(&file, pPath, NWC24_OPEN_NAND_RW);
    if (result != NWC24_OK) {
        return result;
    }

    result = AddMBCEntry(pHeader, pMsg, &file);
    if (result == NWC24_OK) {
        result = DuplicationCheck(pHeader, pMsg, &file, id);
        if (result != NWC24_OK) {
            goto _exit;
        }
    } else if (result == NWC24_ERR_BROKEN) {
        pHeader->magic = 0x42524B4E;
    } else {
        goto _exit;
    }

    result = SaveMBCHeader(pHeader, &file);

_exit:
    closeResult = NWC24FClose(&file);
    return result != NWC24_OK ? result : closeResult;
}

NWC24Err NWC24iMBoxFlushHeader(NWC24MsgBoxId id) {
    NWC24Err result;
    NWC24Err closeResult;
    NWC24iMBCHeader* pHeader;
    char* pPath;
    NWC24File file;

    result = GetCachedMBCHeader(id, &pHeader);
    if (result != NWC24_OK) {
        return result;
    }

    pPath = NWC24WorkP->pathWork;
    result = NWC24iMBoxGetCtrlFilePath(id, pPath, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24FOpen(&file, pPath, NWC24_OPEN_NAND_RW);
    if (result != NWC24_OK) {
        return result;
    }

    result = SaveMBCHeader(pHeader, &file);

    closeResult = NWC24FClose(&file);
    return result != NWC24_OK ? result : closeResult;
}

NWC24Err NWC24iMBoxCheck(NWC24MsgBoxId id, u32 size) {
    NWC24Err result;
    NWC24iMBCHeader* pHeader;
    volatile u32 oldestMsgId;
    u32 freeSpaceNeed;

    if (size >= NWC24i_MBOX_MAX_SIZE) {
        return NWC24_ERR_OVERFLOW;
    }

    freeSpaceNeed = size + 0x4000;

    result = GetCachedMBCHeader(id, &pHeader);
    if (result != NWC24_OK) {
        return result;
    }

    if (id == NWC24_MSGBOX_SEND) {
        if (pHeader->numMsgs >= pHeader->capacity) {
            return NWC24_ERR_FULL;
        }

        if (pHeader->freeSpace <= freeSpaceNeed) {
            return NWC24_ERR_FULL;
        }

    } else if (id == NWC24_MSGBOX_RECV) {
        while (pHeader->numMsgs >= pHeader->capacity || pHeader->freeSpace <= freeSpaceNeed) {
            oldestMsgId = pHeader->oldestMsgId;

            result = DeleteMsg(id, oldestMsgId, FALSE);
            if (result != NWC24_OK) {
                return result;
            }

            result = GetCachedMBCHeader(id, &pHeader);
            if (result != NWC24_OK) {
                return result;
            }
        }

    } else {
        return NWC24_ERR_INVALID_VALUE;
    }

    return NWC24_OK;
}

static NWC24Err GetMBoxFilePath(NWC24MsgBoxId id, char* pDst, s32 maxlen) {
    const char* pMBoxDir = NWC24GetMBoxDir();

    if (STD_strnlen(pMBoxDir, NWC24i_MBOX_DIR_LENGTH) + NWC24i_KSTRLEN("/wc24****.mbx") > maxlen) {
        return NWC24_ERR_NOMEM;
    }

    switch (id) {
    case NWC24_MSGBOX_SEND: {
        Mail_sprintf(pDst, "%s%s", pMBoxDir, SEND_MAIL_MBOX);
        break;
    }

    case NWC24_MSGBOX_RECV: {
        Mail_sprintf(pDst, "%s%s", pMBoxDir, RECV_MAIL_MBOX);
        break;
    }

    default: {
        return NWC24_ERR_INVALID_VALUE;
    }
    }

    return NWC24_OK;
}

static NWC24Err MakeMailPath(NWC24MsgBoxId id, u32 msgId, char* pDst, s32 maxlen) {
    if (maxlen < NWC24i_KSTRLEN("*******.msg")) {
        return NWC24_ERR_NOMEM;
    }

    switch (id) {
    case NWC24_MSGBOX_SEND: {
        Mail_sprintf(pDst, SEND_MAIL_FMT, msgId);
        break;
    }

    case NWC24_MSGBOX_RECV: {
        Mail_sprintf(pDst, RECV_MAIL_FMT, msgId);
        break;
    }

    default: {
        return NWC24_ERR_INVALID_VALUE;
    }
    }

    return NWC24_OK;
}

static NWC24Err IsFileThere(const char* pPath) {
    NWC24File file;
    NWC24Err result;

    result = NWC24FOpen(&file, pPath, NWC24_SEEK_END);
    if (result == NWC24_OK) {
        result = NWC24FClose(&file);
    }

    return result;
}

static NWC24Err DeleteMsg(NWC24MsgBoxId id, u32 msgId, BOOL checkPermission) {
    NWC24Err result;
    NWC24Err fileResult;
    NWC24iMBCHeader* pHeader;
    char* pathWork;
    u32 lastId;
    u32 offset;
    u32 clearOffset;
    u32 oldestMsgId;
    NWC24iMBCEntry* pEntry;
    NWC24File file;

    lastId = 0;
    offset = 0;
    clearOffset = 0;
    oldestMsgId = 0;

    if (!NWC24IsMsgLibOpened() && !NWC24IsMsgLibOpenedByTool()) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }

    pEntry = &NWC24WorkP->mbcEntry;

    result = GetCachedMBCHeader(id, &pHeader);
    if (result != NWC24_OK) {
        return result;
    }

    pathWork = NWC24WorkP->pathWork;

    result = NWC24iMBoxGetCtrlFilePath(id, pathWork, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24FOpen(&file, pathWork, NWC24_OPEN_NAND_RW);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24_ERR_NOT_FOUND;

    for (offset = sizeof(NWC24iMBCHeader); offset < pHeader->fileSize; offset += sizeof(NWC24iMBCHeader)) {
        fileResult = NWC24FSeek(&file, offset, NWC24_SEEK_BEG);
        if (fileResult != NWC24_OK) {
            result = fileResult;
            break;
        }
        fileResult = NWC24FRead(pEntry, sizeof(NWC24iMBCHeader), &file);
        if (fileResult != NWC24_OK) {
            result = fileResult;
            break;
        }

        if (pEntry->id == 0) {
            continue;
        }

        lastId++;

        if (pEntry->id == msgId) {
            if (checkPermission) {
                if ((pEntry->flags & NWC24_MSGOBJ_FOR_PUBLIC) && pEntry->appId == 0) {
                    if (NWC24GetAppId() != NWC24i_APP_ID_IPL) {
                        result = NWC24_ERR_PROTECTED;
                        break;
                    }

                } else if ((pEntry->appId & 0xFFFFFF00) != (NWC24GetAppId() & 0xFFFFFF00) &&

                           (!(pEntry->flags & NWC24_MSGOBJ_FOR_MENU) || NWC24GetAppId() != NWC24i_APP_ID_IPL)) {
                    result = NWC24_ERR_PROTECTED;
                    break;
                }
            }

            clearOffset = offset;
            result = NWC24_OK;
            continue;
        }

        if (oldestMsgId != 0) {
            if (CompareMsgId(oldestMsgId, pEntry->id) <= 0) {
                continue;
            }
        }

        oldestMsgId = pEntry->id;
    }

    if (result == NWC24_ERR_NOT_FOUND && pHeader->numMsgs != lastId) {
        pHeader->numMsgs = lastId;
        SaveMBCHeader(pHeader, &file);
    }

    if (result != NWC24_OK) {
        fileResult = NWC24FClose(&file);

        if (msgId == 0) {
            return NWC24_ERR_INVALID_VALUE;
        }

        return result;
    }

    pHeader->oldestMsgId = oldestMsgId;
    result = ClearMBCEntry(pHeader, &file, clearOffset);

    fileResult = DeleteMsgFile(id, msgId);
    result = result != NWC24_OK ? result : fileResult;

    fileResult = SaveMBCHeader(pHeader, &file);
    result = result != NWC24_OK ? result : fileResult;

    fileResult = NWC24FClose(&file);
    result = result != NWC24_OK ? result : fileResult;

    return result;
}

static NWC24Err DeleteMsgFile(NWC24MsgBoxId id, u32 msgId) {
    char* pPath;
    NWC24Err mountResult;
    NWC24Err result;

    mountResult = NWC24_OK;

    mountResult = MountVFMBox(id);
    if (mountResult != NWC24_OK) {
        return mountResult;
    }

    pPath = NWC24WorkP->pathWork;

    result = MakeMailPath(id, msgId, pPath, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24FDeleteVF(pPath);
    mountResult = UnmountVFMBox();

    result = result != NWC24_OK ? result : mountResult;
    return result;
}

static NWC24Err DuplicationCheck(NWC24iMBCHeader* pHeader, const NWC24iMsgObj* pMsg, NWC24File* pFile, NWC24MsgBoxId id) {
    NWC24Err result;
    NWC24Err deleteErr;
    u32 offset;
    u32 checkedCount;
    u32 bestMsgId;
    u32 bestOffset;
    u32 oldestMsgId;
    NWC24iMBCEntry* pEntry;
    char* pPath;

    result = NWC24_OK;
    checkedCount = 0;
    bestMsgId = 0;
    bestOffset = 0;
    oldestMsgId = 0;

    if (id == 0) {
        return NWC24_OK;
    }

    pEntry = &NWC24WorkP->mbcEntry;

    for (offset = sizeof(NWC24iMBCHeader); offset < pHeader->fileSize; offset += sizeof(NWC24iMBCHeader)) {
        BOOL isDuplicate = FALSE;

        if (checkedCount >= pHeader->numMsgs) {
            break;
        }

        result = NWC24FSeek(pFile, offset, NWC24_SEEK_BEG);
        if (result != NWC24_OK) {
            break;
        }
        result = NWC24FRead(pEntry, sizeof(NWC24iMBCHeader), pFile);
        if (result != NWC24_OK) {
            break;
        }

        if (pEntry->id == 0) {
            continue;
        }

        checkedCount++;
        isDuplicate = TRUE;

        if (pEntry->id == pMsg->id) {
            isDuplicate = FALSE;
        }

        if (pEntry->flags & pMsg->flags & NWC24_MSGOBJ_FOR_RECIPIENT) {
            if (pEntry->appId != pMsg->appId) {
                isDuplicate = FALSE;
            }

            if (pEntry->from.id != pMsg->from.id) {
                isDuplicate = FALSE;
            }
        }

        if (isDuplicate) {
            if ((pMsg->tag & 0xFFFF) != 0 && (pEntry->tag & 0xFFFF) == (pMsg->tag & 0xFFFF)) {
                bestMsgId = pEntry->id;
                bestOffset = offset;
                continue;
            }

            if (pMsg->unk1C != 0 && pEntry->unk1C == pMsg->unk1C && pEntry->flags == pMsg->flags && pEntry->length == pMsg->length &&
                pEntry->unk10 == pMsg->unk10 && pEntry->createTime == pMsg->createTime) {
                bestMsgId = pEntry->id;
                bestOffset = offset;
                continue;
            }
        }

        if (oldestMsgId != 0 && CompareMsgId(oldestMsgId, pEntry->id) <= 0) {
            continue;
        }

        oldestMsgId = pEntry->id;
    }

    if (result != NWC24_OK) {
        return result;
    }

    pHeader->oldestMsgId = oldestMsgId;

    if (bestMsgId != 0) {
        result = ClearMBCEntry(pHeader, pFile, bestOffset);
        deleteErr = DeleteMsgFile(id, bestMsgId);

        return result != NWC24_OK ? result : deleteErr;
    }

    return result;
}

static NWC24Err GetCachedMBCHeader(NWC24MsgBoxId id, NWC24iMBCHeader** ppHeader) {
    NWC24Err result;
    NWC24Err closeResult;
    NWC24File file;
    char* pPath;

    result = NWC24_OK;

    if (id == NWC24_MSGBOX_SEND) {
        *ppHeader = &NWC24WorkP->sendCtrl;
    } else if (id == NWC24_MSGBOX_RECV) {
        *ppHeader = &NWC24WorkP->recvCtrl;
    } else {
        *ppHeader = NULL;
        return NWC24_ERR_INVALID_VALUE;
    }

    if ((*ppHeader)->magic != MBOX_CTRL_MAGIC) {
        pPath = NWC24WorkP->pathWork;

        result = NWC24iMBoxGetCtrlFilePath(id, pPath, NWC24i_PATH_WORK_SIZE);
        if (result != NWC24_OK) {
            return result;
        }

        result = NWC24FOpen(&file, pPath, NWC24_OPEN_READ);
        if (result != NWC24_OK) {
            return result;
        }

        result = LoadMBCHeader(*ppHeader, &file);
        closeResult = NWC24FClose(&file);

        if (result == NWC24_OK && closeResult != NWC24_OK) {
            result = closeResult;
        }
    }

    if ((*ppHeader)->version != MBOX_CTRL_VERSION) {
        return NWC24_ERR_VER_MISMATCH;
    }
    if ((*ppHeader)->freeChain & 31) {
        return NWC24_ERR_BROKEN;
    }

    return result;
}

static NWC24Err LoadMBCHeader(NWC24iMBCHeader* pHeader, NWC24File* pFile) {
    NWC24Err result;

    result = NWC24FSeek(pFile, 0, NWC24_SEEK_BEG);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24FRead(pHeader, sizeof(NWC24iMBCHeader), pFile);
    if (result != NWC24_OK) {
        return result;
    }

    if (pHeader->magic != MBOX_CTRL_MAGIC) {
        return NWC24_ERR_BROKEN;
    }

    return NWC24_OK;
}

static NWC24Err SaveMBCHeader(NWC24iMBCHeader* pHeader, NWC24File* pFile) {
    NWC24Err result = NWC24FSeek(pFile, 0, NWC24_SEEK_BEG);
    if (result != NWC24_OK) {
        return result;
    }
    return NWC24FWrite(pHeader, sizeof(NWC24iMBCHeader), pFile);
}

static NWC24Err AddMBCEntry(NWC24iMBCHeader* pHeader, const NWC24iMsgObj* pMsg, NWC24File* pFile) {
    u32 headerNext;
    u32 entryNext;
    NWC24iMBCEntry* pEntry;
    NWC24Err result;

    headerNext = pHeader->freeChain;
    pEntry = &NWC24WorkP->mbcEntry;

    if (headerNext == 0) {
        return NWC24_ERR_FULL;
    }

    if (headerNext >= pHeader->fileSize || ((headerNext - sizeof(NWC24iMBCHeader)) & 0x7F)) {
        return NWC24_ERR_BROKEN;
    }

    result = NWC24FSeek(pFile, headerNext, NWC24_SEEK_BEG);
    if (result != NWC24_OK) {
        return result;
    }
    result = NWC24FRead(pEntry, sizeof(NWC24iMBCEntry), pFile);
    if (result != NWC24_OK) {
        return result;
    }

    entryNext = pEntry->appId;

    if (entryNext >= pHeader->fileSize || ((entryNext - sizeof(NWC24iMBCHeader)) & 0x7F)) {
        return NWC24_ERR_BROKEN;
    }

    pHeader->freeChain = entryNext;
    CopyMsgObjToMBCFmt(pMsg, pEntry);

    result = NWC24FSeek(pFile, headerNext, NWC24_SEEK_BEG);
    if (result != NWC24_OK) {
        return result;
    }
    result = NWC24FWrite(pEntry, sizeof(NWC24iMBCEntry), pFile);
    if (result != NWC24_OK) {
        return result;
    }

    if (pHeader->numMsgs == 0) {
        pHeader->oldestMsgId = pMsg->id;
    }

    pHeader->numMsgs++;
    pHeader->totalMsgSize += pMsg->length;

    return NWC24_OK;
}

static NWC24Err ClearMBCEntry(NWC24iMBCHeader* pHeader, NWC24File* pFile, u32 offset) {
    NWC24iMBCEntry* pEntry;
    NWC24Err result;

    pEntry = &NWC24WorkP->mbcEntry;

    result = NWC24FSeek(pFile, offset, NWC24_SEEK_BEG);
    if (result != NWC24_OK) {
        return result;
    }
    result = NWC24FRead(pEntry, sizeof(NWC24iMBCEntry), pFile);
    if (result != NWC24_OK) {
        return result;
    }

    pHeader->numMsgs--;
    pHeader->totalMsgSize -= pEntry->length;

    memset(pEntry, 0, sizeof(NWC24iMBCEntry));
    pEntry->appId = pHeader->freeChain;
    pHeader->freeChain = offset;

    result = NWC24FSeek(pFile, offset, NWC24_SEEK_BEG);
    if (result != NWC24_OK) {
        return result;
    }
    result = NWC24FWrite(pEntry, sizeof(NWC24iMBCEntry), pFile);
    return result;
}

static NWC24Err GetNewMsgId(NWC24iMBCHeader* pHeader, u32* pMsgId) {
    if (pHeader->magic != MBOX_CTRL_MAGIC) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (pHeader->nextMsgId > MSG_ID_MAX) {
        pHeader->nextMsgId = MSG_ID_MIN;
    }

    *pMsgId = pHeader->nextMsgId;
    pHeader->nextMsgId++;

    return NWC24_OK;
}

static int CompareMsgId(u32 lhs, u32 rhs) {
    if (lhs == rhs) {
        return 0;
    }

    if (lhs > 900000 && rhs < 100000) {
        return -1;
    }

    if (lhs < 100000 && rhs > 900000) {
        return 1;
    }

    return lhs > rhs ? 1 : -1;
}

static NWC24Err MountVFMBox(NWC24MsgBoxId id) {
    NWC24Err result;
    const char* pMBoxDir;
    char* pPath;

    if (MountInfo.count != 0 && MountInfo.id != id) {
        UnmountVFMBoxForced();
        return NWC24_ERR_FATAL;
    }

    if (++MountInfo.count > 1) {
        return NWC24_OK;
    }

    pPath = NWC24WorkP->pathWork;

    result = GetMBoxFilePath(id, pPath, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24MountVF(NWC24i_VF_DRIVE, pPath);
    if (result != NWC24_OK) {
        return result;
    }

    MountInfo.id = id;
    return NWC24_OK;
}

static NWC24Err UnmountVFMBox(void) {
    NWC24Err result;
    NWC24iMBCHeader* pHeader;
    u32 freeSize;

    if (MountInfo.count == 0) {
        return NWC24_OK;
    }

    if (--MountInfo.count > 0) {
        return NWC24_OK;
    }

    result = GetCachedMBCHeader(MountInfo.id, &pHeader);
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24CheckSizeVF(NWC24i_VF_DRIVE, &freeSize);
    if (result != NWC24_OK) {
        return result;
    }

    pHeader->freeSpace = freeSize;
    result = NWC24UnmountVF(NWC24i_VF_DRIVE);

    return result;
}

static NWC24Err UnmountVFMBoxForced(void) {
    MountInfo.count = 1;
    return UnmountVFMBox();
}

static u32 PackNWC24Data(const NWC24Data* pData) {
    return (pData->offset << 0) & 0x000FFFFF | (pData->size << 20) & 0xFFF00000;
}

static NWC24Err CopyMsgObjToMBCFmt(const NWC24iMsgObj* pSrc, NWC24iMBCEntry* pDst) {
    int i;

    u32 fromFieldPacked = PackNWC24Data(&pSrc->fromField);
    u32 toFieldPacked = PackNWC24Data(&pSrc->toField);
    u32 subjectPacked = PackNWC24Data(&pSrc->subject);
    u32 contentPacked = PackNWC24Data(&pSrc->contentType);
    u32 txEncodingPacked = PackNWC24Data(&pSrc->txEncoding);

    pDst->id = pSrc->id;
    pDst->flags = pSrc->flags;
    pDst->length = pSrc->length;
    pDst->appId = pSrc->appId;
    pDst->unk10 = pSrc->unk10;
    pDst->tag = pSrc->tag;
    pDst->command = pSrc->command;
    pDst->unk1C = pSrc->unk1C;
    pDst->from = pSrc->from;
    pDst->createTime = pSrc->createTime;
    pDst->unk2C = pSrc->unk2C;
    pDst->numTo = pSrc->numTo;
    pDst->numAttached = pSrc->numAttached;
    pDst->groupId = pSrc->groupId;

    pDst->fromField = fromFieldPacked;
    pDst->toField = toFieldPacked;
    pDst->subject = subjectPacked;
    pDst->contentType = contentPacked;
    pDst->txEncoding = txEncodingPacked;

    pDst->text = pSrc->text;
    pDst->dwcId = pSrc->dwcId;
    pDst->unk78 = pSrc->iconNew;

    for (i = 0; i < NWC24i_MSG_ATTACHMENT_MAX; i++) {
        pDst->attached[i] = pSrc->attached[i];
        pDst->attachedSize[i] = pSrc->attachedSize[i];
        pDst->attachedType[i] = pSrc->attachedType[i];
    }

    return NWC24_OK;
}

static NWC24Err CreateCtrlFile(NWC24MsgBoxId id, BOOL forceCreate) {
    NWC24Err result;
    NWC24Err close;
    NWC24File file;
    NWC24iMBCHeader* header;
    NWC24iMBCEntry* entry = &NWC24WorkP->mbcEntry;
    char* path = NWC24WorkP->pathWork;
    u32 offset;
    result = GetCachedMBCHeader(id, &header);
    if (result == NWC24_OK && !forceCreate) {
        return result;
    }
    result = NWC24iMBoxGetCtrlFilePath(id, path, NWC24i_PATH_WORK_SIZE);
    if (result != NWC24_OK) {
        return result;
    }
    result = NWC24FOpen(&file, path, NWC24_OPEN_NAND_W);
    if (result != NWC24_OK) {
        return result;
    }
    header = id == NWC24_MSGBOX_SEND ? &NWC24WorkP->sendCtrl : &NWC24WorkP->recvCtrl;
    Mail_memset(header, 0, sizeof(NWC24iMBCHeader));
    header->magic = MBOX_CTRL_MAGIC;
    header->version = MBOX_CTRL_VERSION;
    header->capacity = id == NWC24_MSGBOX_SEND ? 127 : 255;
    header->freeSpace = id == NWC24_MSGBOX_SEND ? 0x200000 : 0x700000;
    Mail_memset(header->lastUIDL, '0', sizeof(header->lastUIDL) - 1);
    header->fileSize = sizeof(NWC24iMBCHeader) + header->capacity * sizeof(NWC24iMBCEntry);
    header->nextMsgId = MSG_ID_MIN;
    header->freeChain = sizeof(NWC24iMBCHeader);
    result = NWC24FWrite(header, sizeof(NWC24iMBCHeader), &file);
    Mail_memset(entry, 0, sizeof(NWC24iMBCEntry));
    for (offset = sizeof(NWC24iMBCHeader); offset < header->fileSize; offset += sizeof(NWC24iMBCEntry)) {
        entry->appId = (offset + sizeof(NWC24iMBCEntry)) % header->fileSize;
        close = NWC24FWrite(entry, sizeof(NWC24iMBCEntry), &file);
        if (result == NWC24_OK) {
            result = close;
        }
    }
    close = NWC24FClose(&file);
    if (result != NWC24_OK) {
        return result;
    }
    return close != NWC24_OK ? close : 1;
}
