#include <RVLFaceLibInternal.h>
#include <cstdio>

#define MAGIC_OFFICIAL_DB 'RNOD'
#define MAGIC_HIDDEN_DB 'RNHD'

#define TABLE_DATA_STEP 1250
// TODO: How is this size calculated?
#define TEMP_BUFFER_SIZE (TABLE_DATA_STEP * sizeof(RFLiHiddenCharData) + 0x30E0)

void RFLiClearTableData(RFLiTableData* data) {
    memset(data, 0, sizeof(RFLiTableData));
    data->next = -1;
    data->prev = -1;
}

void RFLiClearDBBuffer(void) {
    RFLiDatabase* db;
    RFLiHiddenDB* header;
    int i;

    db = RFLiGetDBManager()->database;
    header = &db->hidden;

    // @bug Four-byte buffer overrun
    memset((u8*)db + sizeof(db->identifier), 0, sizeof(RFLiDatabase));
    db->identifier = MAGIC_OFFICIAL_DB;
    db->isolation = TRUE;

    header->identifier = MAGIC_HIDDEN_DB;
    header->head = -1;
    header->tail = -1;

    for (i = 0; i < RFLi_HDB_DATA_MAX; i++) {
        RFLiClearTableData(&header->data[i]);
    }
}

static void formatCloseCallback_(void) {
    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        RFLiBootLoadDatabaseAsync(RFLiGetDBManager()->formatCb);
    }
}

static void maxblockCloseCallback_(void) {
    RFLiGetManager()->lastErrCode = RFLErrcode_MaxBlocks;
}

static void formatWriteCallback_(void) {
    RFLiHDBManager* mgr;
    RFLErrcode status;
    BOOL success;
    s32 offset;
    int i;

    mgr = RFLiGetHDBManager();
    success = FALSE;
    status = RFLGetAsyncStatus();

    if (status == RFLErrcode_Success) {
        if (mgr->formatIndex == -1) {
            mgr->formatIndex = 0;
        } else {
            mgr->formatIndex += TABLE_DATA_STEP;
        }

        if (mgr->formatIndex < RFLi_HDB_DATA_MAX) {
            offset = mgr->formatIndex * sizeof(RFLiHiddenCharData);

            for (i = 0; i < TABLE_DATA_STEP; i++) {
                memset((RFLiHiddenCharData*)mgr->formatTmp + i, 0,
                       sizeof(RFLiHiddenCharData));
            }

            switch (RFLiWriteAsync(RFLiFileType_Database, mgr->formatTmp,
                                   TEMP_BUFFER_SIZE, formatWriteCallback_,
                                   offset + sizeof(RFLiDatabase))) {
            case RFLErrcode_Success:
            case RFLErrcode_Busy:
                success = TRUE;
            }
        } else if (RFLiIsCachedHDB()) {
            RFLiClearCacheHDB(mgr->cachedDB);
        }
    } else if (status == RFLErrcode_NANDCommandfail) {
        if (RFLiGetLastReason() == NAND_RESULT_MAXBLOCKS) {
            RFLiCloseAsync(RFLiFileType_Database, maxblockCloseCallback_);
        } else {
            RFLiCloseAsync(RFLiFileType_Database, NULL);
        }
    }

    if (!success) {
        RFLiFree(mgr->formatTmp);
        mgr->formatTmp = NULL;
        RFLiCloseAsync(RFLiFileType_Database, formatCloseCallback_);
    }
}

static void formatCalcCRCCb_(u32 arg) {
#pragma unused(arg)
    RFLiWriteAsync(RFLiFileType_Database, RFLiGetDBManager()->database,
                   sizeof(RFLiDatabase), formatWriteCallback_, 0);
}

static void formatOpenCallback_(void) {
    RFLiHDBManager* mgr;
    RFLErrcode status;

    status = RFLGetAsyncStatus();

    if (status == RFLErrcode_Success) {
        mgr = RFLiGetHDBManager();
        mgr->formatIndex = -1;
        mgr->formatTmp = RFLiAlloc32(TEMP_BUFFER_SIZE);
        RFLiCreateHeaderCRCAsync(formatCalcCRCCb_);
    } else if (status == RFLErrcode_NANDCommandfail) {
        if (RFLiGetLastReason() == NAND_RESULT_MAXFILES) {
            RFLiGetManager()->lastErrCode = RFLErrcode_MaxFiles;
        }
    } else {
        RFLiCloseAsync(RFLiFileType_Database, formatCloseCallback_);
    }
}

static void formatDeleteCallback_(void) {
    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        RFLiOpenAsync(RFLiFileType_Database, 2,
                      formatOpenCallback_);
    }
}

static void formatCreateDirCallback_(void) {
    RFLErrcode status;

    status = RFLGetAsyncStatus();

    if (status == RFLErrcode_Success) {
        RFLiDeleteAsync(RFLiFileType_Database, formatDeleteCallback_);
    } else if (status == RFLErrcode_NANDCommandfail &&
               RFLiGetLastReason() == NAND_RESULT_MAXFILES) {
        RFLiGetManager()->lastErrCode = RFLErrcode_MaxFiles;
    }
}

RFLErrcode RFLiFormatAsync(RFLiCallback cb) {
    RFLiDBManager* mgr;

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    mgr = RFLiGetDBManager();
    if (mgr == NULL) {
        return RFLErrcode_NotAvailable;
    }

    RFLiClearDBBuffer();
    RFLiGetManager()->brokenType =
        RFLiGetManager()->brokenType & (1 << RFLiFileBrokenType_DBBroken);

    mgr->formatCb = cb;
    RFLiCreateSaveDirAsync(formatCreateDirCallback_);
}