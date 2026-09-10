#ifndef RVL_SDK_NWC24_INTERNAL_DATE_PARSER_H
#define RVL_SDK_NWC24_INTERNAL_DATE_PARSER_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#include <revolution/nwc24/NWC24Utils.h>
#include <revolution/os.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NWC24i_SEC_MAX 60
#define NWC24i_MIN_MAX 60
#define NWC24i_HOUR_MAX 24

#define NWC24i_DAY_MIN 1

#define NWC24i_MONTH_MIN 1
#define NWC24i_MONTH_MAX 12

#define NWC24i_YEAR_MIN 1900

#define NWC24i_WEEK_DAY_MIN 1
#define NWC24i_WEEK_DAY_MAX 7

#define NWC24i_YEAR_DAY_MIN 1
#define NWC24i_YEAR_DAY_MAX 365

NWC24Err NWC24iDateToMinutes(s32* pMinutes, const NWC24Date* pDate);
s32 NWC24iGetDayOfWeek(u16 year, u8 month, u8 day);
NWC24Err NWC24iEpochSecondsToDate(NWC24Date* pDate, s64 timestamp);
NWC24Err NWC24iDateToOSCalendarTime(OSCalendarTime* pOSTime, const NWC24Date* pDate);
NWC24Err NWC24iIsValidDate(u16 year, u8 month, u8 day);

#ifdef __cplusplus
}
#endif
#endif
