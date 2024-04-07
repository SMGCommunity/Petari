#ifndef RVL_FACE_LIBRARY_DATA_UTILITY_H
#define RVL_FACE_LIBRARY_DATA_UTILITY_H
#include <RFL_Types.h>
#include <revolution/gx.h>
#include <revolution/types.h>
#include <cstdio>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct RFLMiddleDB;

typedef struct RFLAdditionalInfo {
    wchar_t name[RFL_NAME_LEN + 1];       // at 0x0
    wchar_t creator[RFL_CREATOR_LEN + 1]; // at 0x16
    RFLCreateID createID;                 // at 0x2C

    // at 0x34
    u32 sex : 1;
    u32 bmonth : 4;
    u32 bday : 5;
    u32 color : 4;
    u32 favorite : 1;
    u32 height : 7;
    u32 build : 7;
    u32 reserved : 3;

    GXColor skinColor; // at 0x38
} RFLAdditionalInfo;

RFLErrcode RFLGetAdditionalInfo(RFLAdditionalInfo* info, RFLDataSource source,
                                struct RFLMiddleDB* db, u16 index);

#ifdef __cplusplus
}
#endif
#endif
