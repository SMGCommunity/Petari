#include <RVLFaceLibInternal.h>
#include <cstdio>

static void writeData_(RFLiHiddenCharData* data) NO_INLINE;

static void initWritableList_(void) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();
    mgr->writeCb = NULL;
    mgr->list.num = 0;
    mgr->list.current = 0;
}

void RFLiInitHiddenDatabase(void) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();
    if (mgr != NULL) {
        mgr->cachedDB = NULL;
        mgr->cached = FALSE;
    }
}

static void loadclosecallback_(void) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();
    if (mgr->loadCb != NULL) {
        mgr->loadCb(mgr->loadArg);
    }
}

static void loadcallback_(void) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        RFLiHiddenCharData* data = mgr->loadTmp;
        RFLiCharInfo info;

        if (RFLiIsSameID(
                &data->createID,
                &RFLiGetHiddenHeader()->data[mgr->loadIndex].createID)) {
            RFLiConvertHRaw2Info(data, &info);

            if (RFLiCheckValidInfo(&info) && RFLiIsValidOnNAND(&info)) {
                memcpy(mgr->loadDst, data, sizeof(RFLiHiddenCharData));
            } else {
                RFLiGetManager()->lastErrCode = RFLErrcode_Broken;
            }
        }
    }

    RFLiFree(mgr->loadTmp);
    mgr->loadTmp = NULL;

    RFLiCloseAsync(RFLiFileType_Database, loadclosecallback_);
}

static void loadopencallback_(void) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        s32 offset = mgr->loadIndex * sizeof(RFLiHiddenCharData);
        mgr->loadTmp = RFLiAlloc32(sizeof(RFLiHiddenCharData));

        switch (RFLiReadAsync(RFLiFileType_Database, mgr->loadTmp,
                              sizeof(RFLiHiddenCharData), loadcallback_,
                              offset + sizeof(RFLiDatabase))) {
        case RFLErrcode_Success:
        case RFLErrcode_Busy:
            break;
        default:
            RFLiFree(mgr->loadTmp);
            mgr->loadTmp = NULL;
            RFLiCloseAsync(RFLiFileType_Database, NULL);
        }
    } else {
        mgr->loadDst = NULL;
        mgr->loadTmp = NULL;
        mgr->loadIndex = 0;
        mgr->loadCb = NULL;
    }
}

RFLErrcode RFLiLoadHiddenDataAsync(RFLiHiddenCharData* data, u16 index,
                                   RFLiExCallback cb, u32 arg) {
    RFLiHDBManager* mgr;

    if (index >= RFLi_HDB_DATA_MAX) {
        return RFLErrcode_WrongParam;
    }

    if (data == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    mgr = RFLiGetHDBManager();
    if (mgr == NULL) {
        return RFLErrcode_NotAvailable;
    }

    mgr->loadDst = data;
    mgr->loadIndex = index;
    mgr->loadCb = cb;
    mgr->loadArg = arg;

    return RFLiOpenAsync(RFLiFileType_Database, 1,
                         loadopencallback_);
}

RFLErrcode RFLiLoadCachedHiddenData(RFLiHiddenCharData* data, u16 index) {
    RFLiHDBManager* mgr;
    RFLiCharInfo info;

    if (index >= RFLi_HDB_DATA_MAX) {
        return RFLErrcode_WrongParam;
    }

    if (data == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    mgr = RFLiGetHDBManager();
    if (mgr == NULL) {
        return RFLErrcode_NotAvailable;
    }

    if (RFLiIsCachedHDB()) {
        if (RFLiIsSameID(&mgr->cachedDB[index].createID,
                         &RFLiGetHiddenHeader()->data[index].createID)) {
            RFLiConvertHRaw2Info(&mgr->cachedDB[index], &info);

            if (RFLiCheckValidInfo(&info)) {
                memcpy(data, &mgr->cachedDB[index], sizeof(RFLiHiddenCharData));
                return RFLErrcode_Success;
            } else {
                return RFLErrcode_Broken;
            }
        } else {
            return RFLErrcode_Broken;
        }
    } else {
        return RFLErrcode_NotAvailable;
    }
}

static void writeCtrl2HDBCallback_(void) {
    RFLiCloseAsync(RFLiFileType_Database, NULL);
}

static int overwrite_(const RFLiHiddenCharData* data) {
    RFLiHiddenDB* hdb = RFLiGetHiddenHeader();
    s16 head = hdb->head;

    RFLiTableData* it = &hdb->data[head];
    s16 next = it->next;
    s16 prev = hdb->tail;

    RFLiTableData* itnext = &hdb->data[next];
    RFLiTableData* itprev = &hdb->data[prev];

    if (head < 0 || head >= RFLi_HDB_DATA_MAX) {
        return -1;
    }

    RFLiClearTableData(it);
    hdb->head = next;
    itnext->prev = -1;
    itprev->next = head;
    it->prev = prev;
    hdb->tail = head;
    it->next = -1;

    memcpy(&it->createID, &data->createID, sizeof(RFLCreateID));
    it->sex = data->sex;

    return head;
}

static void create_(const RFLiHiddenCharData* data, s16 index) {
    RFLiHiddenDB* hdb = RFLiGetHiddenHeader();
    RFLiTableData* it = &hdb->data[index];

    if (index < 0 || index >= RFLi_HDB_DATA_MAX) {
        return;
    }

    RFLiClearTableData(it);

    if (hdb->tail >= 0) {
        hdb->data[hdb->tail].next = index;
        it->prev = hdb->tail;
        hdb->tail = index;
        it->next = -1;
    } else {
        hdb->tail = index;
        hdb->head = index;
        it->prev = -1;
        it->next = -1;
    }

    memcpy(&it->createID, &data->createID, sizeof(RFLCreateID));
    it->sex = data->sex;
}

static void writeHeader_(void) {
    RFLiSaveDatabaseAsync(RFLiGetHDBManager()->writeCb);
}

static void writeCallback_(void) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        RFLiHDBList* list = &RFLiGetHDBManager()->list;

        if (RFLiIsCachedHDB()) {
            memcpy(&mgr->cachedDB[mgr->writeIndex], mgr->writeTmp,
                   sizeof(RFLiHiddenCharData));
        }

        list->current++;
        if (list->current < list->num) {
            writeData_(&list->data[list->current]);
        } else {
            RFLiFree(mgr->writeTmp);
            mgr->writeTmp = NULL;
            writeHeader_();
        }
    } else {
        RFLiFree(mgr->writeTmp);
        mgr->writeTmp = NULL;
        RFLiCloseAsync(RFLiFileType_Database, NULL);
    }
}

