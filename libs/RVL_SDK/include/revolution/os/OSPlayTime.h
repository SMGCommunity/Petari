#ifndef OSPLAYTIME_H
#define OSPLAYTIME_H

#include <revolution/esp.h>

typedef enum {
    OSPLAYTIME_PERMANENT        = 0,
    OSPLAYTIME_LIMITED          = 1,
    OSPLAYTIME_LIMITED_LAUNCH   = 4,
    OSPLAYTIME_OTHER            = 9

} __OSPlayTimeType;

typedef void (*OSPlayTimeCallbackFunc)(void);

void __OSInitPlayTime(void);
BOOL __OSWriteExpiredFlagIfSet(void);
BOOL OSPlayTimeIsLimited(void);
s32 __OSGetPlayTime(ESTicketView *, __OSPlayTimeType *, u32 *);
BOOL __OSWriteExpiredFlag(void);

#endif // OSPLAYTIME_H
