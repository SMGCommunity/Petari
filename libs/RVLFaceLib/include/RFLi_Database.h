#ifndef RVL_FACE_LIBRARY_INTERNAL_DATABASE_H
#define RVL_FACE_LIBRARY_INTERNAL_DATABASE_H
#include <RFLi_HiddenDatabase.h>
#include <RFLi_Types.h>
#include <revolution/MEM.h>
#include <revolution/OS.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLiDatabase {
    u32 identifier;                        // at 0x0
    RFLiCharData rawData[RFL_DB_CHAR_MAX]; // at 0x4

    // at 0x1CEC
    u32 isolation : 1;
    u32 padding1 : 31;

    u8 specialInvite[13]; // at 0x1CF0
    u8 nwc24Month;        // at 0x1CFD
    u8 mwc24Day;          // at 0x1CFE
    u8 padding2;          // at 0x1CFF

    RFLiHiddenDB hidden; // at 0x1D00
} RFLiDatabase;

typedef struct RFLiCRCInfo {
    void* head;              // at 0x0
    u32 size;                // at 0x4
    u8* current;             // at 0x8
    u32 count;               // at 0xC
    u16 crc;                 // at 0x10
    OSAlarm alarm;           // at 0x18
    RFLiExCallback callback; // at 0x48
} RFLiCRCInfo;

typedef struct RFLiDBManager {
    RFLiDatabase* database;  // at 0x0
    RFLiCallback saveCb;     // at 0x4
    RFLiCallback formatCb;   // at 0x8
    RFLiCallback bootloadCb; // at 0xC
    RFLiOpenType saveType;   // at 0x10
    RFLiCharData trg;        // at 0x14
    RFLiCRCInfo crcInfo;     // at 0x60
} RFLiDBManager;

void RFLiInitDatabase(MEMiHeapHead* heap);
RFLErrcode RFLiBootLoadDatabaseAsync(RFLiCallback callback);
RFLErrcode RFLiSaveDatabaseAsync(RFLiCallback callback);
RFLiCharData* RFLiGetCharData(u16 index);
void RFLiConvertRaw2Info(const RFLiCharData* data, RFLiCharInfo* out);
void RFLiConvertHRaw2Info(const RFLiHiddenCharData* data, RFLiCharInfo* out);
void RFLiConvertInfo2Raw(const RFLiCharInfo* info, RFLiCharData* out);
void RFLiConvertInfo2HRaw(const RFLiCharInfo* info, RFLiHiddenCharData* out);
void RFLiConvertRaw2HRaw(const RFLiCharData* data, RFLiHiddenCharData* out);
RFLErrcode RFLiGetCharRawData(RFLiCharData* out, u16 index);
RFLErrcode RFLiGetCharInfo(RFLiCharInfo* out, u16 index);
void RFLiSetTemporaryID(RFLiCharInfo* info);
BOOL RFLiIsSameID(const RFLCreateID* id1, const RFLCreateID* id2);
BOOL RFLiIsValidID(const RFLCreateID* id);
BOOL RFLiIsSpecialID(const RFLCreateID* id);
BOOL RFLiIsTemporaryID(const RFLCreateID* id);
BOOL RFLiIsValidName(const RFLiCharData* data);
BOOL RFLiIsValidName2(const RFLiCharInfo* info);
BOOL RFLiGetIsolation(void);
RFLiHiddenDB* RFLiGetHiddenHeader(void);
BOOL RFLiDBIsLoaded(void);
u16 RFLiCalculateCRC(const void* p, u32 len);
void RFLiCreateHeaderCRCAsync(RFLiExCallback callback);
void RFLiCheckHeaderCRCAsync(RFLiExCallback callback);

#ifdef __cplusplus
}
#endif
#endif
