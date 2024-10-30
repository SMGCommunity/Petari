#include <RVLFaceLibInternal.h>

typedef struct RandomParam {
    u8 sex;     // at 0x0
    u8 age;     // at 0x1
    u8 race;    // at 0x2
    u8 padding; // at 0x3
} RandomParam;

typedef struct HiddenRandomParam {
    u16 dstIdx; // at 0x0
    u8 sex;     // at 0x2
    u8 padding; // at 0x3
} HiddenRandomParam;

typedef struct HiddenNewOldParam {
    s32 sex : 2;
    s32 srcIdx : 15;
    s32 dstIdx : 15;
} HiddenNewOldParam;

typedef struct Param2 {
    s16 lastSrcIdx; // at 0x0
    u16 padding;    // at 0x2
} Param2;

u32 RFLGetMiddleDBBufferSize(u16 size) {
    return size * sizeof(RFLiHiddenCharData);
}

void RFLInitMiddleDB(RFLMiddleDB* db, RFLMiddleDBType type, void* buffer,
                     u16 size) {
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db;
    HiddenNewOldParam* nparam;
    HiddenRandomParam* hparam;
    RandomParam* rparam;

    if (db == NULL) {
        return;
    }

    if (buffer == NULL) {
        return;
    }

    idb->type = type;
    idb->callback = NULL;
    idb->size = size;
    idb->data = (RFLiHiddenCharData*)buffer;
    idb->storedSize = 0;
    idb->userData1 = 0;
    idb->userData2 = 0;

    switch (type) {
    case RFLMiddleDBType_Random:
        rparam = (RandomParam*)&idb->userData1;

        if (size > RFL_DB_CHAR_MAX) {
            return;
        }

        rparam->sex = RFLSex_All;
        rparam->age = RFLAge_All;
        rparam->race = RFLRace_All;
        break;
    case RFLMiddleDBType_HiddenNewer:
    case RFLMiddleDBType_HiddenOlder:
        nparam = (HiddenNewOldParam*)&idb->userData1;

        if (size > RFLi_HDB_DATA_MAX) {
            return;
        }

        nparam->srcIdx = -1;
        nparam->dstIdx = 0;
        break;
    case RFLMiddleDBType_HiddenRandom:
        hparam = (HiddenRandomParam*)&idb->userData1;
        hparam->sex = RFLSex_All;
        hparam->dstIdx = 0;
        break;
    }

    memset(idb->data, 0, RFLGetMiddleDBBufferSize(idb->size));
}

static BOOL checkHiddenData_(RFLiHiddenCharData* data) {
    RFLiCharInfo info;

    if (!RFLiIsValidID(&data->createID)) {
        return FALSE;
    }

    RFLiConvertHRaw2Info(data, &info);
    return RFLiCheckValidInfo(&info) ? TRUE : FALSE;
}

static void updateHDBcallback_(u32 arg) {
    RFLErrcode err;
    HiddenNewOldParam* nparam;
    RFLiMiddleDB* db;

    db = (RFLiMiddleDB*)arg;
    nparam = (HiddenNewOldParam*)&db->userData1;

    if (RFLGetAsyncStatus() == RFLErrcode_Success ||
        RFLGetAsyncStatus() == RFLErrcode_Broken) {
        s16 src = -1;

        if (db->type == RFLMiddleDBType_HiddenOlder) {
            src = RFLiGetHiddenNext(nparam->srcIdx);
        } else {
            src = RFLiGetHiddenPrev(nparam->srcIdx);
        }

        if (RFLGetAsyncStatus() != RFLErrcode_Broken &&
            checkHiddenData_(&db->data[db->storedSize])) {
            db->storedSize++;
        }

        nparam->dstIdx++;

        if (src >= 0 && nparam->dstIdx < db->size) {
            nparam->srcIdx = src;

            err = RFLiLoadHiddenDataAsync(&db->data[db->storedSize],
                                          nparam->srcIdx, updateHDBcallback_,
                                          (u32)db);

            if (err != RFLErrcode_Busy) {
                RFLiEndWorking(err);
            }
        } else {
            RFLiGetManager()->lastErrCode = db->storedSize < db->size
                                                ? RFLErrcode_DBNodata
                                                : RFLErrcode_Success;
        }
    }

    if (!RFLiIsWorking()) {
        if (RFLGetAsyncStatus() == RFLErrcode_NANDCommandfail &&
            RFLGetLastReason() == NAND_RESULT_BUSY) {
            Param2* param2 = (Param2*)&db->userData2;
            nparam->srcIdx = param2->lastSrcIdx;
            db->storedSize = 0;
            nparam->dstIdx = 0;
        }

        if (db->callback != NULL) {
            db->callback();
        }
    }
}

