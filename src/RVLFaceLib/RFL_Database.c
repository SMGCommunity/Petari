#include <RVLFaceLibInternal.h>

#define ASYNC_CRC_STEP 0x1400

void RFLiInitDatabase(MEMiHeapHead* heap) {
    RFLiDBManager* mgr = RFLiGetDBManager();

    mgr->database =
        (RFLiDatabase*)MEMAllocFromExpHeapEx(heap, sizeof(RFLiDatabase), 32);
    mgr->saveCb = NULL;
    mgr->formatCb = NULL;
    mgr->bootloadCb = NULL;

    RFLiClearDBBuffer();
}

static void initBrokenDatabase_(void) {
    RFLiClearDBBuffer();
}

static void bootloadCheckCRCCb_(u32 crc) {
    RFLiDBManager* mgr = RFLiGetDBManager();

    if (crc != 0) {
        RFLiGetManager()->lastErrCode = RFLErrcode_Loadfail;
        RFLiSetFileBroken(RFLiFileBrokenType_DBBroken);
    } else {
        RFLiGetManager()->lastErrCode = RFLErrcode_Success;
    }

    RFLiCloseAsync(RFLiFileType_Database, mgr->bootloadCb);
}

static void bootloadDBcallback_(void) {
    RFLiDBManager* mgr = RFLiGetDBManager();

    switch (RFLGetAsyncStatus()) {
    case RFLErrcode_Success:
        RFLiCheckHeaderCRCAsync(bootloadCheckCRCCb_);
        break;
    case RFLErrcode_NANDCommandfail:
        switch (RFLiGetLastReason()) {
        case NAND_RESULT_CORRUPT:
            RFLiSetFileBroken(RFLiFileBrokenType_Corrupt);
            break;
        case NAND_RESULT_AUTHENTICATION:
        case NAND_RESULT_ECC_CRIT:
        default:
            RFLiGetManager()->lastErrCode = RFLErrcode_Loadfail;
            RFLiSetFileBroken(RFLiFileBrokenType_DBBroken);
            break;
        }
        RFLiCloseAsync(RFLiFileType_Database, mgr->bootloadCb);
        break;
    case RFLErrcode_NotAvailable:
    default:
        RFLiSetFileBroken(RFLiFileBrokenType_DBBroken);
        RFLiCloseAsync(RFLiFileType_Database, mgr->bootloadCb);
        break;
    }
}

static void bootloadDBopencallback_(void) {
    RFLiDBManager* mgr = RFLiGetDBManager();

    switch (RFLGetAsyncStatus()) {
    case RFLErrcode_Success:
        switch (RFLiReadAsync(RFLiFileType_Database, mgr->database,
                              sizeof(RFLiDatabase), bootloadDBcallback_, 0)) {
        case RFLErrcode_Busy:
        case RFLErrcode_Success:
            break;
        default:
            if (mgr->bootloadCb != NULL) {
                mgr->bootloadCb();
            }
            break;
        }
        break;
    case RFLErrcode_NANDCommandfail:
        switch (RFLiGetLastReason()) {
        case NAND_RESULT_NOEXISTS:
            RFLiSetFileBroken(RFLiFileBrokenType_DBNotFound);
            RFLiGetManager()->lastErrCode = RFLErrcode_Success;
            if (mgr->bootloadCb != NULL) {
                mgr->bootloadCb();
            }
            break;
        case NAND_RESULT_CORRUPT:
            RFLiSetFileBroken(RFLiFileBrokenType_Corrupt);
            break;
        case NAND_RESULT_AUTHENTICATION:
        case NAND_RESULT_ECC_CRIT:
            RFLiSetFileBroken(RFLiFileBrokenType_DBBroken);
            if (mgr->bootloadCb != NULL) {
                mgr->bootloadCb();
            }
            break;
        default:
            break;
        }
        break;
    case RFLErrcode_NotAvailable:
    default:
        break;
    }
}

