#include <revolution/nwc24.h>
#include <revolution/nwc24/NWC24Internal.h>

#include <mem.h>

static const char* DLFilePath = "/shared2/wc24/nwc24dl.bin";

static BOOL IsPrivateId(u16 id);
static BOOL IsMyApp(u32 appId);
static BOOL IsGroupWritable(u16 groupId, u32 flags);
static NWC24iDlEntry* GetDlTaskEntryHeader(u16 id);
static NWC24Err WriteDlHeader(NWC24File* pFile);
static NWC24Err SeekDlTaskEntry(u16 id, NWC24File* pFile);
static NWC24Err CheckDlEntryAvailable(u16 id);
static NWC24Err WriteDlTaskEntry(NWC24iDlTask* pTask, NWC24File* pFile);
static NWC24Err ReadDlTaskEntry(NWC24iDlTask* pTask, u16 id, NWC24File* pFile);
static NWC24Err ClearDlTaskEntry(u16 id, NWC24File* pFile);
static NWC24Err ReadDlHeader(NWC24File* pFile);
static void InitTaskEntryHeader(u16 id);
static NWC24Err LoadDlTask(NWC24iDlTask* pTask, u16 id);
static NWC24Err DeleteDlTask(NWC24iDlTask* pTask) NO_INLINE;
static NWC24Err CheckHeader(NWC24iDlHeader* pHeader);

NWC24Err NWC24CheckDlTask(const NWC24DlTask* pTask, BOOL wantWrite) {
    NWC24iDlTask* pTaskImpl;
    NWC24iDlHeader* pHeader;

    pTaskImpl = (NWC24iDlTask*)pTask;
    pHeader = NWC24iGetCachedDlHeader();

    if (pTaskImpl == 0) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (pHeader == 0) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }

    if (pTaskImpl->appId == 0 || pTaskImpl->titleIdLo == 0) {
        return NWC24_ERR_INVALID_VALUE;
    }
    if (wantWrite && !NWC24IsMsgLibOpenedByTool() && !IsMyApp(pTaskImpl->appId)) {
        if (!IsGroupWritable(pTaskImpl->groupId, pTaskImpl->flags)) {
            return NWC24_ERR_PROTECTED;
        }
    }
    if (pTaskImpl->id != 0xFFFF && pTaskImpl->id >= pHeader->maxTasks) {
        return NWC24_ERR_INVALID_VALUE;
    }

    return NWC24_OK;
}

static BOOL IsGroupWritable(u16 groupId, u32 flags) {
    return (flags & 0x40) && groupId == NWC24GetGroupId();
}

static BOOL IsMyApp(u32 appId) {
    return (appId & 0xFFFFFF00) == (NWC24GetAppId() & 0xFFFFFF00);
}

static BOOL IsPrivateId(u16 id) {
    return id < NWC24iGetCachedDlHeader()->privateTasks;
}

NWC24Err NWC24DeleteDlTaskForced(NWC24DlTask* pTask) {
    NWC24iDlTask* pTaskImpl;
    NWC24Err result;

    pTaskImpl = (NWC24iDlTask*)pTask;

    result = NWC24CheckDlTask(pTask, FALSE);
    if (result != NWC24_OK) {
        return result;
    }

    result = CheckDlEntryAvailable(pTaskImpl->id);
    if (result < 0) {
        return result;
    }
    result = DeleteDlTask(pTaskImpl);
    if (result < 0) {
        return result;
    }

    pTaskImpl->id = 0xFFFF;
    return result;
}

NWC24Err NWC24GetDlTask(NWC24DlTask* pTask, u16 id) {
    NWC24iDlTask* pTaskImpl;
    NWC24Err result;

    pTaskImpl = (NWC24iDlTask*)pTask;

    if (NWC24iGetCachedDlHeader() == NULL) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }
    result = CheckDlEntryAvailable(id);
    if (result < 0) {
        return result;
    }

    result = LoadDlTask(pTaskImpl, id);
    if (result < 0) {
        return result;
    }

    return result;
}