static s16 stepOne_(s16 srcIdx, BOOL oldIsHead) {
    s16 ret;

    (void)RFLiGetHDBManager();

    ret = -1;

    if (oldIsHead) {
        if (srcIdx < 0) {
            ret = RFLiGetHiddenHeader()->head;
        } else {
            ret = RFLiGetHiddenNext(srcIdx);
        }
    } else {
        if (srcIdx < 0) {
            ret = RFLiGetHiddenHeader()->tail;
        } else {
            ret = RFLiGetHiddenPrev(srcIdx);
        }
    }

    return ret;
}

static void loadHiddenDataSync_(RFLiMiddleDB* db) {
    s16 src = -1;
    BOOL running = TRUE;
    HiddenNewOldParam* nparam = (HiddenNewOldParam*)&db->userData1;

    while (running) {
        RFLiLoadCachedHiddenData(&db->data[db->storedSize], nparam->srcIdx);

        if (db->type == RFLMiddleDBType_HiddenOlder) {
            src = RFLiGetHiddenNext(nparam->srcIdx);
        } else {
            src = RFLiGetHiddenPrev(nparam->srcIdx);
        }

        if (checkHiddenData_(&db->data[db->storedSize])) {
            db->storedSize++;
        }

        nparam->dstIdx++;

        if (src >= 0 && nparam->dstIdx < db->size) {
            nparam->srcIdx = src;
        } else {
            running = FALSE;
        }
    }

    RFLiEndWorking(nparam->srcIdx == -1 ? RFLErrcode_DBNodata
                                        : RFLErrcode_Success);
}

static void updateHiddenOld_(RFLiMiddleDB* db, BOOL oldIsHead, BOOL cache) {
    HiddenNewOldParam* nparam;
    s16 src;

    if (!RFLiDBIsLoaded()) {
        RFLiEndWorking(RFLErrcode_DBNodata);
        return;
    }

    if (db->data == NULL || db->size <= 0 || !RFLiDBIsLoaded()) {
        RFLiEndWorking(RFLErrcode_NotAvailable);
        return;
    }

    nparam = (HiddenNewOldParam*)&db->userData1;
    src = stepOne_(nparam->srcIdx, oldIsHead);

    if (src >= 0) {
        nparam->srcIdx = src;

        if (cache) {
            loadHiddenDataSync_(db);
        } else {
            RFLErrcode err = RFLiLoadHiddenDataAsync(
                &db->data[nparam->dstIdx], nparam->srcIdx, updateHDBcallback_,
                (u32)db);

            if (err != RFLErrcode_Busy) {
                RFLiEndWorking(err);
            }
        }
    } else {
        RFLiEndWorking(RFLErrcode_DBNodata);
    }
}

static void loadHiddenRandomSync_(RFLiMiddleDB* db) {
    BOOL running = TRUE;
    HiddenRandomParam* hparam = (HiddenRandomParam*)&db->userData1;

    while (running) {
        // Accesses gender/color/favorite bitfield. Is this done to be random?
        u32 src = *(u32*)&db->data[hparam->dstIdx];

        if (src > 0) {
            u16 srcIdx = src - 1;
            RFLiLoadCachedHiddenData(&db->data[db->storedSize], srcIdx);

            if (checkHiddenData_(&db->data[db->storedSize])) {
                db->storedSize++;
            }

            hparam->dstIdx++;

            if (src > 0 && hparam->dstIdx < db->size) {
                if (src >= RFLi_HDB_DATA_MAX) {
                    running = FALSE;
                }
            } else {
                running = FALSE;
            }

        } else {
            running = FALSE;
        }
    }

    RFLiGetManager()->lastErrCode =
        db->storedSize < db->size ? RFLErrcode_DBNodata : RFLErrcode_Success;
}

static void updateHDBRandcallback_(u32 arg) {
    RFLiMiddleDB* db = (RFLiMiddleDB*)arg;
    HiddenRandomParam* hparam = (HiddenRandomParam*)&db->userData1;
    u32* src;
    u16 srcIdx;

    if (RFLGetAsyncStatus() == RFLErrcode_Success ||
        RFLGetAsyncStatus() == RFLErrcode_Broken) {
        if (RFLGetAsyncStatus() != RFLErrcode_Broken &&
            checkHiddenData_(&db->data[db->storedSize])) {
            db->storedSize++;
        }

        hparam->dstIdx++;
        // Accesses gender/color/favorite bitfield. Is this done to be random?
        src = (u32*)&db->data[hparam->dstIdx];

        if (*src > 0 && hparam->dstIdx < db->size) {
            if (*src < RFLi_HDB_DATA_MAX) {
                u16 srcIdx = *src - 1;
                RFLErrcode err =
                    RFLiLoadHiddenDataAsync(&db->data[db->storedSize], srcIdx,
                                            updateHDBRandcallback_, (u32)db);

                if (err != RFLErrcode_Busy) {
                    RFLiEndWorking(err);
                }
            } else {
                RFLiGetManager()->lastErrCode = RFLErrcode_Broken;
            }
        } else {
            RFLiGetManager()->lastErrCode = db->storedSize < db->size
                                                ? RFLErrcode_DBNodata
                                                : RFLErrcode_Success;
        }
    }

    if (!RFLiIsWorking() && db->callback != NULL) {
        if (RFLGetAsyncStatus() == RFLErrcode_NANDCommandfail &&
            RFLGetLastReason() == NAND_RESULT_BUSY) {
            db->storedSize = 0;
            hparam->dstIdx = 0;
        }

        db->callback();
    }
}

