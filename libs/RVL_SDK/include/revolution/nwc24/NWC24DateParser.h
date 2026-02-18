#ifndef NWC24DATEPARSER_H
#define NWC24DATEPARSER_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

NWC24Err NWC24iIsValidDate(u16 year, u8 month, u8 day);

typedef struct NWC24iDate {
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 min;
    u8 sec;
} NWC24iDate;

#ifdef __cplusplus
}
#endif

#endif  // NWC24DATEPARSER_H
