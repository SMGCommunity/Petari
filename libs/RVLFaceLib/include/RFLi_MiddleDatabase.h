#ifndef RVL_FACE_LIBRARY_INTERNAL_MIDDLE_DATABASE_H
#define RVL_FACE_LIBRARY_INTERNAL_MIDDLE_DATABASE_H
#include <RFL_MiddleDatabase.h>
#include <RFLi_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLiMiddleDB {
    RFLMiddleDBType type;     // at 0x0
    RFLiCallback callback;    // at 0x4
    RFLiHiddenCharData* data; // at 0x8
    u16 size;                 // at 0xC
    u16 storedSize;           // at 0xE
    u32 userData1;            // at 0x10
    u32 userData2;            // at 0x14
} RFLiMiddleDB;

RFLErrcode RFLiUpdateMiddleDBAsync(RFLMiddleDB* db, RFLiCallback cb,
                                   BOOL cache);
BOOL RFLiGetCharInfoMiddleDB(RFLiCharInfo* info, const RFLMiddleDB* db,
                             u16 index);
RFLErrcode RFLiAddMiddleDBUserData(RFLMiddleDB* db, RFLiCharData* raw);

#ifdef __cplusplus
}
#endif
#endif