static void updateHiddenRandom_(RFLiMiddleDB* db, BOOL cache) {
    u16 max;
    u16* array;
    HiddenRandomParam* hparam;
    RFLSex sex;
    u16 count;
    u32 rand;
    u16 aidx;
    u16 i;

    hparam = (HiddenRandomParam*)&db->userData1;
    sex = (RFLSex)hparam->sex;
    max = db->size;
    rand = OSGetTick();

    RFLiStartWorking();

    if (!RFLiDBIsLoaded()) {
        RFLiEndWorking(RFLErrcode_DBNodata);
        return;
    }

    count = RFLiCountupHiddenDataNum(sex);
    if (count <= 0) {
        RFLiEndWorking(RFLErrcode_DBNodata);
        return;
    }

    if (count < db->size) {
        max = count;
    }

    array = (u16*)RFLiAlloc(count * sizeof(u16));
    if (array == NULL) {
        RFLiEndWorking(RFLErrcode_Fatal);
        return;
    }

    aidx = 0;
    for (i = 0; i < RFLi_HDB_DATA_MAX; i++) {
        if (RFLiIsValidHiddenData(i, sex)) {
            array[aidx++] = i;
        }
    }

    for (i = 0; i < count - 1; i++) {
        u16 tmp;

        u16 target = (((rand >> 16) + rand) & 0xFFFF) % (count - 1);
        if (target >= i) {
            target++;
        }

        tmp = array[target];
        array[target] = array[i];
        array[i] = tmp;

        rand = 0x04F8BB63 * (rand + 0x046AC055);
    }

    for (i = 0; i < max; i++) {
        u32* src = (u32*)&db->data[i];
        *src = array[i] + 1;
    }

    RFLiFree(array);

    {
        u16 srcIdx = 0;
        u32* src = (u32*)&db->data[hparam->dstIdx];

        if (*src == 0) {
            RFLiEndWorking(RFLErrcode_DBNodata);
        } else {
            srcIdx = *src - 1;

            if (cache) {
                loadHiddenRandomSync_(db);
            } else {
                RFLErrcode err =
                    RFLiLoadHiddenDataAsync(&db->data[hparam->dstIdx], srcIdx,
                                            updateHDBRandcallback_, (u32)db);

                if (err != RFLErrcode_Busy) {
                    RFLiEndWorking(err);
                }
            }
        }
    }
}

static void updateRandom_(RFLiMiddleDB* db) {
    int count = 0;
    RandomParam* rparam = (RandomParam*)&db->userData1;
    int j;
    RFLiCharInfo info;

    RFLiStartWorking();

    while (db->storedSize < db->size) {
        BOOL isSame = FALSE;

        RFLi_MakeRandomFace(&info, (RFLSex)rparam->sex, (RFLAge)rparam->age,
                            (RFLRace)rparam->race);
        RFLiSetTemporaryID(&info);

        for (j = 0; j < db->storedSize; j++) {
            RFLiCharInfo temp;
            RFLiConvertHRaw2Info(&db->data[j], &temp);

            if (RFLiIsSameFaceCore(&info, &temp)) {
                isSame = TRUE;
                break;
            }
        }

        if (!isSame) {
            RFLiConvertInfo2HRaw(&info, &db->data[db->storedSize]);
            db->storedSize++;
        }
    }

    RFLiEndWorking(RFLErrcode_Success);
}

static void startUpdateDB_(RFLiMiddleDB* db) {
    Param2* param2;
    HiddenNewOldParam* nparam;
    HiddenRandomParam* hparam;

    db->storedSize = 0;
    memset(db->data, 0, RFLGetMiddleDBBufferSize(db->size));

    switch (db->type) {
    case RFLMiddleDBType_HiddenNewer:
    case RFLMiddleDBType_HiddenOlder:
        nparam = (HiddenNewOldParam*)&db->userData1;
        param2 = (Param2*)&db->userData2;

        nparam->dstIdx = 0;
        param2->lastSrcIdx = nparam->srcIdx;
        break;
    case RFLMiddleDBType_HiddenRandom:
        hparam = (HiddenRandomParam*)&db->userData1;
        param2 = (Param2*)&db->userData2;

        hparam->dstIdx = 0;
        param2->lastSrcIdx = 0;
        break;
    }
}