static BOOL checkCtrlWritableData_(const RFLiCtrlBuf* buf, BOOL ch, u8 index) {
    if (RFLiCheckCtrlBufferCore(buf, index,
                                ch ? RFLiHiddenType_Yes : RFLiHiddenType_Any)) {
        RFLiHiddenCharData data;
        RFLiConvertRaw2HRaw(&buf->data[index], &data);

        if (!data.localonly && !RFLSearchOfficialData(&data.createID, NULL)) {
            return TRUE;
        }
    }

    return FALSE;
}

static u8 getCtrlWritableCount_(const RFLiCtrlBuf* buf, volatile s64 ch) {
    u8 i;
    u8 count = 0;
    RFLiHDBManager* mgr = RFLiGetHDBManager();
    RFLiHiddenCharData* dst;

    for (i = 0; i < RFL_CTRL_CHAR_MAX; i++) {
        if (checkCtrlWritableData_(buf, ch, i)) {

            RFLiHiddenCharData temp;
            RFLiConvertRaw2HRaw(&buf->data[i], &temp);

            dst = &mgr->list.data[mgr->list.num];
            memcpy(dst, &temp, sizeof(RFLiHiddenCharData));
            dst->favorite = FALSE;

            count++;
            mgr->list.num++;
        }
    }

    return count;
}

static s32 getFirstBlank_(void) {
    RFLiTableData* head = RFLiGetHiddenHeader()->data;
    int i;

    for (i = 0; i < RFLi_HDB_DATA_MAX; i++) {
        if (!RFLiIsValidID(&head[i].createID)) {
            return i;
        }
    }

    return -1;
}

static void writeData_(RFLiHiddenCharData* data) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();
    s32 blank = getFirstBlank_();
    s32 index = RFLiSearchHiddenData(&data->createID);

    if ((s16)index != -1) {
        blank = index;
    } else if ((s16)blank < 0) {
        blank = overwrite_(data);
    } else {
        create_(data, blank);
    }

    memcpy(mgr->writeTmp, data, sizeof(RFLiHiddenCharData));
    mgr->writeIndex = blank;
    RFLiWriteAsync(RFLiFileType_Database, mgr->writeTmp,
                   sizeof(RFLiHiddenCharData), writeCallback_,
                   (s16)blank * sizeof(RFLiHiddenCharData) +
                       sizeof(RFLiDatabase));
}

static void openForWriteCallback_(void) {
    RFLiHDBManager* mgr;
    RFLiHDBList* list;

    if (RFLGetAsyncStatus() != RFLErrcode_Success) {
        return;
    }

    mgr = RFLiGetHDBManager();
    list = &mgr->list;

    if (list->current < list->num) {
        mgr->writeTmp = RFLiAlloc32(sizeof(RFLiHiddenCharData));
        writeData_(&list->data[list->current]);
    }
}