RFLErrcode RFLiBootLoadDatabaseAsync(RFLiCallback callback) {
    RFLiDBManager* mgr;

    if (!RFLAvailable()) {
        return RFLErrcode_Fatal;
    }

    if (RFLiIsWorking()) {
        return RFLErrcode_Fatal;
    }

    mgr = RFLiGetDBManager();
    if (mgr == NULL) {
        return RFLErrcode_Fatal;
    }

    mgr->bootloadCb = callback;

    switch (RFLiOpenAsync(RFLiFileType_Database, 1,
                          bootloadDBopencallback_)) {
    case RFLErrcode_NANDCommandfail:
        if (RFLiGetLastReason() == NAND_RESULT_NOEXISTS) {
            initBrokenDatabase_();
            RFLiGetManager()->lastErrCode = RFLErrcode_Success;

            if (mgr->bootloadCb != NULL) {
                mgr->bootloadCb();
            }
        }
        break;
    case RFLErrcode_Success:
    case RFLErrcode_Busy:
    default:
        break;
    }

    return RFLGetAsyncStatus();
}

static void saveDBcallback_(void) {
    RFLiCloseAsync(RFLiFileType_Database, RFLiGetDBManager()->saveCb);
}

static RFLErrcode saveCore_(RFLiCallback callback) {
    return RFLiWriteAsync(RFLiFileType_Database, RFLiGetDBManager()->database,
                          sizeof(RFLiDatabase), callback, 0);
}

static void saveDBopencallback_(void) {
    (void)RFLiGetDBManager();

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        switch (saveCore_(saveDBcallback_)) {
        case RFLErrcode_Success:
        case RFLErrcode_Busy:
            break;
        default:
            RFLiCloseAsync(RFLiFileType_Database, NULL);
            break;
        }
    }
}

static void saveDBmultiopencallback_(void) {
    (void)RFLiGetDBManager();

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        switch (saveCore_(RFLiGetDBManager()->saveCb)) {
        case RFLErrcode_Success:
        case RFLErrcode_Busy:
            break;
        default:
            RFLiCloseAsync(RFLiFileType_Database, NULL);
            break;
        }
    }
}

static void saveDatabase_(u32 arg) {
    switch (RFLiGetDBManager()->saveType) {
    case RFLiOpenType_Multi:
        RFLiOpenAsync(RFLiFileType_Database, 2,
                      saveDBmultiopencallback_);
        break;
    case RFLiOpenType_Single:
        RFLiOpenAsync(RFLiFileType_Database, 2,
                      saveDBopencallback_);
        break;
    case RFLiOpenType_Opened:
        saveCore_(RFLiGetDBManager()->saveCb);
        break;
    default:
        break;
    }
}

static RFLErrcode RFLiSaveDatabaseAsync(RFLiCallback callback) {
    RFLiDBManager* mgr;

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    mgr = RFLiGetDBManager();
    if (mgr == NULL || !RFLiDBIsLoaded()) {
        return RFLErrcode_NotAvailable;
    }

    mgr->saveCb = callback;
    mgr->saveType = RFLiOpenType_Opened;

    RFLiCreateHeaderCRCAsync(saveDatabase_);
    return RFLGetAsyncStatus();
}

RFLiCharData* RFLiGetCharData(u16 index) {
    RFLiDBManager* mgr;
    RFLiCharData* data;
    RFLiCharInfo info;

    if (index >= RFL_DB_CHAR_MAX) {
        return NULL;
    }

    if (!RFLAvailable()) {
        return NULL;
    }

    mgr = RFLiGetDBManager();
    if (mgr == NULL || !RFLiDBIsLoaded()) {
        return NULL;
    }

    data = &mgr->database->rawData[index];

    if (!RFLiIsValidID(&data->createID)) {
        return NULL;
    }

    RFLiConvertRaw2Info(data, &info);
    if (!RFLiCheckValidInfo(&info)) {
        return NULL;
    }

    if (!RFLiIsValidOnNAND(&info)) {
        return NULL;
    }

    return data;
}