NWC24Err NWC24iOpenDlTaskList(void) {
    NWC24Err result;

    result = NWC24iLoadDlHeader();

    if (result >= 0) {
        NWC24iSynchronizeRtcCounter(FALSE);
        result = NWC24iCheckDlHeaderConsistency(NWC24iGetCachedDlHeader(), FALSE);

        if (result >= 0) {
            return NWC24_OK;
        }
    }

    return result;
}

NWC24Err NWC24iCloseDlTaskList(void) {
    return NWC24_OK;
}

NWC24iDlHeader* NWC24iGetCachedDlHeader(void) {
    if (NWC24WorkP != 0)
        return (NWC24iDlHeader*)NWC24WorkP->dlHeader;
    else
        return 0;
}

NWC24Err NWC24iCheckDlHeaderConsistency(NWC24iDlHeader* pHeader, BOOL clear) {
    NWC24iDlTask* pTaskImpl;
    NWC24DlTask task;
    u16 i;

    pTaskImpl = (NWC24iDlTask*)&task;

    for (i = 0; i < pHeader->maxTasks; i++) {
        if (CheckDlEntryAvailable(i) == NWC24_OK && clear) {
            if (NWC24GetDlTask(&task, i) < 0) {
                NWC24DeleteDlTaskForced(&task);
            } else if (!IsPrivateId(i) && pTaskImpl->count == 0) {
                NWC24DeleteDlTaskForced(&task);
            }
        }
    }

    return NWC24_OK;
}

NWC24Err NWC24iLoadDlHeader(void) {
    NWC24File file;
    NWC24Err result;
    NWC24Err close;
    NWC24Err ret;
    u32 length;

    length = 0;

    result = NWC24FOpen(&file, DLFilePath, NWC24_OPEN_NAND_R);
    if (result < 0) {
        return result;
    }

    result = ReadDlHeader(&file);
    if (result < 0) {
        return result;
    }

    result = NWC24FGetLength(&file, &length);
    if (result >= 0) {
        result = CheckHeader(NWC24iGetCachedDlHeader());
    }

    close = NWC24FClose(&file);

    if (result != NWC24_OK) {
        ret = result;
    } else {
        ret = close;
    }

    return ret;
}

static NWC24iDlEntry* GetDlTaskEntryHeader(u16 id) {
    return &NWC24iGetCachedDlHeader()->entries[id];
}

static NWC24Err WriteDlHeader(NWC24File* pFile) {
    NWC24Err result;

    result = NWC24FSeek(pFile, 0, NWC24_SEEK_BEG);
    if (result < 0) {
        return result;
    }

    result = NWC24FWrite(NWC24iGetCachedDlHeader(), sizeof(NWC24iDlHeader), pFile);
    if (result < 0) {
        return result;
    }

    return NWC24_OK;
}

static NWC24Err SeekDlTaskEntry(u16 id, NWC24File* pFile) {
    u16 maxTasks = NWC24iGetCachedDlHeader()->maxTasks;
    if (maxTasks > NWC24i_DL_TASK_MAX || id >= maxTasks) {
        return NWC24_ERR_INVALID_VALUE;
    }
    return NWC24FSeek(pFile, sizeof(NWC24iDlHeader) + id * sizeof(NWC24iDlTask), NWC24_SEEK_BEG);
}

static NWC24Err CheckDlEntryAvailable(u16 id) {
    if (id >= NWC24iGetCachedDlHeader()->maxTasks || id == 0xFFFF) {
        return NWC24_ERR_INVALID_VALUE;
    }

    if (GetDlTaskEntryHeader(id)->app == 0) {
        return NWC24_ERR_NOT_FOUND;
    }

    return NWC24_OK;
}