static void writeDataStart_(void) {
    RFLiOpenAsync(RFLiFileType_Database, 2,
                  openForWriteCallback_);
}

static void writeHeaderStart_(void) {
    RFLiOpenAsync(RFLiFileType_Database, 2, writeHeader_);
}

static void writeCtrl2HDB_(RFLiCallback formatCb, RFLiCallback writeCb) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();
    mgr->writeCb = writeCtrl2HDBCallback_;

    if (!RFLiDBIsLoaded()) {
        RFLiFormatAsync(formatCb);
    } else {
        RFLiOpenAsync(RFLiFileType_Database, 2, writeCb);
    }
}

RFLErrcode RFLiWriteCtrlToHiddenDB(const RFLiCtrlBuf* buf, BOOL ch) {
    initWritableList_();

    if (getCtrlWritableCount_(buf, ch) > 0) {
        writeCtrl2HDB_(writeDataStart_, openForWriteCallback_);
    } else {
        writeCtrl2HDB_(writeHeaderStart_, writeHeader_);
    }

    return RFLGetAsyncStatus();
}

s32 RFLiSearchHiddenData(const RFLCreateID* id) {
    RFLiHDBManager* mgr;
    RFLiTableData* head;
    int i;
    s32 index = -1;

    if (!RFLAvailable()) {
        return -1;
    }

    mgr = RFLiGetHDBManager();
    if (mgr == NULL) {
        return -1;
    }

    head = RFLiGetHiddenHeader()->data;
    for (i = 0; i < RFLi_HDB_DATA_MAX; i++) {
        if (RFLiIsSameID(&head[i].createID, id)) {
            index = i;
            break;
        }
    }

    return index;
}

u16 RFLiCountupHiddenDataNum(RFLSex sex) {
    RFLiHDBManager* mgr;
    RFLiHiddenDB* header;
    u16 count = 0;
    u16 i;

    if (!RFLAvailable()) {
        return 0;
    }

    mgr = RFLiGetHDBManager();
    if (mgr == NULL) {
        return 0;
    }

    if (!RFLiDBIsLoaded()) {
        return 0;
    }

    header = RFLiGetHiddenHeader();

    for (i = 0; i < RFLi_HDB_DATA_MAX; i++) {
        if (RFLiIsValidHiddenData(i, sex)) {
            count++;
        }
    }

    return count;
}

s16 RFLiGetHiddenNext(u16 index) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();
    RFLiTableData* data;

    if (index >= RFLi_HDB_DATA_MAX) {
        return -1;
    }

    if (mgr == NULL) {
        return -1;
    }

    data = &RFLiGetHiddenHeader()->data[index];

    if (RFLiIsValidID(&data->createID)) {
        return data->next;
    }

    return -1;
}

s16 RFLiGetHiddenPrev(u16 index) {
    RFLiHDBManager* mgr = RFLiGetHDBManager();
    RFLiTableData* data;

    if (index >= RFLi_HDB_DATA_MAX) {
        return -1;
    }

    if (mgr == NULL) {
        return -1;
    }

    data = &RFLiGetHiddenHeader()->data[index];

    if (RFLiIsValidID(&data->createID)) {
        return data->prev;
    }

    return -1;
}

BOOL RFLiIsValidHiddenData(u16 index, RFLSex sex) {
    RFLiHDBManager* mgr;
    RFLiHiddenDB* header;
    u16 count;

    if (!RFLAvailable()) {
        return FALSE;
    }

    mgr = RFLiGetHDBManager();
    if (mgr == NULL) {
        return FALSE;
    }

    header = RFLiGetHiddenHeader();

    if (!RFLiIsValidID(&header->data[index].createID)) {
        return FALSE;
    }

    if (sex == RFLSex_All) {
        return TRUE;
    }

    switch (header->data[index].sex) {
    case RFLSex_Male:
        return sex == RFLSex_Male;
    case RFLSex_Female:
        return sex == RFLSex_Female;
    }

    return FALSE;
}

void RFLiClearCacheHDB(RFLiHiddenCharData* cache) {
    memset(cache, 0, sizeof(RFLiHiddenCharData) * RFLi_HDB_DATA_MAX);
}

BOOL RFLiIsCachedHDB(void) {
    RFLiHDBManager* mgr;

    if (!RFLAvailable()) {
        return FALSE;
    }

    mgr = RFLiGetHDBManager();
    if (mgr == NULL) {
        return FALSE;
    }

    return mgr->cached;
}