static RFLErrcode RFLUpdateMiddleDBAsync(RFLMiddleDB* db) {
    if (db == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (RFLAvailable()) {
        return RFLiUpdateMiddleDBAsync(db, NULL, FALSE);
    }

    return RFLErrcode_NotAvailable;
}

static RFLErrcode RFLiUpdateMiddleDBAsync(RFLMiddleDB* db, RFLiCallback cb,
                                   BOOL cache) {
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db;

    if (db == NULL) {
        return RFLErrcode_WrongParam;
    }

    idb->callback = cb;
    startUpdateDB_(idb);

    switch (idb->type) {
    case RFLMiddleDBType_HiddenRandom:
        updateHiddenRandom_(idb, cache);
        break;
    case RFLMiddleDBType_HiddenNewer:
        updateHiddenOld_(idb, FALSE, cache);
        break;
    case RFLMiddleDBType_HiddenOlder:
        updateHiddenOld_(idb, TRUE, cache);
        break;
    case RFLMiddleDBType_Random:
        updateRandom_(idb);
        break;
    }

    if (!RFLiIsWorking() && idb->callback != NULL) {
        idb->callback();
    }

    return RFLGetAsyncStatus();
}

static RFLMiddleDBType RFLGetMiddleDBType(const RFLMiddleDB* db) {
    return ((RFLiMiddleDB*)db)->type;
}

static u16 RFLGetMiddleDBStoredSize(const RFLMiddleDB* db) {
    return ((RFLiMiddleDB*)db)->storedSize;
}

BOOL RFLiGetCharInfoMiddleDB(RFLiCharInfo* info, const RFLMiddleDB* db,
                             u16 index) {
    RFLiHiddenCharData* data;
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db;

    if (idb == NULL) {
        return FALSE;
    }

    if (index >= idb->size) {
        return FALSE;
    }

    if (idb->storedSize <= 0) {
        return FALSE;
    }

    data = &idb->data[index];
    if (!RFLiIsValidID(&data->createID)) {
        return FALSE;
    }

    RFLiConvertHRaw2Info(data, info);
    return TRUE;
}

static void RFLSetMiddleDBRandomMask(RFLMiddleDB* db, RFLSex sex, RFLAge age,
                              RFLRace race) {
    RandomParam* rparam;
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db;

    if (RFLGetMiddleDBType(db) != RFLMiddleDBType_Random) {
        return;
    }

    rparam = (RandomParam*)&idb->userData1;
    rparam->sex = sex;
    rparam->age = age;
    rparam->race = race;
}

static void RFLSetMiddleDBHiddenMask(RFLMiddleDB* db, RFLSex sex) {
    HiddenRandomParam* hparam;
    HiddenNewOldParam* nparam;
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db;

    switch (RFLGetMiddleDBType(db)) {
    case RFLMiddleDBType_HiddenRandom:
        hparam = (HiddenRandomParam*)&idb->userData1;
        hparam->sex = sex;
        break;
    case RFLMiddleDBType_HiddenNewer:
    case RFLMiddleDBType_HiddenOlder:
        nparam = (HiddenNewOldParam*)&idb->userData1;
        nparam->sex = sex;
        break;
    }
}

static RFLErrcode RFLiAddMiddleDBUserData(RFLMiddleDB* db, RFLiCharData* raw) {
    RFLiHiddenCharData hraw;
    RFLiCharInfo info;
    RFLiMiddleDB* idb = (RFLiMiddleDB*)db;

    if (idb == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (raw == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    RFLiConvertRaw2HRaw(raw, &hraw);

    if (idb->type != RFLMiddleDBType_UserSet) {
        return RFLErrcode_NotAvailable;
    }

    if (!RFLiIsValidID(&hraw.createID)) {
        return RFLErrcode_Broken;
    }

    RFLiConvertRaw2Info(raw, &info);
    if (!RFLiCheckValidInfo(&info)) {
        return RFLErrcode_Broken;
    }

    if (idb->storedSize >= idb->size) {
        return RFLErrcode_DBFull;
    }

    memcpy(&idb->data[idb->storedSize], &hraw, sizeof(RFLiHiddenCharData));
    idb->storedSize++;

    return RFLErrcode_Success;
}

static RFLErrcode RFLAddMiddleDBStoreData(RFLMiddleDB* db, const RFLStoreData* data) {
    RFLiStoreData* idata = (RFLiStoreData*)data;

    if (db == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (data == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    if (RFLiCalculateCRC(data, sizeof(RFLStoreData)) != 0) {
        return RFLErrcode_Broken;
    }

    return RFLiAddMiddleDBUserData(db, &idata->data);
}