static NWC24Err WriteDlTaskEntry(NWC24iDlTask* pTask, NWC24File* pFile) {
    NWC24Err result;

    result = SeekDlTaskEntry(pTask->id, pFile);
    if (result < 0) {
        return result;
    }

    memcpy(NWC24WorkP->dlTask, pTask, sizeof(NWC24iDlTask));
    result = NWC24FWrite(NWC24WorkP->dlTask, sizeof(NWC24iDlTask), pFile);
    if (result < 0) {
        return result;
    }

    return NWC24_OK;
}

static NWC24Err ReadDlTaskEntry(NWC24iDlTask* pTask, u16 id, NWC24File* pFile) {
    NWC24Err result;

    result = SeekDlTaskEntry(id, pFile);
    if (result < 0) {
        return result;
    }

    result = NWC24FRead(pTask, sizeof(NWC24iDlTask), pFile);
    if (result < 0) {
        return result;
    }

    return NWC24_OK;
}

static NWC24Err ClearDlTaskEntry(u16 id, NWC24File* pFile) {
    NWC24iDlTask* pTask = (NWC24iDlTask*)NWC24WorkP->dlTask;

    memset(pTask, 0, sizeof(NWC24iDlTask));
    pTask->type = 0xFF;
    pTask->id = id;

    InitTaskEntryHeader(id);
    return WriteDlTaskEntry(pTask, pFile);
}

static NWC24Err ReadDlHeader(NWC24File* pFile) {
    NWC24Err result;

    result = NWC24FSeek(pFile, 0, NWC24_SEEK_BEG);
    if (result < 0) {
        return result;
    }

    result = NWC24FRead(&NWC24WorkP->dlHeader, sizeof(NWC24iDlHeader), pFile);
    if (result < 0) {
        return result;
    }

    return NWC24_OK;
}

static void InitTaskEntryHeader(u16 id) {
    NWC24iDlHeader* pHeader = (NWC24iDlHeader*)NWC24WorkP->dlHeader;
    memset(&pHeader->entries[id], 0, sizeof(NWC24iDlEntry));
}

static NWC24Err LoadDlTask(NWC24iDlTask* pTask, u16 id) {
    NWC24File file;
    NWC24Err result;
    NWC24Err close;
    NWC24Err ret;

    result = NWC24FOpen(&file, DLFilePath, NWC24_OPEN_NAND_RBUFF);
    if (result < 0) {
        return result;
    }

    result = ReadDlTaskEntry(pTask, id, &file);
    close = NWC24FClose(&file);

    if (result != NWC24_OK) {
        ret = result;
    } else {
        ret = close;
    }

    return ret;
}

static NWC24Err DeleteDlTask(NWC24iDlTask* pTask) {
    NWC24File file;
    NWC24Err result;
    NWC24Err close;
    NWC24Err ret;

    result = NWC24FOpen(&file, DLFilePath, NWC24_OPEN_NAND_RW);
    if (result < 0) {
        return result;
    }

    result = ClearDlTaskEntry(pTask->id, &file);
    if (result >= 0) {
        InitTaskEntryHeader(pTask->id);
        result = WriteDlHeader(&file);
    }

    close = NWC24FClose(&file);

    if (result != NWC24_OK) {
        ret = result;
    } else {
        ret = close;
    }

    return ret;
}

static NWC24Err CheckHeader(NWC24iDlHeader* pHeader) {
    NWC24File file;
    if (pHeader->maxTasks == 0 && pHeader->maxSubTasks != 0) {
        pHeader->maxTasks = pHeader->maxSubTasks;
        if (pHeader->maxSubTasks > 32) {
            pHeader->maxSubTasks = 32;
        }
        if (NWC24FOpen(&file, DLFilePath, NWC24_OPEN_NAND_RW) >= 0) {
            WriteDlHeader(&file);
            NWC24FClose(&file);
        }
    }

    if (pHeader->maxTasks < 1 || pHeader->privateTasks < 1 || pHeader->maxTasks < pHeader->privateTasks) {
        return NWC24_ERR_BROKEN;
    }

    return NWC24_OK;
}
