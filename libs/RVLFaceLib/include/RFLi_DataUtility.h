#ifndef RVL_FACE_LIBRARY_INTENRAL_DATA_UTILITY_H
#define RVL_FACE_LIBRARY_INTERNAL_DATA_UTILITY_H
#include <RFLi_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct RFLMiddleDB;

BOOL RFLiCheckValidInfo(const RFLiCharInfo* info);
BOOL RFLiIsValidOnNAND(const RFLiCharInfo* info);
BOOL RFLiIsSameFaceCore(const RFLiCharInfo* lhs, const RFLiCharInfo* rhs);
RFLErrcode RFLiPickupCharInfo(RFLiCharInfo* info, RFLDataSource source,
                              struct RFLMiddleDB* db, u16 index);
BOOL RFLiCheckBirthday(u8 month, u8 day);

#ifdef __cplusplus
}
#endif
#endif