static void convertRaw2InfoCore_(const RFLiCharData* data, RFLiCharInfo* info) {
    info->faceline.type = data->faceType;
    info->faceline.color = data->faceColor;
    info->faceline.texture = data->faceTex;

    info->hair.type = data->hairType;
    info->hair.color = data->hairColor;
    info->hair.flip = data->hairFlip;

    info->eye.type = data->eyeType;
    info->eye.color = data->eyeColor;
    info->eye.scale = data->eyeScale;
    info->eye.rotate = data->eyeRotate;
    info->eye.x = data->eyeX;
    info->eye.y = data->eyeY;

    info->eyebrow.type = data->eyebrowType;
    info->eyebrow.color = data->eyebrowColor;
    info->eyebrow.scale = data->eyebrowScale;
    info->eyebrow.rotate = data->eyebrowRotate;
    info->eyebrow.x = data->eyebrowX;
    info->eyebrow.y = data->eyebrowY;

    info->nose.type = data->noseType;
    info->nose.scale = data->noseScale;
    info->nose.y = data->noseY;

    info->mouth.type = data->mouthType;
    info->mouth.color = data->mouthColor;
    info->mouth.scale = data->mouthScale;
    info->mouth.y = data->mouthY;

    info->beard.mustache = data->mustacheType;
    info->beard.type = data->beardType;
    info->beard.color = data->beardColor;
    info->beard.scale = data->beardScale;
    info->beard.y = data->beardY;

    info->glass.type = data->glassType;
    info->glass.color = data->glassColor;
    info->glass.scale = data->glassScale;
    info->glass.y = data->glassY;

    info->mole.type = data->moleType;
    info->mole.scale = data->moleScale;
    info->mole.x = data->moleX;
    info->mole.y = data->moleY;

    info->body.height = data->height;
    info->body.build = data->build;

    memcpy(info->personal.name, data->name, RFL_NAME_LEN * sizeof(wchar_t));
    info->personal.name[RFL_NAME_LEN] = L'\0';

    memcpy(&info->createID, &data->createID, sizeof(RFLCreateID));

    info->personal.sex = data->sex;
    info->personal.bmonth = data->birthMonth;
    info->personal.bday = data->birthDay;
    info->personal.color = data->favoriteColor;
    info->personal.favorite = data->favorite;
    info->personal.localOnly = data->localonly;
}

void RFLiConvertRaw2Info(const RFLiCharData* data, RFLiCharInfo* out) {
    convertRaw2InfoCore_(data, out);

    memcpy(out->personal.creator, data->creatorName,
           RFL_CREATOR_LEN * sizeof(wchar_t));
    out->personal.creator[RFL_CREATOR_LEN] = '\0';
}

void RFLiConvertHRaw2Info(const RFLiHiddenCharData* data, RFLiCharInfo* out) {
    convertRaw2InfoCore_((RFLiCharData*)data, out);

    memset(out->personal.creator, 0, RFL_CREATOR_LEN * sizeof(wchar_t));
    out->personal.creator[RFL_CREATOR_LEN] = '\0';

    out->personal.bmonth = 0;
    out->personal.bday = 0;
}

static void convertInfo2RawCore_(const RFLiCharInfo* info, RFLiCharData* data) {
    data->faceType = info->faceline.type;
    data->faceColor = info->faceline.color;
    data->faceTex = info->faceline.texture;

    data->hairType = info->hair.type;
    data->hairColor = info->hair.color;
    data->hairFlip = info->hair.flip;

    data->eyeType = info->eye.type;
    data->eyeColor = info->eye.color;
    data->eyeScale = info->eye.scale;
    data->eyeRotate = info->eye.rotate;
    data->eyeX = info->eye.x;
    data->eyeY = info->eye.y;

    data->eyebrowType = info->eyebrow.type;
    data->eyebrowColor = info->eyebrow.color;
    data->eyebrowScale = info->eyebrow.scale;
    data->eyebrowRotate = info->eyebrow.rotate;
    data->eyebrowX = info->eyebrow.x;
    data->eyebrowY = info->eyebrow.y;

    data->noseType = info->nose.type;
    data->noseScale = info->nose.scale;
    data->noseY = info->nose.y;

    data->mouthType = info->mouth.type;
    data->mouthColor = info->mouth.color;
    data->mouthScale = info->mouth.scale;
    data->mouthY = info->mouth.y;

    data->mustacheType = info->beard.mustache;
    data->beardType = info->beard.type;
    data->beardColor = info->beard.color;
    data->beardScale = info->beard.scale;
    data->beardY = info->beard.y;

    data->glassType = info->glass.type;
    data->glassColor = info->glass.color;
    data->glassScale = info->glass.scale;
    data->glassY = info->glass.y;

    data->moleType = info->mole.type;
    data->moleScale = info->mole.scale;
    data->moleX = info->mole.x;
    data->moleY = info->mole.y;

    data->height = info->body.height;
    data->build = info->body.build;

    memcpy(data->name, info->personal.name, RFL_NAME_LEN * sizeof(wchar_t));
    memcpy(&data->createID, &info->createID, sizeof(RFLCreateID));

    data->sex = info->personal.sex;
    data->birthMonth = info->personal.bmonth;
    data->birthDay = info->personal.bday;
    data->favoriteColor = info->personal.color;
    data->favorite = info->personal.favorite;
    data->localonly = info->personal.localOnly;
}

static void RFLiConvertInfo2Raw(const RFLiCharInfo* info, RFLiCharData* out) {
    convertInfo2RawCore_(info, out);
    memcpy(out->creatorName, info->personal.creator,
           RFL_CREATOR_LEN * sizeof(wchar_t));
}

static void RFLiConvertInfo2HRaw(const RFLiCharInfo* info, RFLiHiddenCharData* out) {
    convertInfo2RawCore_(info, (RFLiCharData*)out);
    out->birthPadding = 0;
}

static void RFLiConvertRaw2HRaw(const RFLiCharData* data, RFLiHiddenCharData* out) {
    memset(out, 0, sizeof(RFLiHiddenCharData));
    // Copy everything that also exists in hidden (just no creator name)
    memcpy(out, data, sizeof(RFLiCharData) - RFL_CREATOR_LEN * sizeof(wchar_t));
    out->birthPadding = 0;
}

static RFLErrcode RFLiGetCharRawData(RFLiCharData* out, u16 index) {
    RFLiCharData* data;

    if (out == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (index >= RFL_DB_CHAR_MAX) {
        return RFLErrcode_WrongParam;
    }

    data = RFLiGetCharData(index);
    if (data == NULL) {
        return RFLErrcode_Broken;
    }

    memcpy(out, data, sizeof(RFLiCharData));
    return RFLErrcode_Success;
}

RFLErrcode RFLiGetCharInfo(RFLiCharInfo* out, u16 index) {
    RFLiCharData* data;

    if (out == NULL) {
        return RFLErrcode_WrongParam;
    }

    if (index >= RFL_DB_CHAR_MAX) {
        return RFLErrcode_WrongParam;
    }

    data = RFLiGetCharData(index);
    if (data == NULL) {
        // Shouldn't this return RFLErrcode_Broken?
        return RFLErrcode_NotAvailable;
    }

    RFLiConvertRaw2Info(data, out);
    if (!RFLiCheckValidInfo(out)) {
        return RFLErrcode_Broken;
    }

    return RFLErrcode_Success;
}

BOOL RFLIsAvailableOfficialData(u16 index) {
    if (index >= RFL_DB_CHAR_MAX) {
        return FALSE;
    }

    return RFLiGetCharData(index) != NULL;
}

static void RFLiSetTemporaryID(RFLiCharInfo* info) {
    u32* dst = (u32*)&info->createID.data;
    dst[0] = RFLi_CREATE_ID_MASK_TEMPORARY;
    dst[1] = 0;
}

BOOL RFLiIsSameID(const RFLCreateID* id1, const RFLCreateID* id2) {
    const u32* ptr1 = (u32*)id1->data;
    const u32* ptr2 = (u32*)id2->data;

    if (id1 == NULL) {
        return FALSE;
    }

    if (id2 == NULL) {
        return FALSE;
    }

    if (!RFLiIsValidID(id1)) {
        return FALSE;
    }

    if (!RFLiIsValidID(id2)) {
        return FALSE;
    }

    if (RFLiIsTemporaryID(id1)) {
        return FALSE;
    }

    if (RFLiIsTemporaryID(id2)) {
        return FALSE;
    }

    if (ptr1[0] != ptr2[0]) {
        return FALSE;
    }

    if (ptr1[1] != ptr2[1]) {
        return FALSE;
    }

    return TRUE;
}

BOOL RFLiIsValidID(const RFLCreateID* id) {
    const u32* ptr = (u32*)id->data;

    if (id == NULL) {
        return FALSE;
    }

    return ptr[0] != 0 || ptr[1] != 0;
}

BOOL RFLiIsSpecialID(const RFLCreateID* id) {
    const u32* ptr = (u32*)id->data;

    if (id == NULL) {
        return FALSE;
    }

    if (!RFLiIsValidID(id)) {
        return FALSE;
    }

    if (RFLiIsTemporaryID(id)) {
        return FALSE;
    }

    return (*ptr & RFLi_CREATE_ID_MASK_NOT_SPECIAL) == 0 ? TRUE : FALSE;
}

BOOL RFLiIsTemporaryID(const RFLCreateID* id) {
    const u32* ptr = (u32*)id->data;

    if (id == NULL) {
        return FALSE;
    }

    if (!RFLiIsValidID(id)) {
        return FALSE;
    }

    return (*ptr & RFLi_CREATE_ID_MASK_TEMPORARY) != 0 ? TRUE : FALSE;
}

BOOL RFLSearchOfficialData(const RFLCreateID* id, u16* index) {
    u16 i;
    BOOL success;
    RFLiDBManager* mgr;
    RFLiCharData* head;

    success = FALSE;

    if (id == NULL) {
        return FALSE;
    }

    if (!RFLAvailable()) {
        return FALSE;
    }

    mgr = RFLiGetDBManager();
    if (mgr == NULL) {
        return FALSE;
    }

    head = mgr->database->rawData;
    for (i = 0; i < RFL_DB_CHAR_MAX; i++) {
        if (RFLIsAvailableOfficialData(i) &&
            RFLiIsSameID(&head[i].createID, id)) {
            success = TRUE;
            break;
        }
    }

    if (success == TRUE && index != NULL) {
        *index = i;
    }

    return success;
}

static BOOL RFLiIsValidName(const RFLiCharData* data) {
    if (data == NULL) {
        return FALSE;
    }

    return data->name[0] != L'\0';
}

BOOL RFLiIsValidName2(const RFLiCharInfo* info) {
    RFLiCharData data;

    if (info == NULL) {
        return FALSE;
    }

    RFLiConvertInfo2Raw(info, &data);
    return RFLiIsValidName(&data);
}

static BOOL RFLiGetIsolation(void) {
    return !RFLiDBIsLoaded() ? TRUE : RFLiGetDBManager()->database->isolation;
}

static RFLiHiddenDB* RFLiGetHiddenHeader(void) {
    if (!RFLiDBIsLoaded()) {
        return NULL;
    }

    return &RFLiGetDBManager()->database->hidden;
}

static BOOL RFLiDBIsLoaded(void) {
    if (!RFLAvailable()) {
        return FALSE;
    }

    if (RFLiGetDBManager() == NULL) {
        return FALSE;
    }

    return !RFLiNeedRepairError() && !RFLiNotFoundError();
}

static u16 RFLiCalculateCRC(const void* p, u32 len) {
    int i = 0;
    u16 crc = 0;
    const u8* current = (u8*)p;

    for (; i < len; current++, i++) {
        u8 data = *current;
        int j;

        for (j = 0; j < 8; j++, data <<= 1) {
            if (crc & 0x8000) {
                crc <<= 1;
                crc ^= 0x1021;
            } else {
                crc <<= 1;
            }

            if (data & 0x80) {
                crc ^= 0x1;
            }
        }
    }

    return crc;
}

static void alarmCreateCb_(OSAlarm* alarm, OSContext* ctx) {
#pragma unused(alarm)
#pragma unused(ctx)

    RFLiDBManager* mgr = RFLiGetDBManager();
    u8* current = mgr->crcInfo.current;
    u32 count = mgr->crcInfo.count;
    u16 crc = mgr->crcInfo.crc;
    u32 size = mgr->crcInfo.size;
    u32 curCount = 0;

    while (count < size) {
        u8 data = *current;
        int i;

        if (curCount >= ASYNC_CRC_STEP) {
            mgr->crcInfo.current = current;
            mgr->crcInfo.count = count;
            mgr->crcInfo.crc = crc;

            OSCancelAlarm(&mgr->crcInfo.alarm);
            OSSetAlarmUserData(&mgr->crcInfo.alarm, &mgr->crcInfo);
            OSSetAlarm(&mgr->crcInfo.alarm, OSMillisecondsToTicks(19),
                       alarmCreateCb_);
            return;
        }

        for (i = 0; i < 8; i++, data <<= 1) {
            if (crc & 0x8000) {
                crc <<= 1;
                crc ^= 0x1021;
            } else {
                crc <<= 1;
            }

            if (data & 0x80) {
                crc ^= 0x1;
            }
        }

        current++;
        count++;
        curCount++;
    }

    OSCancelAlarm(&mgr->crcInfo.alarm);
    mgr->crcInfo.callback(crc);
    mgr->database->hidden.crc = crc;
}

static void RFLiCreateHeaderCRCAsync(RFLiExCallback callback) {
    RFLiDBManager* mgr = RFLiGetDBManager();

    if (!RFLiIsWorking()) {
        RFLiStartWorking();
    }

    mgr->database->hidden.crc = 0;

    mgr->crcInfo.head = mgr->database;
    mgr->crcInfo.size = sizeof(RFLiDatabase);
    mgr->crcInfo.current = mgr->crcInfo.head;
    mgr->crcInfo.count = 0;
    mgr->crcInfo.crc = 0;
    mgr->crcInfo.callback = callback;

    OSCancelAlarm(&mgr->crcInfo.alarm);
    OSSetAlarmUserData(&mgr->crcInfo.alarm, &mgr->crcInfo);
    OSSetAlarm(&mgr->crcInfo.alarm, OSMillisecondsToTicks(19), alarmCreateCb_);
}

static void alarmCheckCb_(OSAlarm* alarm, OSContext* ctx) {
#pragma unused(alarm)
#pragma unused(ctx)

    RFLiDBManager* mgr = RFLiGetDBManager();
    u8* current = mgr->crcInfo.current;
    u32 count = mgr->crcInfo.count;
    u16 crc = mgr->crcInfo.crc;
    u32 size = mgr->crcInfo.size;
    u32 curCount = 0;

    while (count < size) {
        u8 data = *current;
        int i;

        if (curCount >= ASYNC_CRC_STEP) {
            mgr->crcInfo.current = current;
            mgr->crcInfo.count = count;
            mgr->crcInfo.crc = crc;

            OSCancelAlarm(&mgr->crcInfo.alarm);
            OSSetAlarmUserData(&mgr->crcInfo.alarm, &mgr->crcInfo);
            OSSetAlarm(&mgr->crcInfo.alarm, OSMillisecondsToTicks(19),
                       alarmCheckCb_);
            return;
        }

        for (i = 0; i < 8; i++, data <<= 1) {
            if (crc & 0x8000) {
                crc <<= 1;
                crc ^= 0x1021;
            } else {
                crc <<= 1;
            }

            if (data & 0x80) {
                crc ^= 0x1;
            }
        }

        current++;
        count++;
        curCount++;
    }

    OSCancelAlarm(&mgr->crcInfo.alarm);
    mgr->crcInfo.callback(crc);
}

static void RFLiCheckHeaderCRCAsync(RFLiExCallback callback) {
    RFLiDBManager* mgr = RFLiGetDBManager();

    if (!RFLiIsWorking()) {
        RFLiStartWorking();
    }

    mgr->crcInfo.head = mgr->database;
    mgr->crcInfo.size = sizeof(RFLiDatabase);
    mgr->crcInfo.current = mgr->crcInfo.head;
    mgr->crcInfo.count = 0;
    mgr->crcInfo.crc = 0;
    mgr->crcInfo.callback = callback;

    OSCancelAlarm(&mgr->crcInfo.alarm);
    OSSetAlarmUserData(&mgr->crcInfo.alarm, &mgr->crcInfo);
    OSSetAlarm(&mgr->crcInfo.alarm, OSMillisecondsToTicks(19), alarmCheckCb_);
}
